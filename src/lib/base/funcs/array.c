/*
 * Function Name:	array.c
 * Module Requirements:	(none)
 * Description:		Array Creation Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the values passed to it (as a single list).
 */
int sdrl_base_array(sdMachine *mach, sdArray *args)
{
	sdValue *func;

	// Remove the function from the arguments
	func = sdrl_array_shift(args);
	SDRL_DECREF(func);
	mach->ret = SDVALUE(SDRL_INCREF(args));
	return(0);
}


