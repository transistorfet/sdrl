/*
 * Function Name:	length.c
 * Module Requirements:	string type
 * Description:		String Length Expression
 */

#include <string.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/string.h>

/**
 * Args:	<value>
 * Description:	Returns the length of the given string.
 */
int sdrl_string_length(sdMachine *mach, sdArray *args)
{
	if (args->last != 1 || !sdrl_value_isa(args->items[1], &sdStringTypeDef))
		return(sdrl_set_args_error(mach));
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, (number_t) SDSTRING(args->items[1])->len));
	return(0);
}



