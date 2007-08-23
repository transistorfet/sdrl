/*
 * Builtin Name:	unlist.c
 * Module Requirements:	list type
 * Description:		List Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<list>
 * Description:	Returns the values in a list.
 */
int sdrl_base_unlist(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;

	if ((sdrl_value_count(value) != 1) || (value->type != sdrl_find_binding(mach->type_env, "list")))
		ret = SDRL_ERR_INVALID_PARAMS;
	else {
		mach->ret = (struct sdrl_value *) value->data.ptr;
		value->data.ptr = NULL;
	}
	return(ret);
}



