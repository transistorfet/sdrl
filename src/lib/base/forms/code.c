/*
 * Form Name:		code.c
 * Version:		0.1
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an expression value
 */

#include <sdrl/sdrl.h>

/**
 * code(<expr>, ...)
 * Return a value of type expr containing the code passed to it.
 */
int sdrl_base_code(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "expr"), (sdrl_data_t) (void *) expr, 0, NULL);
	return(0);
}

