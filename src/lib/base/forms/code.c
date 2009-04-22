/*
 * Form Name:		code.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as an expression value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type expr containing the code passed to it.
 */
int sdrl_base_code(sdMachine *mach, sdArray *args)
{
	if (args->last > 0)
		mach->ret = SDRL_INCREF(args->items[1]);
	return(0);
}

