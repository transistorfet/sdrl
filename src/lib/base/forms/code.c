/*
 * Form Name:		code.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an expression value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type expr containing the code passed to it.
 */
int sdrl_base_code(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "expr")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!type->create)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	mach->ret = type->create(mach, type, SDRL_VALUE(expr));
	return(0);
}

