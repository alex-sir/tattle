/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include <stdio.h>  // for printf()
#include <stdlib.h> // for exit(), getsubopt(), malloc()
#include <unistd.h> // for getopt(), sysconf()
#include <string.h> // for strncpy()

#include "options.h"
#include "helpers.h"

#define DATE_SIZE 9 // mm/dd/yy
#define TIME_SIZE 6 // HH:MM (24-hour clock)
#define LOGIN_MAX sysconf(_SC_LOGIN_NAME_MAX)
#define LOGINS_NUM 100

int main(int argc, char *argv[])
{
    if (LOGIN_MAX == -1)
    {
        print_err();
        exit(EXIT_FAILURE);
    }

    const char *progname = argv[0];
    int opt = 0, date_given = 0, filename_given = 0, time_given = 0, logins_given = 0;
    char date[DATE_SIZE] = "", filename[FILENAME_MAX], time[TIME_SIZE];
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
        case 'd': // date
            date_given = 1;
            strncpy(date, optarg, DATE_SIZE);
            break;
        case 'f': // filename
            filename_given = 1;
            if (optarg != 0)
            {
                strncpy(filename, optarg, FILENAME_MAX);
            }
            break;
        case 't': // time
            time_given = 1;
            strncpy(time, optarg, TIME_SIZE);
            break;
        case 'u': // login(s)
            logins_given = 1;
            char *subopts = optarg, *value;
            char *const token[] = {NULL};
            do
            {
                if (logins_count % LOGINS_NUM == 0 && logins_count != 0)
                {
                    logins = (char **)realloc(logins, (logins_count + LOGINS_NUM) * sizeof(char *));
                    if (logins == NULL)
                    {
                        print_err();
                        free(logins);
                        exit(EXIT_FAILURE);
                    }
                }
                getsubopt(&subopts, token, &value);
                logins[logins_count] = value;
                logins_count++;
            } while (*subopts != '\0');
            break;
        case '?': // '?': encountered option not in the valid options list
            usage(progname);
            free(logins);
            exit(EXIT_FAILURE);
        }
    }

    // default option: list all available records for all users on all dates and times
    if (argc == 1)
    {
    }

    // go through all the options
    if (date_given)
    {
        // date is specified but time is not: report all logins on the date
    }
    if (filename_given)
    {
        strncpy(filename, argv[optind], FILENAME_MAX);
        printf("%s\n", filename);
    }
    if (time_given)
    {
        // time is specified but date is not: default to current date
    }
    if (logins_given)
    {
        for (size_t i = 0; i < logins_count; i++)
        {
            printf("%s\n", logins[i]);
        }
    }

    free(logins);
    exit(EXIT_SUCCESS);
}
