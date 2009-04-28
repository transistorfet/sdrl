/*
 * Function Name:	tail.c
 * Module Requirements:	list type
 * Description:		Tail Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the all of the values passed to it following the first value.
 */
int sdrl_base_tail(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdListTypeDef))
		return(sdrl_set_type_error(mach));
	mach->ret = SDVALUE(SDRL_INCREF(SDLIST(args->items[1])->next));
	return(0);
}


