/*
 * Library Name:	string.h
 * Description:		String Functions Header
 */


#ifndef _SDRL_LIB_STRING_H
#define _SDRL_LIB_STRING_H

#include <sdrl/sdrl.h>

#define STRING_SIZE		1024

int sdrl_load_string(struct sdrl_machine *);

/*** Functions ***/
int sdrl_string_char(struct sdrl_machine *, struct sdrl_value *);
int sdrl_string_length(struct sdrl_machine *, struct sdrl_value *);
int sdrl_string_substr(struct sdrl_machine *, struct sdrl_value *);
int sdrl_string_tostring(struct sdrl_machine *, struct sdrl_value *);

#endif

