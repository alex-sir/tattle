/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef OPTIONS
#define OPTIONS

#include <stdio.h>  // for printf()
#include <stdlib.h> // for realloc(), getsubopt()
#include <unistd.h> // for pathconf(), sysconf()

#include "helpers.h"

// for array that will hold ints representing if an option is given
#define OPTIONS_NUM 4
#define OPTION_DATE 0
#define OPTION_FILENAME 1
#define OPTION_TIME 2
#define OPTION_LOGINS 3

// sizes of strings for options arguments
#define DATE_SIZE 9 // mm/dd/yy
#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX)
#define TIME_SIZE 6 // HH:MM (24-hour clock)
#define LOGIN_MAX sysconf(_SC_LOGIN_NAME_MAX)

#define LOGINS_NUM 100

typedef struct
{
    char *login;
    char *tty;
    char *log_on;
    char *log_off;
    char *from_host;
} login_record;

extern int check_date(const char *date);
extern int check_time(const char *time);
extern int fill_logins(char ***logins, int *logins_count, char *optarg);

#endif
