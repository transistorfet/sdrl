/*
 * Builtin Name:	file.c
 * Module Requirements:	stdio.h
 * Description:		File Descriptor Type
 */

#include <stdio.h>

#include <sdrl/sdrl.h>

static struct sdrl_type *file_type;

static void *sdrl_create_file_type(struct sdrl_machine *, struct sdrl_value *);
static int sdrl_destroy_file_type(struct sdrl_heap *, void *);

int sdrl_base_load_file_type(struct sdrl_machine *mach, const char *name)
{
	if (!(file_type = sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, (sdrl_create_t) sdrl_create_file_type, NULL, NULL, (sdrl_destroy_t) sdrl_destroy_file_type)))
		return(-1);
	return(sdrl_add_binding(mach->type_env, name, file_type));
}

/*** Local Functions ***/

/**
 * Open the file.
 */
static void *sdrl_create_file_type(struct sdrl_machine *mach, struct sdrl_value *value)
{
	FILE *fptr;
	int ret = 0;
	struct sdrl_type *string;

	if (!value)
		ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
	else if (!(string = sdrl_find_binding(mach->type_env, "string")))
		ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL);
	else if (value->type != string)
		ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
	else if (!(fptr = fopen(value->data.str, "r")))
		ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, "Error opening file");
	if (ret)
		return(NULL);
	return((void *) fptr);
}

/**
 * Close the file.
 */
static int sdrl_destroy_file_type(struct sdrl_heap *heap, void *ptr)
{
	fclose((FILE *) ptr);
	return(0);
}


