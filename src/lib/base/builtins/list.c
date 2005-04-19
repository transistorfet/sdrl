/*
 * Builtin Name:	list.c
 * Version:		0.1
 * Module Requirements:	list type
 * Description:		List Creation Expression
 */

#include <sdrl.h>

/**
 * list(<value>, ...)
 * Returns a value of type list containing the list of parameters.
 */
int sdrl_base_list(struct sdrl_machine *mach, struct sdrl_value *value)
{
	mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "list"), (sdrl_data_t) (void *) value, 0, NULL);
	return(0);
}



