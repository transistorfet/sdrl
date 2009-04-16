/*
 * Function Name:	set.c
 * Module Requirements:	string type ; list type
 * Description:		Binding Assignment Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>, <value>
 * Description:	Bind value to name.  Returns a duplicate of value.
 */
int sdrl_base_set(sdMachine *mach, sdValue *args)
{
	sdValue *value;

	if (sdrl_value_count(args) < 2)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_STRING)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	// TODO should this instead clobber the args stripping off the value to be assigned?
	value = SDRL_INCREF(args->next);
	if (sdrl_replace_binding(mach->env, SDSTRING(args)->str, value)) {
		if (sdrl_add_binding(mach->env, SDSTRING(args)->str, value))
			return(SDRL_ERROR(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	}
	mach->ret = SDRL_INCREF(value);
	return(0);
}


