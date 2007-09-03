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
	int ret = 0;
	struct sdrl_type *type;

	if (!value || (value->type != sdrl_find_binding(mach->type_env, "string")))
		ret = SDRL_ERR_INVALID_PARAMS;
	if (!(type = sdrl_find_binding(mach->type_env, "file")))
		ret = SDRL_ERR_NOT_FOUND;
	else if (type->create && (fptr = (FILE *) type->create(mach, value))) {
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) fptr, 0, NULL);
		return(0);
	}
	else
		ret = -1;
	sdrl_destroy_value(mach->heap, value);
	return(ret);
}


