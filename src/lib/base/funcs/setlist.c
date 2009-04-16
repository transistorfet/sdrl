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
int sdrl_base_setlist(sdMachine *mach, sdValue *args)
{
	sdValue *cur_name, *cur_value, *tmp;

	if ((sdrl_value_count(args) < 2) || (args->type->basetype != SDRL_BT_REFERENCE))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	cur_name = SDREFERENCE(args)->ref;
	cur_value = args->next;
	args->next = NULL;
	while (cur_name) {
		if (cur_name->type->basetype != SDRL_BT_STRING)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		tmp = cur_value->next;
		cur_value->next = NULL;
		if (sdrl_replace_binding(mach->env, SDSTRING(cur_name)->str, cur_value)) {
			if (sdrl_add_binding(mach->env, SDSTRING(cur_name)->str, cur_value))
				return(SDRL_ERROR(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
		}
		cur_name = cur_name->next;
		cur_value = tmp;
	}
	//mach->ret = SDRL_MAKE_REFERENCE(args->next);
	return(0);
}


