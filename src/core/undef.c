/*
 * Name:	undef.c
 * Description:	Undefined Type and Value
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/undef.h>
#include <sdrl/core/value.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>

sdType sdUndefTypeDef = {
	&sdValueTypeDef,
	"undef",
	sizeof(sdValue),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_undef_destroy,
	NULL,
	NULL
};

sdValue sdUndefValue = {
	1,
	&sdUndefTypeDef
};

/**
 * Return a newly allocated undef value
 */
sdValue *sdrl_make_undef(sdMachine *mach, sdType *type)
{
	return(sdrl_make_value(mach, type, 0));
}

/**
 * Free all memory associated with the undef
 */
void sdrl_undef_destroy(sdValue *undef)
{
	if (undef == &sdUndefValue)
		undef->refs++;
	else
		sdrl_heap_free(undef);
}


