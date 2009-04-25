/*
 * Function Name:	lookup.c
 * Module Requirements:	string type ; env type
 * Description:		Environment Type Binding Resolution Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name that is found by looking in the given environment.
 */
int sdrl_base_lookup(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdValue *bind;

	if (args->last != 2)
		return(sdrl_set_args_error(mach));
	SDRL_TRY(sdrl_check_type(mach, args->items[2], &sdStringTypeDef));
	if (sdrl_check_type(mach, args->items[1], &sdStringTypeDef)) {
		if (!(env = sdrl_env_find(mach->env, SDSTRING(args->items[1])->str)))
			return(sdrl_set_not_found_error(mach));
		SDRL_TRY(sdrl_check_type(mach, SDVALUE(env), &sdEnvTypeDef));
	}
	else if (sdrl_check_type(mach, args->items[1], &sdEnvTypeDef))
		env = SDENV(args->items[1]);
	else
		return(sdrl_set_type_error(mach));

	if (!(bind = sdrl_env_find(env, SDSTRING(args->items[2])->str)))
		return(sdrl_set_not_found_error(mach));
	else
		mach->ret = sdrl_duplicate_value(mach, bind);
	return(0);

    FAIL:
	return(mach->error->err);
}


