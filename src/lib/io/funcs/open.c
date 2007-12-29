/*
 * Builtin Name:	open.c
 * Module Requirements:	stdio.h
 * Description:		Open File
 */

#include <stdio.h>

#include <sdrl/sdrl.h>

/**
 * Args:	<filename>, <permissions>
 * Description:	Opens the file and returns a handle to it.
 */
int sdrl_io_open(struct sdrl_machine *mach, struct sdrl_value *value)
{
	FILE *fptr;
	struct sdrl_type *type;

	if (!value || (value->type != sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "file")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if (type->create && (fptr = (FILE *) type->create(mach, value)))
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) fptr, 0, NULL);
	else
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, NULL));
	return(0);
}


