/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include <stdio.h>  // for printf(), fprintf()
#include <stdlib.h> // for exit, malloc()
#include <unistd.h> // for getopt()
#include <string.h> // for strncpy()

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

    if (LOGIN_MAX == -1)
    {
        print_err();
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    int option_given[OPTIONS_NUM] = {0};
    char date[DATE_SIZE], filename[PATHNAME_MAX], time[TIME_SIZE];
    char **logins = (char **)malloc(LOGINS_NUM * sizeof(char *));
    if (logins == NULL)
    {
        print_err();
        exit(EXIT_FAILURE);
    }
    int logins_count = 0;

    extern char *optarg;
    extern int optind;
    // parse all options given
    while ((opt = getopt(argc, argv, "d:f::t:u:")) != -1)
    {
        switch (opt)
        {
        case 'd': // date (mm/dd/yy)
            if (check_date(optarg) == -1)
            {
                fprintf(stderr, "%s: incorrect date\nTry as 'mm/dd/yy'.\n", optarg);
                free(logins);
                exit(EXIT_FAILURE);
            }
            option_given[OPTION_DATE] = 1;
            strncpy(date, optarg, DATE_SIZE);
            break;
        case 'f': // filename
            option_given[OPTION_FILENAME] = 1;
            break;
        case 't': // time (HH:MM) (24-hour clock)
            if (check_time(optarg) == -1)
            {
                fprintf(stderr, "%s: incorrect time\nTry as 'HH:MM' (24-hour clock).\n", optarg);
                free(logins);
                exit(EXIT_FAILURE);
            }
            option_given[OPTION_TIME] = 1;
            strncpy(time, optarg, TIME_SIZE);
            break;
        case 'u': // login(s) (user1,user2,user3)
            option_given[OPTION_LOGINS] = 1;
            logins_count = fill_logins(&logins, optarg);
            if (logins_count == -1)
            {
                print_err();
                free(logins);
                exit(EXIT_FAILURE);
            }
            break;
        case '?': // '?': encountered option not in the valid options list
            usage(argv[0]);
            free(logins);
            exit(EXIT_FAILURE);
        }
    }

    // default option: list all available records for all users on all dates and times
    if (argc == 1)
    {
    }

    // go through all the options
    if (option_given[OPTION_DATE])
    {
        // date is specified but time is not: report all logins on the date
        printf("date: %s\n", date);
    }
    if (option_given[OPTION_FILENAME])
    {
        if (optind < argc)
        {
            strncpy(filename, argv[optind], PATHNAME_MAX);
        }
        else
        {
            strncpy(filename, "/var/log/wtmp", PATHNAME_MAX);
        }
        printf("filename: %s\n", filename);
    }
    if (option_given[OPTION_TIME])
    {
        // time is specified but date is not: default to current date
        printf("time: %s\n", time);
    }
    if (option_given[OPTION_LOGINS])
    {
        printf("logins: ");
        for (size_t i = 0; i < logins_count; i++)
        {
            i != logins_count - 1 ? printf("%s,", logins[i]) : printf("%s\n", logins[i]);
        }
    }

    free(logins);
    exit(EXIT_SUCCESS);
}
