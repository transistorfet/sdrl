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
int sdrl_base_code(sdMachine *mach, sdExpr *expr)
{
	sdType *type;

	if (!(type = sdrl_find_binding(mach->type_env, "expr")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!type->create)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	mach->ret = type->create(mach, type, SDVALUE(expr));
	return(0);
}

