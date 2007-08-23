/*
 * Builtin Name:	list.c
 * Module Requirements:	list type
 * Description:		List Creation Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value of type list containing the list of parameters.
 */
int sdrl_base_list(struct sdrl_machine *mach, struct sdrl_value *value)
{
	mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "list"), (sdrl_data_t) (void *) SDRL_MAKE_REFERENCE(value), 0, NULL);
	return(0);
}



