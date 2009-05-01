/*
 * Form Name:		dynblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as a dynblock value
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type dynblock containing the code passed to it.
 */
int sdrl_base_dynblock(sdMachine *mach, sdArray *args)
{
	mach->ret = SDVALUE(sdrl_make_reference(mach->heap, &sdDynBlockTypeDef, args->items[1]));
	return(0);
}

