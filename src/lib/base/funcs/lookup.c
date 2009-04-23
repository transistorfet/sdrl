/*
 * Function Name:	lookup.c
 * Module Requirements:	string type ; env type
 * Description:		Environment Type Binding Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name that is found by looking in the given environment.
 */
int sdrl_base_lookup(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdValue *bind;
	sdType *string_type, *env_type;

	if (args->last != 2)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (!(string_type = sdrl_env_find(mach->type_env, "string")) || !(env_type = sdrl_env_find(mach->type_env, "env")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if ((args->items[1]->type != env_type && args->items[1]->type != string_type) || (args->items[2]->type != string_type))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	else {
		if (args->items[1]->type == string_type) {
			if (!(env = sdrl_env_find(mach->env, SDSTRING(args->items[1])->str)))
				return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
			else if (SDVALUE(env)->type != env_type)
				return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		}
		else
			env = SDENV(args->items[1]);

		if (!(bind = sdrl_env_find(env, SDSTRING(args->items[2])->str)))
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
		else
			mach->ret = sdrl_duplicate_value(mach, bind);
	}
	return(0);
}


