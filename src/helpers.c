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

int login_records_mem(Login_Records *login_records)
{
    // login_records filled
    if (login_records->count % LOGIN_RECORDS_NUM == 0 && login_records->count != 0)
    {
        login_records->records = (Login_Record *)
            realloc(login_records->records, (login_records->count + LOGIN_RECORDS_NUM) * sizeof(Login_Record));
        if (login_records->records == NULL)
        {
            print_err();
            return -1; // error
        }
    }

    return 0;
}

void find_max_sizes(Login_Records_Sizes *login_records_sizes, Login_Records *login_records)
{
    int login_size = 0, tty_size = 0, from_host_size = 0;
    for (size_t record = 0; record < login_records->count; record++)
    {
        // login
        login_size = strlen(login_records->records[record].login);
        if (login_size > login_records_sizes->login_max)
        {
            login_records_sizes->login_max = login_size;
        }

        // tty
        tty_size = strlen(login_records->records[record].tty);
        if (tty_size > login_records_sizes->tty_max)
        {
            login_records_sizes->tty_max = tty_size;
        }

        // from_host
        from_host_size = strlen(login_records->records[record].from_host);
        if (from_host_size > login_records_sizes->from_host_max)
        {
            login_records_sizes->from_host_max = from_host_size;
        }
    }

    // set default sizes if they are too small
    if (login_size < 5)
    {
        login_records_sizes->login_max = 5;
    }
    if (tty_size < 3)
    {
        login_records_sizes->tty_max = 3;
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
    if (login_records->count == 0)
    {
        printf("tattle: no login records found\n");
    }
    else
    {
        Login_Records_Sizes login_records_sizes = {0, 0, 0, 0, 0};
        find_max_sizes(&login_records_sizes, login_records);

        printf("%-*s", login_records_sizes.login_max, "login");
        printf("%-*s", login_records_sizes.tty_max, "tty");
        printf("%-*s", login_records_sizes.log_on_max, "log on");
        printf("%-*s", login_records_sizes.log_off_max, "log off");
        // printf("%-*s\n", login_records_sizes.from_host_max, "from host");
        printf("%s\n", "from host");
        // output a row containing the information for a login record
        for (size_t record = 0; record < login_records->count; record++)
        {
            printf("%-*s", login_records_sizes.login_max, login_records->records[record].login);
            printf("%-*s", login_records_sizes.tty_max, login_records->records[record].tty);
            printf("%-*s", login_records_sizes.log_on_max, login_records->records[record].log_on);
            printf("%-*s", login_records_sizes.log_off_max, login_records->records[record].log_off);
            // printf("%-*s\n", login_records_sizes.from_host_max, login_record->from_host);
            printf("%s\n", login_records->records[record].from_host);
        }
    }
}

int compare_log_on_times(const void *login_record1, const void *login_record2)
{
    Login_Record *login1 = (Login_Record *)login_record1;
    Login_Record *login2 = (Login_Record *)login_record2;

    if (login1->log_on_time > login2->log_on_time)
    {
        return 1;
    }
    else if (login1->log_on_time < login2->log_on_time)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
