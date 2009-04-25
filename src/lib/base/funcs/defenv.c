/*
 * Function Name:	defenv.c
 * Module Requirements:	list type
 * Description:		Define Environment Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value pointing to a newly created environment.
 */
int sdrl_base_defenv(sdMachine *mach, sdArray *args)
{
	sdValue *value;

	if (!sdEnvTypeDef.create)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	value = sdrl_array_shift(args);
	SDRL_DECREF(value);
	mach->ret = sdEnvTypeDef.create(mach, &sdEnvTypeDef, args);
	return(0);
}


