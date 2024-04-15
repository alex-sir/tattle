/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef OPTIONS
#define OPTIONS

// gets rid of some weird warnings with library functions (strftime(), getsubopt())
#define _GNU_SOURCE

#include <stdio.h>  // for printf(), fprintf()
#include <stdlib.h> // for realloc(), getsubopt()
#include <string.h> // for strncpy(), strcmp()
#include <fcntl.h>  // for open()
#include <unistd.h> // for close(), read()
#include <time.h>   // for struct tm, strptime(), strftime(), localtime()
#include <utmp.h>   // for struct utmp
#include <pwd.h>    // for getpwnam()
#include <errno.h>  // for errno

// sizes of strings for options arguments
#define DATE_SIZE 9 // mm/dd/yy
#define PATHNAME_MAX 4096
#define TIME_SIZE 6 // HH:MM (24-hour clock)

// sizes of strings for a login record
#define LOG_ON_SIZE 15  // mm/dd/yy HH:MM
#define LOG_OFF_SIZE 18 // the above or "(still logged in)"

#define LOGINS_NUM 100
#define LOGIN_RECORDS_NUM 100

#define DEFAULT_FILENAME "/var/log/wtmp"
// TEMP: only for testing purposes
// #define DEFAULT_FILENAME "/home/axc/Dev/cpts360/pa/tattle/test/wtmp"
// #define DEFAULT_FILENAME "/home/axc/Dev/cpts360/pa/tattle/test/wtmp_elec_2022_04_27"
// #define DEFAULT_FILENAME "/home/axc/Dev/cpts360/pa/tattle/test/Oct31_2022_wtmp"
#define DEFAULT_LOG_OFF "(still logged in)"

#define PRINT_RECORDS_SPACING 2

// 0 = option not given by user, 1 = option given by user
typedef struct
{
    int date;
    int filename;
    int time;
    int logins;
} Options_Given;

// the arguments given by the user
typedef struct
{
    char date[DATE_SIZE];
    char filename[PATHNAME_MAX];
    char time[TIME_SIZE];
    char **logins;
    int logins_count;
} Options;

// contains all relevant information to print out for a user login
typedef struct
{
    char login[UT_NAMESIZE];
    char tty[UT_LINESIZE];
    char log_on[LOG_ON_SIZE];
    char log_off[LOG_OFF_SIZE];
    char from_host[UT_HOSTSIZE];
    int is_pending; // indicates whether a corresponding log off has been found (DEAD_PROCESS or BOOT_TIME)
    time_t log_on_time;
} Login_Record;

// holds a list of Login_Record
typedef struct
{
    Login_Record *records;
    int count;
} Login_Records;

/* used for printing login records
    greatest number of characters found for each of the string attributes in Login_Record
*/
typedef struct
{
    int login_max;
    int tty_max;
    int log_on_max;
    int log_off_max;
    int from_host_max;
} Login_Records_Sizes;

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
 * @brief verify that the list of user-specified logins are real logins on the system
 *
 * @param options address to an Options struct
 * @return int -1 = invalid login found | 0 = logins list ok
 */
extern int verify_logins(Options *options);
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
 * @brief get the output time in local time from a log on or log off in a login record
 *
 * @param log string to hold the the log time
 * @param log_size size of the log string
 * @param tv_sec seconds since the unix epoch (from utmp.ut_tv)
 */
extern void get_output_time(char *log, const size_t log_size, int32_t tv_sec);
/**
 * @brief set all login record's "is_pending" value to false (logged off)
 *
 * @param login_records address to a Login_Records struct
 * @param login_record_info address to a utmp struct (usually of BOOT_TIME ut_type)
 */
extern void log_off_all(Login_Records *login_records, struct utmp *login_record_info);
/**
 * @brief fill a login record with information from a utmp struct
 *
 * @param record address to a Login_Record struct
 * @param login_record_info address to a utmp struct containing information about a login record
 */
extern void fill_record(Login_Record *record, struct utmp *login_record_info);
/**
 * @brief find the corresponding login record from a log off record type
 *
 * @param login_records address to a Login_Records struct containing login records
 * @param login_record_info address to a utmp struct with a ut_type that causes a log off (DEAD_PROCESS OR BOOT_TIME)
 */
extern void find_log_off(Login_Records *login_records, struct utmp *login_record_info);
/**
 * @brief check the ut_type of a login record and run the appropriate actions
 *
 * @param login_records address of a Login_Records struct to contain all login records
 * @param login_record_info address to a utmp struct containing information about the current login record
 * @return int -1 = error occurred | 0 = ok | 1 = login record is invalid
 */
extern int check_record_type(Login_Records *login_records, struct utmp *login_record_info);
/**
 * @brief run the default actions of tattle: find and fill all login records found in a wtmp file
 *
 * @param login_records address of a Login_Records struct to contain all login records
 * @return int 0 = run success | -1 = run failure
 */
extern int fill_login_records_d(Login_Records *login_records);
/**
 * @brief check if a login on a record is within the user-specified logins
 *
 * @param options address to an Options struct
 * @param login login string to check
 * @return int 0 = login match found | 1 = no match found
 */
extern int check_login(Options *options, const char login[]);
/**
 * @brief run the actions of tattle when the user specifies options:
 *        find and fill the appropriate login records found in a wtmp file
 *
 * @param login_records address of a Login_Records struct to contain all login records
 * @param options address of an Options struct containing the options given by the user
 * @param options_given address of an Options_Given struct containing which options the user specified
 * @return int 0 = run success | -1 = run failure
 */
extern int fill_login_records(Login_Records *login_records, Options *options, Options_Given *options_given);
extern int filter_login_records(Login_Records *login_records_ft, Login_Records *login_records, Options *options, Options_Given *options_given);

#endif
