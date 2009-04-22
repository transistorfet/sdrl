/*
 * Name:	value.c
 * Description:	Value Manager
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/value.h>
#include <sdrl/core/type.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>


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


