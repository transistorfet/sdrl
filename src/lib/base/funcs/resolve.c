/*
 * Function Name:	resolve.c
 * Module Requirements:	string type
 * Description:		Binding Resolution Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name.
 */
int sdrl_base_resolve(sdMachine *mach, sdArray *args)
{
	sdValue *bind;

	if (args->last < 1 || !sdrl_value_isa(args->items[1], &sdStringTypeDef))
		return(sdrl_set_type_error(mach));
	else if (!(bind = sdrl_env_find(mach->env, SDSTRING(args->items[1])->str)))
		return(sdrl_set_not_found_error(mach));
	// TODO what should the "default" behaviour be?
	mach->ret = sdrl_duplicate_value(mach, bind);
	return(0);
}


