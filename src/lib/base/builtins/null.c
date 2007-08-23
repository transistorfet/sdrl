/*
 * Builtin Name:	null.c
 * Module Requirements:	number type
 * Description:		Null Check Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the number -1 if value passed is null; the number 0 if otherwise.
 */
int sdrl_base_null(struct sdrl_machine *mach, struct sdrl_value *value)
{
	if (!value)
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "number"), (sdrl_data_t) (number_t) -1, 0, NULL);
	else
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "number"), (sdrl_data_t) (number_t) 0, 0, NULL);
	return(0);
}




