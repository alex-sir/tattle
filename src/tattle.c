/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit, malloc(), qsort()
#include <unistd.h> // for getopt()
#include <string.h> // for strncpy(), strcmp()

#include "options.h"
#include "helpers.h"

#define MAX_ARGS 9

int main(int argc, char *argv[])
{
    if (argc > MAX_ARGS)
    {
        fprintf(stderr, "tattle: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    Login_Records login_records = {(Login_Record *)malloc(LOGIN_RECORDS_NUM * sizeof(Login_Record)), 0};
    // command-line options are given by the user
    if (argc > 1)
    {
        // values for the options
        Options_Given options_given = {0, 0, 0, 0};
        Options options;
        if (options_init(&options) == -1)
        {
            print_err();
            free(login_records.records);
            exit(EXIT_FAILURE);
        }
        int opt = 0;
        extern char *optarg;
        extern int optind;

        // parse all options given
        while ((opt = getopt(argc, argv, "d:f::t:u:")) != -1)
        {
            if (check_options(&options_given, &options, opt, optarg) == 0)
            {
                continue;
            }

            if (opt == '?')
            {
                usage(argv[0]);
            }
            free(login_records.records);
            free_logins(&options);
            exit(EXIT_FAILURE);
        }
        // filename argument given after a space (e.g. -f file.txt)
        if (options_given.filename && optind < argc && strcmp(options.filename, DEFAULT_FILENAME) == 0)
        {
            strncpy(options.filename, argv[optind], PATHNAME_MAX);
        }

        // go through the options the user specified
        if (fill_login_records(&login_records, &options, &options_given) == -1)
        {
            free(login_records.records);
            free_logins(&options);
            exit(EXIT_FAILURE);
        }

        // filter login records if a time or date is given by the user
        if (options_given.time || options_given.date)
        {
            Login_Records login_records_ft = {(Login_Record *)malloc(LOGIN_RECORDS_NUM * sizeof(Login_Record)), 0};
            filter_login_records(&login_records_ft, &login_records, &options, &options_given);
            qsort(login_records_ft.records, login_records_ft.count, sizeof(Login_Record), compare_log_on_times);
            print_records(&login_records_ft);
            free(login_records_ft.records);
        }
        else
        {
            qsort(login_records.records, login_records.count, sizeof(Login_Record), compare_log_on_times);
            print_records(&login_records);
        }

        free(login_records.records);
        free_logins(&options);
    }
    else // default option: list all available records for all users on all dates and times
    {
        if (fill_login_records_d(&login_records) == -1)
        {
            free(login_records.records);
            exit(EXIT_FAILURE);
        }

        qsort(login_records.records, login_records.count, sizeof(Login_Record), compare_log_on_times);
        print_records(&login_records);
        free(login_records.records);
    }

    exit(EXIT_SUCCESS);
}
