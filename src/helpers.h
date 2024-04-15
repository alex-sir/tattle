/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef HELPERS
#define HELPERS

// gets rid of some weird warnings with library functions (strftime())
#define _GNU_SOURCE

#include <stdio.h>  // for printf(), fprintf()
#include <string.h> // for strerror(), strlen()
#include <errno.h>  // for errno
#include <time.h>   // for strptime(), mktime()

#include "options.h"

/**
 * @brief print an error message
 */
extern void print_err(void);
/**
 * @brief print a message informing about the use of the program
 *
 * @param progname name of the running program
 */
extern void usage(const char *progname);
/**
 * @brief free all memory for logins in an Options struct
 *
 * @param options address of an Options struct
 */
extern void free_logins(Options *options);
/**
 * @brief check if a Login_Records struct needs to have its dynamic memory reallocated
 *
 * @param login_records address to a Login_Records struct
 * @return int -1 = memory reallocation error | 0 = ok
 */
extern int login_records_mem(Login_Records *login_records);
/**
 * @brief find the max size strings for all strings in all the login records in a Login_Records struct
 *
 * @param login_records_sizes address to a Login_Records_Sizes struct where the max sizes will be stored
 * @param login_records address to a Login_Records struct containing the login records
 */
extern void find_max_sizes(Login_Records_Sizes *login_records_sizes, Login_Records *login_records);
/**
 * @brief print all login records found in login_records
 *
 * @param login_records address of a Login_Records struct
 */
extern void print_records(Login_Records *login_records);
/**
 * @brief ONLY FOR USE WITH qsort() - compare the log on times of two login records (sorted in increasing order)
 *
 * @param login_record1 first login record - compared with login_record2
 * @param login_record2 second login record - compared with login_record1
 * @return int 1 = login_record1 time > login_record2 time |
 *            -1 = login_record1 time > login_record2 time |
 *            0 = login_record1 time == login_record2 time
 */
extern int compare_log_on_times(const void *login_record1, const void *login_record2);

#endif
