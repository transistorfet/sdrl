/*
 * Function Name:	new.c
 * Module Requirements:	string type
 * Description:		Create New Value from Type
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns a value pointing to a newly created environment.
 */
int sdrl_base_new(sdMachine *mach, sdArray *args)
{
	sdType *type;
	sdValue *tmp;
	const char *typename;

	if (args->last < 1)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	SDRL_TRY(typename = sdrl_check_string(mach, args->items[1], SDRL_BT_STRING, NULL));
	SDRL_TRY(type = sdrl_find(mach, mach->type_env, typename));
	if (!type->create)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, "Type has no create function"));
	// Remove the function and typename values from the args
	tmp = sdrl_array_shift(args);
	SDRL_DECREF(tmp);
	tmp = sdrl_array_shift(args);
	SDRL_DECREF(tmp);
	mach->ret = type->create(mach, type, args);
	return(0);

    FAIL:
	return(mach->error->err);
}


