/*
 * Name:	error.h
 * Description:	Error Generating and Reporting Functions Header File
 */


#ifndef _SDRL_CORE_ERROR_H
#define _SDRL_CORE_ERROR_H

#include <sdrl/globals.h>

#define SDRL_EBF_STATIC		0x0001

#define SDRL_ES_FATAL		1
#define SDRL_ES_HIGH		2
#define SDRL_ES_MEDIUM		3
#define SDRL_ES_LOW		4
#define SDRL_ES_WARNING		5

struct sdrl_error {
	linenumber_t line;
	short bitflags;
	short severity;
	int err;
	const char *msg;
};

struct sdrl_error *sdrl_make_error(linenumber_t, short, int, const char *);
int sdrl_destroy_error(struct sdrl_error *);
struct sdrl_error *sdrl_duplicate_error(struct sdrl_error *);

#endif

