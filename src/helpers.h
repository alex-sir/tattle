/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for printf(), fprintf()
#include <string.h> // for strerror(), strlen()
#include <errno.h>  // for errno

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
 * @brief free all memory related to a Login_Records struct
 *
 * @param login_records address of a Login_Records struct
 */
extern void free_login_records(Login_Records *login_records);
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

#endif
