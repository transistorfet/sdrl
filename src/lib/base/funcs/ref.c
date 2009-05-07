/*
 * Function Name:	ref.c
 * Module Requirements:	reference type
 * Description:		Create Reference Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>
 * Description:	Returns a reference to the given value.
 */
int sdrl_base_ref(sdMachine *mach, sdArray *args)
{
	if (args->last != 1)
		return(sdrl_set_args_error(mach));
	mach->ret = SDVALUE(sdrl_make_reference(mach, &sdReferenceTypeDef, args->items[1]));
	return(0);
}


