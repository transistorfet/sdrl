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
int sdrl_base_head(struct sdrl_machine *mach, struct sdrl_value *args)
{
	if (args)
		mach->ret = sdrl_duplicate_single_value(mach, args);
	return(0);
}


