/*
 * Function Name:	open.c
 * Module Requirements:	stdio.h
 * Description:		Open File
 */

#include <stdio.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<filename>, <permissions>
 * Description:	Opens the file and returns a handle to it.
 */
int sdrl_io_open(sdMachine *mach, sdArray *args)
{
	sdType *type;
	sdValue *tmp;

	if (args->last != 1 || (args->items[1]->type->basetype != SDRL_BT_STRING))
		return(sdrl_set_args_error(mach));
	if (!(type = sdrl_env_find(mach->type_env, "file")))
		return(sdrl_set_not_found_error(mach));
	else if (type->create) {
		tmp = sdrl_array_shift(args);
		SDRL_DECREF(tmp);
		mach->ret = type->create(mach, type, args);
	}
	else
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	return(0);
}


