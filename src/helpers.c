/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include "helpers.h"

void print_err(void)
{
    fprintf(stderr, "%s\n", strerror(errno));
}

void usage(const char *progname)
{
    printf("usage: %s [-d date] [-f filename] [-t time] [-u login[,login]*]\n", progname);
}

void free_logins(Options *options)
{
    for (size_t login = 0; login < options->logins_count; login++)
    {
        free(options->logins[login]);
    }

    free(options->logins);
}

void free_login_records(Login_Records *login_records)
{
    for (size_t record = 0; record < login_records->count; record++)
    {
        free(login_records->records[record]);
    }

    free(login_records->records);
}

void find_max_sizes(Login_Records_Sizes *login_records_sizes, Login_Records *login_records)
{
    size_t login_size = 0, tty_size = 0, from_host_size = 0;
    Login_Record *login_record;
    for (size_t record = 0; record < login_records->count; record++)
    {
        login_record = login_records->records[record];

        // login
        login_size = strlen(login_record->login);
        if (login_size > login_records_sizes->login_max)
        {
            login_records_sizes->login_max = login_size;
        }

        // tty
        tty_size = strlen(login_record->tty);
        if (tty_size > login_records_sizes->tty_max)
        {
            login_records_sizes->tty_max = tty_size;
        }

        // from_host
        from_host_size = strlen(login_record->from_host);
        if (from_host_size > login_records_sizes->from_host_max)
        {
            login_records_sizes->from_host_max = from_host_size;
        }
    }
    // add padding to make the column separations more clear
    login_records_sizes->login_max += PRINT_RECORDS_SPACING;
    login_records_sizes->tty_max += PRINT_RECORDS_SPACING;
    login_records_sizes->log_on_max += LOG_ON_SIZE + PRINT_RECORDS_SPACING;
    login_records_sizes->log_off_max += LOG_OFF_SIZE + PRINT_RECORDS_SPACING;
    login_records_sizes->from_host_max += PRINT_RECORDS_SPACING;
}

void print_records(Login_Records *login_records)
{
    Login_Records_Sizes login_records_sizes = {0, 0, 0, 0, 0};
    find_max_sizes(&login_records_sizes, login_records);

    printf("%-*s", login_records_sizes.login_max, "login");
    printf("%-*s", login_records_sizes.tty_max, "tty");
    printf("%-*s", login_records_sizes.log_on_max, "log on");
    printf("%-*s", login_records_sizes.log_off_max, "log off");
    printf("%-*s\n", login_records_sizes.from_host_max, "from host");
    Login_Record *login_record;
    for (size_t record = 0; record < login_records->count; record++)
    {
        login_record = login_records->records[record];
        printf("%-*s", login_records_sizes.login_max, login_record->login);
        printf("%-*s", login_records_sizes.tty_max, login_record->tty);
        printf("%-*s", login_records_sizes.log_on_max, login_record->log_on);
        printf("%-*s", login_records_sizes.log_off_max, login_record->log_off);
        printf("%-*s\n", login_records_sizes.from_host_max, login_record->from_host);
    }
}
