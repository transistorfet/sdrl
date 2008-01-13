/*
 * Type Name:		file.c
 * Module Requirements:	stdio.h
 * Description:		File Descriptor Type
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_value *sdrl_io_create_file(struct sdrl_machine *, struct sdrl_type *, struct sdrl_value *);
int sdrl_io_destroy_file(struct sdrl_value *);

struct sdrl_type *sdrl_base_make_file_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_pointer),
		0,
		SDRL_BT_POINTER,
		(sdrl_create_t) sdrl_io_create_file,
		(sdrl_destroy_t) sdrl_io_destroy_file,
		NULL,
		NULL
	));
}

struct sdrl_value *sdrl_io_create_file(struct sdrl_machine *mach, struct sdrl_type *type, struct sdrl_value *args)
{
	FILE *fptr;
	struct sdrl_type *str_type;

	if (!args) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	else if (!(str_type = sdrl_find_binding(mach->type_env, "string"))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL);
		return(NULL);
	}
	else if (args->type != str_type) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	else if (!(fptr = fopen(SDRL_STRING(args)->str, "r"))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, "Error opening file");
		return(NULL);
	}
	return(sdrl_make_pointer(mach->heap, type, fptr));
}

int sdrl_io_destroy_file(struct sdrl_value *value)
{
	fclose((FILE *) SDRL_POINTER(value)->ptr);
	return(0);
}


