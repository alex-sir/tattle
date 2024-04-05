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

/**
 * @brief initialize an Options struct with default values
 *
 * @param options address of an Options struct
 * @return int 0 = init success | -1 = init failure
 */
extern int options_init(Options *options);
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
/**
 * @brief check all command-line options and corresponding arguments specified by the user
 *
 * @param options_given address of an Options_Given struct containing which options are specified
 * @param options address of an Options struct that will get filled with the values of the specified options
 * @param opt option specified
 * @param optarg argument of the option specfied "opt"
 * @return int 0 = options check success | -1 = options check failure
 */
extern int check_options(Options_Given *options_given, Options *options, const char opt, char *optarg);
/**
 * @brief run the actions of any options that were specified by the user
 *
 * @param options_given address of an Options_Given struct containing which options are specified
 * @param options address of an Options struct filled with the values of the specified options
 * @return int 0 = run(s) success | -1 = run(s) failure
 */
extern int run_options(Options_Given *options_given, Options *options);

#endif
