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

    // command-line options given by the user
    Options_Given options_given = {0, 0, 0, 0};
    Options options;
    if (init_options(&options) == -1)
    {
        print_err();
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
        free(options.logins);
        exit(EXIT_FAILURE);
    }

    // default option: list all available records for all users on all dates and times
    if (argc == 1)
    {
        NULL;
    }
    else // go through the options
    {
        run_options(&options_given, &options);
    }

    free(options.logins);
    exit(EXIT_SUCCESS);
}
