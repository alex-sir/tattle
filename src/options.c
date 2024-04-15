/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include "options.h"

int options_init(Options *options)
{
    strncpy(options->date, "", DATE_SIZE);
    strncpy(options->filename, DEFAULT_FILENAME, PATHNAME_MAX);
    strncpy(options->time, "", TIME_SIZE);
    options->logins = (char **)malloc(LOGINS_NUM * sizeof(char *));
    if (options->logins == NULL)
    {
        return -1;
    }
    options->logins_count = 0;

    return 0;
}

int check_date(const char *date)
{
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    char *date_result = strptime(date, "%D", &tm);
    if (date_result == NULL || date_result[0] != '\0')
    {
        return -1;
    }

    return 0;
}

int check_time(const char *time)
{
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    char *time_result = strptime(time, "%R", &tm);
    if (time_result == NULL || time_result[0] != '\0')
    {
        return -1;
    }

    return 0;
}

int fill_logins(char ***logins, char *optarg)
{
    int logins_count = 0;
    char *subopts = optarg, *value;
    char *const token[] = {NULL};

    do
    {
        // logins array filled up
        if (logins_count % LOGINS_NUM == 0 && logins_count != 0)
        {
            *logins = (char **)realloc(*logins, (logins_count + LOGINS_NUM) * sizeof(char *));
            if (*logins == NULL)
            {
                return -1;
            }
        }
        // get each of the login strings
        getsubopt(&subopts, token, &value);
        (*logins)[logins_count] = (char *)malloc(UT_NAMESIZE);
        if ((*logins)[logins_count] == NULL)
        {
            return -1;
        }
        strncpy((*logins)[logins_count], value, UT_NAMESIZE);
        logins_count++;
    } while (*subopts != '\0');

    return logins_count;
}

int verify_logins(Options *options)
{
    for (size_t i = 0; i < options->logins_count; i++)
    {
        // check that the login is a real login on the system
        if (getpwnam(options->logins[i]) == NULL)
        {
            fprintf(stderr, "tattle: user '%s' not found\n", options->logins[i]);
            return -1; // error
        }
    }

    return 0; // ok
}

int check_options(Options_Given *options_given, Options *options, const char opt, char *optarg)
{
    switch (opt)
    {
    case 'd': // date (mm/dd/yy)
        if (check_date(optarg) == -1)
        {
            fprintf(stderr, "%s: invalid date\nTry as 'mm/dd/yy'.\n", optarg);
            return -1;
        }
        options_given->date = 1;
        strncpy(options->date, optarg, DATE_SIZE);
        break;
    case 'f': // filename
        options_given->filename = 1;
        if (optarg)
        {
            strncpy(options->filename, optarg, PATHNAME_MAX);
        }
        break;
    case 't': // time (HH:MM) (24-hour clock)
        if (check_time(optarg) == -1)
        {
            fprintf(stderr, "%s: invalid time\nTry as 'HH:MM' (24-hour clock).\n", optarg);
            return -1;
        }
        options_given->time = 1;
        strncpy(options->time, optarg, TIME_SIZE);
        break;
    case 'u': // login(s) (user1,user2,user3)
        options_given->logins = 1;
        options->logins_count = fill_logins(&options->logins, optarg);
        if (options->logins_count == -1)
        {
            print_err();
            return -1;
        }
        if (verify_logins(options) == -1)
        {
            return -1;
        }
        break;
    case '?': // '?': encountered option not in the valid options list
        return -1;
    }

    return 0;
}

int invalid_user(const char *user)
{
    if (strncmp(user, "reboot", UT_NAMESIZE) == 0 ||
        strncmp(user, "LOGIN", UT_NAMESIZE) == 0 ||
        strncmp(user, "runlevel", UT_NAMESIZE) == 0 ||
        strncmp(user, "shutdown", UT_NAMESIZE) == 0 ||
        strncmp(user, "", UT_NAMESIZE) == 0)
    {
        return 1;
    }

    return 0;
}

void get_output_time(char *log, const size_t log_size, int32_t tv_sec)
{
    time_t tv_sec_time = tv_sec;
    struct tm *login_record_time = localtime(&tv_sec_time);
    strftime(log, log_size, "%D %R", login_record_time);
}

void log_off_all(Login_Records *login_records, struct utmp *login_record_info)
{
    char log_off[LOG_OFF_SIZE];
    get_output_time(log_off, LOG_OFF_SIZE, login_record_info->ut_tv.tv_sec);

    for (size_t record = 0; record < login_records->count; record++)
    {
        if (login_records->records[record].is_pending)
        {
            strncpy(login_records->records[record].log_off, log_off, LOG_OFF_SIZE);
            login_records->records[record].is_pending = 0;
        }
    }
}

void fill_record(Login_Record *record, struct utmp *login_record_info)
{
    strncpy(record->login, login_record_info->ut_user, UT_NAMESIZE);
    strncpy(record->tty, login_record_info->ut_line, UT_LINESIZE);

    // get the log on string
    char log_on[LOG_ON_SIZE];
    get_output_time(log_on, LOG_ON_SIZE, login_record_info->ut_tv.tv_sec);
    strncpy(record->log_on, log_on, LOG_ON_SIZE);

    strncpy(record->log_off, DEFAULT_LOG_OFF, LOG_OFF_SIZE);
    strncpy(record->from_host, login_record_info->ut_host, UT_HOSTSIZE);
    record->is_pending = 1;

    time_t tv_sec_time = login_record_info->ut_tv.tv_sec;
    record->log_on_time = tv_sec_time;
}

void find_log_off(Login_Records *login_records, struct utmp *login_record_info)
{
    for (size_t record = 0; record < login_records->count; record++)
    {
        if (strcmp(login_records->records[record].tty, login_record_info->ut_line) == 0 &&
            login_records->records[record].is_pending)
        {
            // get the log off string
            char log_off[LOG_OFF_SIZE];
            get_output_time(log_off, LOG_OFF_SIZE, login_record_info->ut_tv.tv_sec);
            strncpy(login_records->records[record].log_off, log_off, LOG_OFF_SIZE);
            login_records->records[record].is_pending = 0;
            break;
        }
    }
}

int check_record_type(Login_Records *login_records, struct utmp *login_record_info)
{
    // a log off can be represented by either a DEAD_PROCESS or BOOT_TIME record
    switch (login_record_info->ut_type)
    {
    case BOOT_TIME: // system reboot: any pending login sessions from the previous session are logged off
        log_off_all(login_records, login_record_info);
        break;
    case USER_PROCESS: // log on: create a new login record
        if (invalid_user(login_record_info->ut_user))
        {
            return 1; // continue
        }

        if (login_records_mem(login_records) == -1)
        {
            return -1; // error
        }
        // REM: watch out for a log on followed by another log on (with the same ut_line) with no intervening log off
        // FIXME: this doesn't work, it gives false positives. maybe try doing at the end after finished with all the records?
        // find_log_off(login_records, &login_record_info); // look for the rare case of a record with no log off
        fill_record(&(login_records->records[login_records->count]), login_record_info);
        login_records->count++;
        break;
    case DEAD_PROCESS: // log off (has same ut_line as log on from USER_PROCESS)
        find_log_off(login_records, login_record_info);
        break;
    default: // irrelevant ut_type
        break;
    }

    return 0; // ok
}

int fill_login_records_d(Login_Records *login_records)
{
    int login_records_file = open(DEFAULT_FILENAME, O_RDONLY);
    if (login_records_file == -1)
    {
        print_err();
        return -1;
    }
    struct utmp login_record_info;
    int check_result;

    // go through every login record in the file
    while (read(login_records_file, &login_record_info, sizeof(struct utmp)))
    {
        check_result = check_record_type(login_records, &login_record_info);
        if (check_result == 1)
        {
            continue;
        }
        else if (check_result == -1)
        {
            close(login_records_file);
            return -1;
        }
    }

    close(login_records_file);
    return 0;
}

int fill_login_records(Login_Records *login_records, Options *options, Options_Given *options_given)
{
    int login_records_file;
    // filter for the user-specified file
    if (options_given->filename)
    {
        login_records_file = open(options->filename, O_RDONLY);
    }
    else
    {
        login_records_file = open(DEFAULT_FILENAME, O_RDONLY);
    }
    if (login_records_file == -1)
    {
        print_err();
        return -1;
    }
    struct utmp login_record_info;
    int check_record_result;

    // go through every login record in the file
    while (read(login_records_file, &login_record_info, sizeof(struct utmp)))
    {
        check_record_result = check_record_type(login_records, &login_record_info);
        if (check_record_result == 1)
        {
            continue;
        }
        else if (check_record_result == -1)
        {
            close(login_records_file);
            return -1;
        }
    }

    close(login_records_file);
    return 0;
}

int check_date_filter(Options *options, Options_Given *options_given, const char log_on[], const char log_off[])
{
    char current_log_on[LOG_ON_SIZE];
    char *current_date;
    strncpy(current_log_on, log_on, LOG_ON_SIZE);
    current_date = strtok(current_log_on, " ");

    struct tm tm_option, tm_current;
    memset(&tm_option, 0, sizeof(struct tm));
    memset(&tm_current, 0, sizeof(struct tm));

    // FIXME: records that are "(still logged in)" but at a future date show up yet they shouldn't (look at handout)
    time_t time_option, time_current;
    strptime(options->date, "%D", &tm_option);
    time_option = mktime(&tm_option);
    strptime(current_date, "%D", &tm_current);
    time_current = mktime(&tm_current);

    // date is specified but time is not: report all logins active on that date
    if (time_option != time_current &&
        strcmp(log_off, DEFAULT_LOG_OFF) != 0)
    {
        return 1; // continue
    }

    // date AND time is specified
    if (options_given->time)
    {
    }

    return 0; // ok
}

int check_time_filter(Options *options, Options_Given *options_given, const char log_on[], const char log_off[])
{
    char current_log_on[LOG_ON_SIZE];
    char *current_time;
    strncpy(current_log_on, log_on, LOG_ON_SIZE);
    strtok(current_log_on, " ");
    current_time = strtok(NULL, "\0");

    struct tm tm_option, tm_current;
    memset(&tm_option, 0, sizeof(struct tm));
    memset(&tm_current, 0, sizeof(struct tm));

    time_t time_option, time_current;
    strptime(options->time, "%D", &tm_option);
    time_option = mktime(&tm_option);
    strptime(current_time, "%D", &tm_current);
    time_current = mktime(&tm_current);

    // time is specified but date is not: default to current date
    if (time_option != time_current)
    {
        return 1; // continue
    }

    // time AND date are specified
    if (options_given->date)
    {
    }

    return 0; // ok
}

int check_login_filter(Options *options, const char login[])
{
    // check if "login" is within the user-specified options->logins
    for (size_t i = 0; i < options->logins_count; i++)
    {
        if (strcmp(options->logins[i], login) == 0)
        {
            return 0; // ok
        }
    }

    return 1; // continue
}

int filter_login_records(Login_Records *login_records_ft, Login_Records *login_records,
                         Options *options, Options_Given *options_given)
{
    int check_date_result = 0, check_time_result = 0, check_login_result = 0;

    // TODO: do date, time, and logins filtering
    for (size_t i = 0; i < login_records->count; i++)
    {
        if (options_given->date)
        {
            check_date_result = check_date_filter(options, options_given, login_records->records[i].log_on,
                                                  login_records->records[i].log_off);
            if (check_date_result == 1)
            {
                continue;
            }
        }

        if (options_given->time)
        {
            check_time_result = check_time_filter(options, options_given, login_records->records[i].log_on,
                                                  login_records->records[i].log_off);
            if (check_time_result == 1)
            {
                continue;
            }
        }

        // filter for the user-specified logins
        if (options_given->logins)
        {
            check_login_result = check_login_filter(options, login_records->records[i].login);
            if (check_login_result == 1)
            {
                continue;
            }
        }

        if (login_records_mem(login_records) == -1)
        {
            return -1; // error
        }
        login_records_ft->records[login_records_ft->count] = login_records->records[i];
        login_records_ft->count++;
    }

    return 0;
}
