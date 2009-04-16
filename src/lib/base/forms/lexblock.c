/*
 * Form Name:		lexblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an lexblock value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type lexblock containing the code passed to it.
 */
int sdrl_base_lexblock(sdMachine *mach, sdExpr *expr)
{
	sdType *type;

	if (!(type = sdrl_find_binding(mach->type_env, "lexblock")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (type->create)
		mach->ret = type->create(mach, type, SDVALUE(expr));
	return(0);
}

