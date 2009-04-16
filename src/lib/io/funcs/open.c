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
int sdrl_io_open(sdMachine *mach, sdValue *args)
{
	sdType *type;

	if (!args || (args->type->basetype != SDRL_BT_STRING))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "file")))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if (type->create)
		mach->ret = type->create(mach, type, args);
	else
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	return(0);
}


