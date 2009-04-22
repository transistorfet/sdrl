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
int sdrl_base_lexblock(sdMachine *mach, sdArray *args)
{
	sdType *type;
	sdValue *func;

	if (!(type = sdrl_find_binding(mach->type_env, "lexblock")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	// Remove the function value from the args
	func = sdrl_array_shift(args);
	SDRL_DECREF(func);
	if (type->create)
		mach->ret = type->create(mach, type, args);
	return(0);
}

