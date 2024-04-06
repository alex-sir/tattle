/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include "options.h"

int options_init(Options *options)
{
    strncpy(options->date, "", DATE_SIZE);
    strncpy(options->filename, "/var/log/wtmp", PATHNAME_MAX);
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
        break;
    case '?': // '?': encountered option not in the valid options list
        return -1;
    }

    return 0;
}

int run_options_default(void)
{
    return 0;
}

int run_options(Options_Given *options_given, Options *options)
{
    if (options_given->date)
    {
        // date is specified but time is not: report all logins on the date
        printf("date: %s\n", options->date);
    }
    if (options_given->filename)
    {
        printf("filename: %s\n", options->filename);
    }
    if (options_given->time)
    {
        // time is specified but date is not: default to current date
        printf("time: %s\n", options->time);
    }
    if (options_given->logins)
    {
        printf("logins: ");
        for (size_t i = 0; i < options->logins_count; i++)
        {
            i != options->logins_count - 1 ? printf("%s,", options->logins[i]) : printf("%s\n", options->logins[i]);
        }
    }

    return 0;
}
