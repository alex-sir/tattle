/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include <stdio.h>  // for printf()
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

    // values for the options
    Options_Given options_given = {0, 0, 0, 0};
    Options options;
    if (options_init(&options) == -1)
    {
        print_err();
        exit(EXIT_FAILURE);
    }

    // command-line options are given by the user
    if (argc > 1)
    {
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
            free(options.logins);
            exit(EXIT_FAILURE);
        }
        // argument given after a space (e.g. -f file.txt)
        if (options_given.filename && options.filename[0] == '\0' && optind < argc)
        {
            strncpy(options.filename, argv[optind], PATHNAME_MAX);
        }
    }

    /*
        default option: list all available records for all users on all dates and times
        otherwise: go through the options the user specified
    */
    if (run_options(&options_given, &options) == -1)
    {
        free(options.logins);
        exit(EXIT_FAILURE);
    }

    free(options.logins);
    exit(EXIT_SUCCESS);
}
