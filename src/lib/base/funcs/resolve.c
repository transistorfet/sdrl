/*
 * Builtin Name:	resolve.c
 * Version:		0.1
 * Module Requirements:	string type
 * Description:		Binding Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * $(<name>)
 * Returns a copy of the value bound to the given name.
 */
int sdrl_base_resolve(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	struct sdrl_value *bind;

	if (!value || (value->type != sdrl_find_binding(mach->type_env, "string")))
		ret = ERR_INVALID_TYPE;
	else if (!(bind = sdrl_find_binding(mach->env, value->data.str)))
		ret = ERR_NOT_FOUND;
	else
		mach->ret = sdrl_duplicate_value(mach->heap, bind);
	sdrl_destroy_value(mach->heap, value);
	return(ret);
}


