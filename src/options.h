/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef OPTIONS
#define OPTIONS

// gets rid of some weird warnings with library functions
#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>  // for printf(), fprintf()
#include <stdlib.h> // for realloc(), getsubopt()
#include <string.h> // for strncpy()
#include <time.h>   // for strptime()

#include "helpers.h"

// sizes of strings for options arguments
#define DATE_SIZE 9 // mm/dd/yy
#define PATHNAME_MAX 4096
#define TIME_SIZE 6 // HH:MM (24-hour clock)
#define LOGIN_MAX 32

#define LOGINS_NUM 100

typedef struct
{
    int date;
    int filename;
    int time;
    int logins;
} Options_Given;

typedef struct
{
    char date[DATE_SIZE];
    char filename[PATHNAME_MAX];
    char time[TIME_SIZE];
    char **logins;
    int logins_count;
} Options;

typedef struct
{
    char *login;
    char *tty;
    char *log_on;
    char *log_off;
    char *from_host;
} login_record;

int init_options(Options *options);
/**
 * @brief validate the format of a date string as mm/dd/yy
 *
 * @param date date string to validate
 * @return int 0: valid date format | -1: invalid date format
 */
extern int check_date(const char *date);
/**
 * @brief validate the format of a time string as HH:MM (24-hour clock)
 *
 * @param time time string to validate
 * @return int 0: valid time format | -1: invalid time format
 */
extern int check_time(const char *time);
/**
 * @brief fill an array with login name strings
 *
 * @param logins address to an array of login name strings
 * @param optarg command-line argument from getopt() specifying the logins
 * @return int number of logins filled | -1: logins fill unsuccessful
 */
extern int fill_logins(char ***logins, char *optarg);
extern int check_options(Options_Given *options_given, Options *options, const char opt, char *optarg);
extern int run_options(Options_Given *options_given, Options *options);

#endif
