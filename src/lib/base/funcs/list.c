/*
 * Function Name:	list.c
 * Module Requirements:	list type
 * Description:		List Creation Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value of type list containing the list of parameters.
 */
int sdrl_base_list(sdMachine *mach, sdArray *args)
{
	sdType *type;

	// TODO come back to this when you know more about create
	/*
	if (!(type = sdrl_find_binding(mach->type_env, "list")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (type->create)
		mach->ret = type->create(mach, type, args);
	*/
	return(0);
}


