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
int sdrl_base_lookup(struct sdrl_machine *mach, struct sdrl_value *args)
{
	struct sdrl_value *bind;
	struct sdrl_environment *env;
	struct sdrl_type *string_type, *env_type;

	if ((sdrl_value_count(args) != 2))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (!(string_type = sdrl_find_binding(mach->type_env, "string")) || !(env_type = sdrl_find_binding(mach->type_env, "env")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if ((args->type != env_type && args->type != string_type) || (args->next->type != string_type))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	else {
		if (args->type == string_type) {
			if (!(env = sdrl_find_binding(mach->env, SDRL_STRING(args)->str)))
				return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
			else if (bind->type != env_type)
				return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		}
		else
			env = SDRL_ENVIRONMENT(args);

		if (!(bind = sdrl_find_binding(env, SDRL_STRING(args->next)->str)))
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
		else
			mach->ret = sdrl_duplicate_value(mach, bind);
	}
	return(0);
}


