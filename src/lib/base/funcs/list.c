/*
 * Function Name:	list.c
 * Module Requirements:	list type
 * Description:		List Creation Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value of type list containing the list of parameters.
 */
int sdrl_base_list(sdMachine *mach, sdArray *args)
{
	sdType *type;

	// TODO come back to this when you know more about create
	/*
	if (sdListTypeDef.create)
		mach->ret = sdListTypeDef.create(mach, &sdListTypeDef, args);
	*/
	return(0);
}


