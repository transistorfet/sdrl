/*
 * Name:	value.c
 * Description:	Value Manager
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/value.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>

sdType sdValueTypeDef = {
	NULL,
	"value",
	sizeof(sdValue),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_destroy_value,
	(sdrl_duplicate_t) sdrl_duplicate_value,
	NULL
};


/**
 * Allocate a value of size type->size + size and return it.  The value's refs
 * and type fields are initialized and the rest of the memory is zero'd.
 */
sdValue *sdrl_make_value(sdMachine *mach, sdType *type, int size)
{
	sdValue *value;

	if (!(value = (sdValue *) sdrl_heap_alloc(mach->heap, type->size + size)))
		return(NULL);
	memset(value, '\0', type->size + size);
	value->refs = 1;
	value->type = type;
	return(value);
}

/**
 * Duplicate the value.  Doesn't copy deep structure
 */
sdValue *sdrl_duplicate_value(sdMachine *mach, sdValue *org)
{
	if (!org || !org->type->duplicate)
		return(NULL);
	return(org->type->duplicate(mach, org));
}

/**
 * Frees memory of value and all of values links.
 */
void sdrl_destroy_value(sdValue *value)
{
	if (--value->refs)
		return;
	if (value && value->type->destroy)
		value->type->destroy(value);
}


