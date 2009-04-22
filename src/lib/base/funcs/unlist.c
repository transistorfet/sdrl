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
int sdrl_base_unlist(sdMachine *mach, sdArray *args)
{
	// TODO deal with this when you know about lists (if you are even going to need this anymore)
	/*
	if (!args || args->next || (args->type->basetype != SDRL_BT_REFERENCE) || !SDREFERENCE(args)->ref->type)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	mach->ret = SDRL_INCREF(SDREFERENCE(args)->ref);
	*/
	return(0);
}


