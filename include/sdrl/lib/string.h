/*
 * Library Name:	string.h
 * Description:		String Functions Header
 */


#ifndef _SDRL_LIB_STRING_H
#define _SDRL_LIB_STRING_H

#include <sdrl/sdrl.h>

#define SDRL_STRING_SIZE		1024

int sdrl_load_string(sdMachine *);

/*** Functions ***/
int sdrl_string_char(sdMachine *, sdValue *);
int sdrl_string_length(sdMachine *, sdValue *);
int sdrl_string_substr(sdMachine *, sdValue *);
int sdrl_string_tostring(sdMachine *, sdValue *);

#endif

