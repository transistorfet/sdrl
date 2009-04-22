/*
 * Function Name:	head.c
 * Module Requirements:	list type
 * Description:		Head Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the first value passed to it.
 */
int sdrl_base_head(sdMachine *mach, sdArray *args)
{
	// TODO will lists be arrays or a new linked-list type??
	if (args->last > 0)
		mach->ret = sdrl_duplicate_value(mach, args->items[1]);
	return(0);
}


