/*
 * Name:	error.h
 * Version:	0.2
 * Description:	Error Generating and Reporting Functions Header File
 */


#ifndef _SDRL_CORE_ERROR_H
#define _SDRL_CORE_ERROR_H

#include <sdrl/globals.h>

struct sdrl_error {
	linenumber_t line;
	int bitflags;
	int err;
	char *msg;
};

struct sdrl_error *sdrl_make_error(int, int, char *);
int sdrl_destroy_error(struct sdrl_error *);
void sdrl_set_linenumber(linenumber_t);
int sdrl_set_error(int, int, char *);
struct sdrl_error *sdrl_last_error(void);
int sdrl_clear_error(void);

#endif
