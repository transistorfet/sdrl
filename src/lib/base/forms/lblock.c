/*
 * Form Name:		lblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an lblock value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type lblock containing the code passed to it.
 */
int sdrl_base_lblock(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "lblock")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (type->create)
		mach->ret = type->create(mach, type, SDRL_VALUE(expr));
	return(0);
}

