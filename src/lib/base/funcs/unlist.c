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
int sdrl_base_unlist(struct sdrl_machine *mach, struct sdrl_value *args)
{
	// TODO you need to know what the type name will be
	if ((sdrl_value_count(args) != 1) || (args->type != sdrl_find_binding(mach->type_env, "list")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	mach->ret = SDRL_MAKE_REFERENCE(SDRL_REFERENCE(args)->ref);
	return(0);
}


