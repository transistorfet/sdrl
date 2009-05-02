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
	"file",
	sizeof(sdPointer),
	0,
	(sdrl_create_t) sdrl_io_file_create,
	(sdrl_destroy_t) sdrl_io_file_destroy,
	NULL,
	NULL
};


sdValue *sdrl_io_file_create(sdMachine *mach, sdType *type, sdArray *args)
{
	FILE *fptr;

	if (args->last < 0) {
		sdrl_set_args_error(mach);
		return(NULL);
	}
	else if (!sdrl_value_isa(args->items[1], &sdStringTypeDef)) {
		sdrl_set_type_error(mach, &sdStringTypeDef, args->items[1]->type);
		return(NULL);
	}
	else if (!(fptr = fopen(SDSTRING(args->items[1])->str, "r"))) {
		sdrl_set_error(mach, SDRL_ES_LOW, SDRL_ERR_FAILED, "Error opening file");
		return(NULL);
	}
	return(SDVALUE(sdrl_make_pointer(mach->heap, type, fptr)));
}

void sdrl_io_file_destroy(sdValue *value)
{
	fclose((FILE *) SDPOINTER(value)->ptr);
}


