/*
 * Name:	aspects.c
 * Version:	0.1
 * Description:	Aspects
 */

#include <stdio.h>

#include <sdrl.h>
#include "aspects.h"

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
int aspects_initialize(struct sdrl_machine *mach)
{
	struct sdrl_type *form, *builtin;
	form = sdrl_find_type(mach->type_env, "form");
	builtin = sdrl_find_type(mach->type_env, "builtin");

//	sdrl_bind_value(mach->env, "code", sdrl_make_value(form, (sdrl_data_t) (void *) prim_code, 0, NULL));

	return(0);
}

/**
 * aspect(<name, <pointcut>, <expr>)
 * Return a value of type expr containing the code passed to it.
 */
int aspects_aspect(struct sdrl_machine *mach, struct sdrl_value *value)
{
	return(0);
}

/**
 * code(<expr>, ...)
 * Return a value of type expr containing the code passed to it.
 */
//int prim_code(struct sdrl_machine *mach, struct sdrl_expr *expr)
//{
//	mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "expr"), (sdrl_data_t) (void *) expr, 0, NULL);
//	return(0);
//}


