/*
 * Function Name:	setlist.c
 * Module Requirements:	string type ; list type
 * Description:		Binding Assignment Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<name>, <value>
 * Description:	Bind value to name.  Returns a duplicate of value.
 */
int sdrl_base_setlist(sdMachine *mach, sdArray *args)
{
	int i;
	sdArray *labels, *values;

	if (args->last != 2 || (args->items[1]->type->basetype != SDRL_BT_ARRAY)
	    || (args->items[2]->type->basetype != SDRL_BT_ARRAY))
		return(sdrl_set_args_error(mach));
	labels = SDARRAY(args->items[1]);
	values = SDARRAY(args->items[2]);
	for (i = 0; i <= labels->last && i <= values->last; i++) {
		if (labels->items[i]->type->basetype != SDRL_BT_STRING)
			return(sdrl_set_type_error(mach));
		SDRL_INCREF(values->items[i]);
		if (sdrl_env_replace(mach->env, SDSTRING(labels->items[i])->str, values->items[i])) {
			if (sdrl_env_add(mach->env, SDSTRING(labels->items[i])->str, values->items[i]))
				return(sdrl_set_memory_error(mach));
		}
	}
	//mach->ret = SDRL_INCREF(args->items[2]);
	return(0);
}


