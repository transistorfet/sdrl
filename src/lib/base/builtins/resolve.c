/*
 * Builtin Name:	resolve.c
 * Module Requirements:	string type
 * Description:		Binding Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>
 * Description:	Returns a copy of the value bound to the given name.
 */
int sdrl_base_resolve(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *bind;

	if (!value || (value->type != sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERR_INVALID_TYPE);
	else if (!(bind = sdrl_find_binding(mach->env, value->data.str)))
		return(SDRL_ERR_NOT_FOUND);
	mach->ret = sdrl_duplicate_value(mach->heap, bind);
	return(0);
}


