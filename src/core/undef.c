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
sdValue *sdrl_make_undef(sdHeap *heap, sdType *type)
{
	sdValue *undef;

	if (!(undef = (sdValue *) sdrl_heap_alloc(heap, sizeof(sdValue))))
		return(NULL);
	SDVALUE(undef)->refs = 1;
	SDVALUE(undef)->type = type;
	return(undef);
}

/**
 * Free all memory associated with the undef
 */
int sdrl_undef_destroy(sdValue *undef)
{
	if (undef == &sdUndefValue)
		undef->refs++;
	else
		sdrl_heap_free(undef);
	return(0);
}


