/*
 * Name:	scheme.c
 * Version:	0.1
 * Description:	Scheme Functions
 */

#include <stdio.h>

#include <sdrl.h>
#include "scheme.h"

#define traverse_value_m(type_name, value, init, line)					\
	if (!(type = sdrl_find_type(mach->type_env, type_name)))			\
		ret = ERR_NOT_FOUND;							\
	else {										\
		result = init;								\
		cur = value;								\
		while (cur) {								\
			if (cur->type != type) {					\
				ret = ERR_INVALID_TYPE;					\
				break;							\
			}								\
			line;								\
			cur = cur->next;						\
		}									\
		mach->ret = sdrl_make_value(type, (sdrl_data_t) result, 0, NULL);	\
	}

/**
 * Add all the functions to the environment of machine
 */
int scheme_initialize(struct sdrl_machine *mach)
{
	struct sdrl_type *form, *builtin;
	form = sdrl_find_type(mach->type_env, "form");
	builtin = sdrl_find_type(mach->type_env, "builtin");

	sdrl_bind_value(mach->env, "define", sdrl_make_value(form, (sdrl_data_t) (void *) scheme_define, 0, NULL));

	return(0);
}

/**
 * define(<name, <expr>)
 * Define a binding between a name and expr in the current enviornment.
 */
int scheme_define(struct sdrl_machine *mach, struct sdrl_value *value)
{
	return(0);
}


