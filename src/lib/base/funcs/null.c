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
	// TODO this is really really weird, maybe you should make a nil/undef type?
	if (args->last < 1)
		mach->ret = sdrl_make_number(mach->heap, &sdNumberTypeDef, -1);
	else
		mach->ret = sdrl_make_number(mach->heap, &sdNumberTypeDef, 0);
	return(0);
}


