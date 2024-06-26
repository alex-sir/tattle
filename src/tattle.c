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
    if (login_records.records == NULL)
    {
        print_err();
        exit(EXIT_FAILURE);
    }
    // command-line options are given by the user
    if (argc > 1)
    {
        // values for the options & arguments
        Options_Given options_given = {0, 0, 0, 0};
        Arguments arguments;
        if (arguments_init(&arguments) == -1)
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
            if (check_options(&options_given, &arguments, opt, optarg) == 0)
            {
                continue;
            }

            if (opt == '?')
            {
                usage(argv[0]);
            }
            free(login_records.records);
            free_logins(&arguments);
            exit(EXIT_FAILURE);
        }
        // filename argument given after a space (e.g. -f file.txt)
        if (options_given.filename && optind < argc && strcmp(arguments.filename, DEFAULT_FILENAME) == 0)
        {
            strncpy(arguments.filename, argv[optind], PATHNAME_MAX);
        }

        // go through the options the user specified
        if (fill_login_records(&login_records, &arguments, &options_given) == -1)
        {
            free(login_records.records);
            free_logins(&arguments);
            exit(EXIT_FAILURE);
        }

        // filter login records if a date, time, or a list of logins is given by the user
        if (options_given.date || options_given.time || options_given.logins)
        {
            Login_Records login_records_ft = {(Login_Record *)malloc(LOGIN_RECORDS_NUM * sizeof(Login_Record)), 0};
            if (login_records_ft.records == NULL)
            {
                print_err();
                free(login_records.records);
                free_logins(&arguments);
                exit(EXIT_FAILURE);
            }
            filter_login_records(&login_records_ft, &login_records, &arguments, &options_given);
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
        free_logins(&arguments);
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
