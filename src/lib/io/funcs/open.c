/*
 * Function Name:	open.c
 * Module Requirements:	stdio.h
 * Description:		Open File
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>

/**
 * Args:	<filename>, <permissions>
 * Description:	Opens the file and returns a handle to it.
 */
int sdrl_io_open(sdMachine *mach, sdArray *args)
{
	sdValue *tmp;

	if (args->last != 1 || !sdrl_value_isa(args->items[1], &sdStringTypeDef))
		return(sdrl_set_args_error(mach));
	else if (sdFileTypeDef.create) {
		tmp = sdrl_array_shift(args);
		SDRL_DECREF(tmp);
		mach->ret = sdFileTypeDef.create(mach, &sdFileTypeDef, args);
	}
	else
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	return(0);
}


