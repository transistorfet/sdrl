/*
 * Function Name:	unlist.c
 * Module Requirements:	list type
 * Description:		List Resolution Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<list>
 * Description:	Returns the values in a list.
 */
int sdrl_base_unlist(sdMachine *mach, sdValue *args)
{
	if (!args || args->next || (args->type->basetype != SDRL_BT_REFERENCE) || !SDREFERENCE(args)->ref->type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	mach->ret = SDRL_MAKE_REFERENCE(SDREFERENCE(args)->ref);
	return(0);
}


