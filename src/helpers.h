/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  tattle
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>  // for printf(), fprintf()
#include <string.h> // for strerror()
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

#endif
