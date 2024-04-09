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
#include <fcntl.h>  // for open()
#include <unistd.h> // for close(), read()
#include <time.h>   // for struct tm, strptime(), strftime(), localtime()
#include <utmp.h>   // for struct utmp

// sizes of strings for options arguments
#define DATE_SIZE 9 // mm/dd/yy
#define PATHNAME_MAX 4096
#define TIME_SIZE 6 // HH:MM (24-hour clock)

// sizes of strings for a login record
#define LOG_ON_SIZE 15  // mm/dd/yy HH:MM
#define LOG_OFF_SIZE 18 // the above or "(still logged in)"

#define LOGINS_NUM 100
#define LOGIN_RECORDS_NUM 100

// #define DEFAULT_FILENAME "/var/log/wtmp"
#define DEFAULT_FILENAME "/home/axc/Dev/cpts360/pa/tattle/wtmp" // TEMP: only for testing purposes
#define DEFAULT_LOG_OFF "(still logged in)"

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
    char login[UT_NAMESIZE];
    char tty[UT_LINESIZE];
    char log_on[LOG_ON_SIZE];
    char log_off[LOG_OFF_SIZE];
    char from_host[UT_HOSTSIZE];
} Login_Record;

typedef struct
{
    Login_Record **records;
    int count;
} Login_Records;

#include "helpers.h"

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
 * @brief check if a ut_user from a utmp record is valid for a login record
 *
 * @param user ut_user string from a utmp record to check
 * @return int 0 = valid user | 1 = invalid user
 */
extern int invalid_user(const char *user);
/**
 * @brief fill a login record with information from a utmp struct
 *
 * @param record address to a Login_Record struct
 * @param login_record_info address to a utmp struct containing information about a login record
 */
extern void fill_record(Login_Record *record, struct utmp *login_record_info);
/**
 * @brief run the default actions of tattle
 *
 * @param login_records address of a Login_Records struct
 * @return int 0 = run success | -1 = run failure
 */
extern int run_options_default(Login_Records *login_records);
/**
 * @brief run the actions of any options that were specified by the user
 *
 * @param options_given address of an Options_Given struct containing which options are specified
 * @param options address of an Options struct filled with the values of the specified options
 * @return int 0 = run(s) success | -1 = run(s) failure
 */
extern int run_options(Options_Given *options_given, Options *options);

#endif
