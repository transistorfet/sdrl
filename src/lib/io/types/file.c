/*
 * Type Name:		file.c
 * Module Requirements:	stdio.h
 * Description:		File Descriptor Type
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/io.h>
#include <sdrl/lib/base.h>

sdType sdFileTypeDef = {
	&sdValueTypeDef,
	sizeof(sdPointer),
	0,
	(sdrl_create_t) sdrl_io_create_file,
	(sdrl_destroy_t) sdrl_io_destroy_file,
	NULL,
	NULL
};


sdValue *sdrl_io_create_file(sdMachine *mach, sdType *type, sdArray *args)
{
	FILE *fptr;

	if (args->last < 0) {
		sdrl_set_args_error(mach);
		return(NULL);
	}
	else if (!sdrl_value_isa(args->items[1], &sdStringTypeDef)) {
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


