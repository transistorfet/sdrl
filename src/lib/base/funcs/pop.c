/*
 * Function Name:	pop.c
 * Module Requirements:	array type
 * Description:		Pop Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<array>
 * Description:	Removes the last value in the given array and returns it.
 */
int sdrl_base_pop(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdArrayTypeDef))
		return(sdrl_set_type_error(mach));
	mach->ret = sdrl_array_pop(SDARRAY(args->items[1]));
	return(0);
}


