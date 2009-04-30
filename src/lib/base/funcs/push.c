/*
 * Function Name:	push.c
 * Module Requirements:	array type
 * Description:		Push Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<array>, <value>, ...
 * Description:	Adds the given values to the end of the given array.
 */
int sdrl_base_push(sdMachine *mach, sdArray *args)
{
	int i;

	if (args->last < 2)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdArrayTypeDef))
		return(sdrl_set_type_error(mach));
	for (i = 2; i <= args->last; i++) {
		if (sdrl_array_push(SDARRAY(args->items[1]), args->items[i])) {
			sdrl_set_memory_error(mach);
			return(-1);
		}
	}
	return(0);
}


