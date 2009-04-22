/*
 * Function Name:	defenv.c
 * Module Requirements:	list type
 * Description:		Define Environment Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value pointing to a newly created environment.
 */
int sdrl_base_defenv(sdMachine *mach, sdArray *args)
{
	sdType *type;

	// TODO fix this up when you know more about the create function
/*
	if (!(type = sdrl_find_binding(mach->type_env, "env")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if (!type->create)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	mach->ret = type->create(mach, type, args);
*/
	return(0);
}


