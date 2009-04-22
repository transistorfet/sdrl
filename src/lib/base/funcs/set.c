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
int sdrl_base_set(sdMachine *mach, sdArray *args)
{
	sdValue *value;

	if (args->last < 2)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->items[1]->type->basetype != SDRL_BT_STRING)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	// TODO should this instead clobber the args stripping off the value to be assigned?
	value = SDRL_INCREF(args->items[2]);
	if (sdrl_replace_binding(mach->env, SDSTRING(args->items[1])->str, value)) {
		if (sdrl_add_binding(mach->env, SDSTRING(args->items[1])->str, value))
			return(sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	}
	mach->ret = SDRL_INCREF(value);
	return(0);
}


