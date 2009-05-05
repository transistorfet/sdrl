/*
 * Function Name:	getenv.c
 * Module Requirements:	string type ; env type
 * Description:		Environment Type Binding Resolution Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<env>, <name>
 * Description:	Returns a copy of the value bound to the given name that is
 *		found by looking in the given environment.
 */
int sdrl_base_getenv(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdValue *bind;

	if (args->last != 2)
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

	SDRL_TRY(bind = sdrl_find(mach, env, SDSTRING(args->items[2])->str));
	// TODO should this duplicate, incref, or use a type bitflag to base it's action on?
	mach->ret = sdrl_duplicate_value(mach, bind);
	return(0);

    FAIL:
	return(mach->error->err);
}


