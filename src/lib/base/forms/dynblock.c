/*
 * Form Name:		dynblock.c
 * Module Requirements:	expr type
 * Description:		A form that returns its expression parameter as a dynblock value
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<expr>, ...
 * Description:	Return a value of type dynblock containing the code passed to it.
 */
int sdrl_base_dynblock(sdMachine *mach, sdArray *args)
{
	mach->ret = sdrl_make_reference(mach->heap, sdrl_env_find(mach->type_env, "dynblock"), args->items[1]);
	return(0);
}

