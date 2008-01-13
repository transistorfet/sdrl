/*
 * Form Name:		dblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as a dblock value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type dblock containing the code passed to it.
 */
int sdrl_base_dblock(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	mach->ret = sdrl_make_reference(mach->heap, sdrl_find_binding(mach->type_env, "dblock"), SDRL_VALUE(expr));
	return(0);
}

