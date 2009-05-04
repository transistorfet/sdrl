/*
 * Function Name:	deref.c
 * Module Requirements:	reference type
 * Description:		Dereference a Reference Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<reference>
 * Description:	Returns the value pointed to by the given reference.
 */
int sdrl_base_deref(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	if (!sdrl_value_isa(args->items[1], &sdReferenceTypeDef))
		return(sdrl_set_type_error(mach, &sdReferenceTypeDef, args->items[1]->type));
	mach->ret = SDRL_INCREF(SDREFERENCE(args->items[1])->ref);
	return(0);
}


