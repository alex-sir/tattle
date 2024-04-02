/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#include "options.h"

int check_date(const char *date)
{
    return 0;
}

int check_time(const char *time)
{
    return 0;
}

int fill_logins(char ***logins, int *logins_count, char *optarg)
{
    char *subopts = optarg, *value;
    char *const token[] = {NULL};
    do
    {
        // logins array filled up
        if (*logins_count % LOGINS_NUM == 0 && *logins_count != 0)
        {
            *logins = (char **)realloc(*logins, (*logins_count + LOGINS_NUM) * sizeof(char *));
            if (*logins == NULL)
            {
                return -1;
            }
        }
        getsubopt(&subopts, token, &value);
        (*logins)[*logins_count] = value;
        (*logins_count)++;
    } while (*subopts != '\0');

    return 0;
}
