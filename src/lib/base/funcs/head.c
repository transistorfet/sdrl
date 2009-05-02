/*
 * Function Name:	head.c
 * Module Requirements:	list type
 * Description:		Head Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns the first value passed to it.
 */
int sdrl_base_head(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdListTypeDef))
		return(sdrl_set_type_error(mach, &sdListTypeDef, args->items[1]->type));
	mach->ret = SDRL_INCREF(SDLIST(args->items[1])->item);
	return(0);
}


