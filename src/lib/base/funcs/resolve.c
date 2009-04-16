/*
 * Function Name:	resolve.c
 * Module Requirements:	string type
 * Description:		Binding Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name.
 */
int sdrl_base_resolve(sdMachine *mach, sdValue *args)
{
	sdValue *bind;

	if (!args || (args->type->basetype != SDRL_BT_STRING))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	else if (!(bind = sdrl_find_binding(mach->env, SDSTRING(args)->str)))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	// TODO what should the "default" behaviour be?
	mach->ret = sdrl_duplicate_value(mach, bind);
	return(0);
}


