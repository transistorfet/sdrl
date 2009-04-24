/*
 * Type Name:		file.c
 * Module Requirements:	stdio.h
 * Description:		File Descriptor Type
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdValue *sdrl_io_create_file(sdMachine *, sdType *, sdArray *);
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

sdValue *sdrl_io_create_file(sdMachine *mach, sdType *type, sdArray *args)
{
	FILE *fptr;
	sdType *str_type;

	if (args->last < 0) {
		sdrl_set_args_error(mach);
		return(NULL);
	}
	else if (!(str_type = sdrl_env_find(mach->type_env, "string"))) {
		sdrl_set_type_error(mach);
		return(NULL);
	}
	else if (args->items[1]->type != str_type) {
		sdrl_set_type_error(mach);
		return(NULL);
	}
	else if (!(fptr = fopen(SDSTRING(args->items[1])->str, "r"))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_FAILED, "Error opening file");
		return(NULL);
	}
	return(sdrl_make_pointer(mach->heap, type, fptr));
}

int sdrl_io_destroy_file(sdValue *value)
{
	fclose((FILE *) SDPOINTER(value)->ptr);
	return(0);
}


