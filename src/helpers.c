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
