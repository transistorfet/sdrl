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
int sdrl_base_resolve(sdMachine *mach, sdArray *args)
{
	sdValue *bind;

	if (args->last < 1 || (args->items[1]->type->basetype != SDRL_BT_STRING))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	else if (!(bind = sdrl_env_find(mach->env, SDSTRING(args->items[1])->str)))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	// TODO what should the "default" behaviour be?
	mach->ret = sdrl_duplicate_value(mach, bind);
	return(0);
}


