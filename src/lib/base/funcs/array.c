/*
 * Function Name:	array.c
 * Module Requirements:	(none)
 * Description:		Array Creation Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the values passed to it (as a single list).
 */
int sdrl_base_array(sdMachine *mach, sdValue *args)
{
	mach->ret = SDRL_INCREF(args);
	return(0);
}


