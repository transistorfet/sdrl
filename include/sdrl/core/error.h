/*
 * Name:	error.h
 * Description:	Error Generating and Reporting Functions Header File
 */


#ifndef _SDRL_CORE_ERROR_H
#define _SDRL_CORE_ERROR_H

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_EBF_STATIC		0x0001

#define SDRL_ES_FATAL		1
#define SDRL_ES_HIGH		2
#define SDRL_ES_MEDIUM		3
#define SDRL_ES_LOW		4
#define SDRL_ES_WARNING		5

extern sdType sdErrorTypeDef;

struct sdError {
	sdValue value;
	linenumber_t line;
	short bitflags;
	short severity;
	int err;
	const char *msg;
};

extern sdError sdMemoryError;

sdError *sdrl_make_error(sdHeap *, sdType *, linenumber_t, short, int, const char *);
int sdrl_error_destroy(sdError *);
sdError *sdrl_error_duplicate(sdHeap *, sdError *);

#endif

