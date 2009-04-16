/*
 * Function Name:	null.c
 * Module Requirements:	number type
 * Description:		Null Check Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the number -1 if value passed is null; the number 0 if otherwise.
 */
int sdrl_base_null(sdMachine *mach, sdValue *args)
{
	sdType *type;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (!args)
		mach->ret = sdrl_make_number(mach->heap, type, -1);
	else
		mach->ret = sdrl_make_number(mach->heap, type, 0);
	return(0);
}


