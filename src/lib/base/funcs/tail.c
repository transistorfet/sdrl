/*
 * Function Name:	tail.c
 * Module Requirements:	list type
 * Description:		Tail Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the all of the values passed to it following the first value.
 */
int sdrl_base_tail(sdMachine *mach, sdValue *args)
{
	if (args && args->next)
		mach->ret = SDRL_INCREF(args->next);
	else
		mach->ret = NULL;
	return(0);
}


