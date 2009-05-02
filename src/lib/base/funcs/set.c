/*
 * Function Name:	set.c
 * Module Requirements:	string type ; list type
 * Description:		Binding Assignment Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<name>, <value>
 * Description:	Bind value to name.  Returns a duplicate of value.
 */
int sdrl_base_set(sdMachine *mach, sdArray *args)
{
	sdValue *value;

	if (args->last != 2)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdStringTypeDef))
		return(sdrl_set_type_error(mach, &sdStringTypeDef, args->items[1]->type));
	// TODO should this instead clobber the args stripping off the value to be assigned?
	value = SDRL_INCREF(args->items[2]);
	if (sdrl_env_replace(mach->env, SDSTRING(args->items[1])->str, value)) {
		if (sdrl_env_add(mach->env, SDSTRING(args->items[1])->str, value))
			return(sdrl_set_memory_error(mach));
	}
	mach->ret = SDRL_INCREF(value);
	return(0);
}


