/*
 * Type Name:		file.c
 * Module Requirements:	stdio.h
 * Description:		File Descriptor Type
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdValue *sdrl_io_create_file(sdMachine *, sdType *, sdValue *);
int sdrl_io_destroy_file(sdValue *);

sdType *sdrl_base_make_file_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdPointer),
		0,
		SDRL_BT_POINTER,
		(sdrl_create_t) sdrl_io_create_file,
		(sdrl_destroy_t) sdrl_io_destroy_file,
		NULL,
		NULL
	));
}

sdValue *sdrl_io_create_file(sdMachine *mach, sdType *type, sdValue *args)
{
	FILE *fptr;
	sdType *str_type;

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
	else if (!(fptr = fopen(SDSTRING(args)->str, "r"))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, "Error opening file");
		return(NULL);
	}
	return(sdrl_make_pointer(mach->heap, type, fptr));
}

int sdrl_io_destroy_file(sdValue *value)
{
	fclose((FILE *) SDPOINTER(value)->ptr);
	return(0);
}


