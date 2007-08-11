/*
 * Builtin Name:	lookup.c
 * Module Requirements:	string type ; env type
 * Description:		Environment Type Binding Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name that is found by looking in the given environment.
 */
int sdrl_base_lookup(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	struct sdrl_value *bind;
	struct sdrl_environment *env;
	struct sdrl_type *string_type, *env_type;

	if ((sdrl_value_count(value) != 2))
		ret = SDRL_ERR_INVALID_PARAMS;
	else if (!(string_type = sdrl_find_binding(mach->type_env, "string")) || !(env_type = sdrl_find_binding(mach->type_env, "env")))
		ret = SDRL_ERR_INVALID_TYPE;
	else if ((value->type != env_type && value->type != string_type) || (value->next->type != string_type))
		ret = SDRL_ERR_INVALID_TYPE;
	else {
		if (value->type == string_type) {
			if (!(bind = sdrl_find_binding(mach->env, value->data.str)))
				ret = SDRL_ERR_NOT_FOUND;
			else if (bind->type != env_type)
				ret = SDRL_ERR_INVALID_TYPE;
			else
				env = (struct sdrl_environment *) bind->data.ptr;
		}
		else
			env = (struct sdrl_environment *) value->data.ptr;

		if (!ret) {
			if (!(bind = sdrl_find_binding(env, value->next->data.str)))
				ret = SDRL_ERR_NOT_FOUND;
			else
				mach->ret = sdrl_duplicate_value(mach->heap, bind);
		}
	}
	sdrl_destroy_value(mach->heap, value);
	return(ret);
}


