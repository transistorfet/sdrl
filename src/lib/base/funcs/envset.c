/*
 * Function Name:	envset.c
 * Module Requirements:	string type ; env type
 * Description:		Environment Type Assignment Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<env>, <name>, <value>
 * Description:	Binds the value to the given name in the environment.  The
 *		environment can be passed as a string or env type.
 */
int sdrl_base_envset(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdValue *value;

	if (args->last != 3)
		return(sdrl_set_args_error(mach));
	SDRL_TRY(sdrl_check_type(mach, args->items[2], &sdStringTypeDef));
	if (sdrl_value_isa(args->items[1], &sdStringTypeDef)) {
		SDRL_TRY(env = sdrl_find(mach, mach->env, SDSTRING(args->items[1])->str));
		SDRL_TRY(sdrl_check_type(mach, SDVALUE(env), &sdEnvTypeDef));
	}
	else if (sdrl_value_isa(args->items[1], &sdEnvTypeDef))
		env = SDENV(args->items[1]);
	else
		return(sdrl_set_type_error(mach, &sdStringTypeDef, args->items[1]->type));

	value = SDRL_INCREF(args->items[3]);
	if (sdrl_env_replace(env, SDSTRING(args->items[2])->str, value)) {
		if (sdrl_env_add(env, SDSTRING(args->items[2])->str, value))
			return(sdrl_set_memory_error(mach));
	}
	mach->ret = SDRL_INCREF(value);
	return(0);

    FAIL:
	return(mach->error->err);
}


