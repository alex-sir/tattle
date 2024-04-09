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
    for (size_t i = 0; i < options->logins_count; i++)
    {
        free(options->logins[i]);
    }

    free(options->logins);
}

void free_login_records(Login_Records *login_records)
{
    for (size_t i = 0; i < login_records->count; i++)
    {
        free(login_records->records[i]);
    }

    free(login_records->records);
}

void print_record(Login_Record *login_record)
{
    printf("name           contents\n");
    printf("login          %s\n", login_record->login);
    printf("tty            %s\n", login_record->tty);
    printf("log on         %s\n", login_record->log_on);
    printf("log off        %s\n", login_record->log_off);
    printf("from host      %s\n\n", login_record->from_host);
}
