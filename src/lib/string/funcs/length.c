/*
 * Function Name:	length.c
 * Module Requirements:	string type
 * Description:		String Length Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>
 * Description:	Returns the length of the given string.
 */
int sdrl_string_length(sdMachine *mach, sdArray *args)
{
	sdType *num_type;

	if (args->last != 1 || args->items[1]->type->basetype != SDRL_BT_STRING)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(num_type = sdrl_find_binding(mach->type_env, "number")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	mach->ret = sdrl_make_number(mach->heap, num_type, (number_t) SDSTRING(args->items[1])->len);
	return(0);
}



