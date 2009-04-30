/*
 * Function Name:	shift.c
 * Module Requirements:	array type
 * Description:		Shift Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<array>
 * Description:	Removes the first value in the given array and returns it.
 */
int sdrl_base_shift(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdArrayTypeDef))
		return(sdrl_set_type_error(mach));
	mach->ret = sdrl_array_shift(SDARRAY(args->items[1]));
	return(0);
}


