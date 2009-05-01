/*
 * Function Name:	null.c
 * Module Requirements:	number type
 * Description:		Null Check Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the number -1 if value passed is null; the number 0 if otherwise.
 */
int sdrl_base_null(sdMachine *mach, sdArray *args)
{
	if (args->last < 1 || sdrl_value_isa(args->items[1], &sdUndefTypeDef))
		mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, -1));
	else
		mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, 0));
	return(0);
}


