/*
 * Module Name:	basetypes.c
 * Description:	Base Types
 */

#include <string.h>

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/core/machine.h>
#include <sdrl/core/basetypes.h>
#include <sdrl/globals.h>

/*** Number Type ***/

sdType sdNumberTypeDef = {
	&sdValueTypeDef,
	sizeof(sdNumber),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_number_duplicate,
	NULL
};


sdValue *sdrl_make_number(sdHeap *heap, sdType *type, number_t num)
{
	sdNumber *value;

	if (!(value = (sdNumber *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->num = num;
	return(SDVALUE(value));
}

sdValue *sdrl_number_duplicate(sdMachine *mach, sdNumber *org)
{
	return(sdrl_make_number(mach->heap, SDVALUE(org)->type, org->num));
}

/*** String Type ***/

sdType sdStringTypeDef = {
	&sdValueTypeDef,
	sizeof(sdString),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_string_duplicate,
	NULL
};


sdValue *sdrl_make_string(sdHeap *heap, sdType *type, const char *str, int len)
{
	sdString *value;

	if (!(value = (sdString *) sdrl_heap_alloc(heap, type->size + len + 1)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->len = len;
	value->str = (char *) (SDSTRING(value) + 1);
	memcpy(value->str, str, len);
	value->str[len] = '\0';		/** Just in case someone uses unbounded cstring functions */
	return(SDVALUE(value));
}

sdValue *sdrl_string_duplicate(sdMachine *mach, sdString *org)
{
	return(sdrl_make_string(mach->heap, SDVALUE(org)->type, org->str, org->len));
}


sdValue *sdrl_make_reference(sdHeap *heap, sdType *type, sdValue *ref)
{
	sdReference *value;

	if (!(value = (sdReference *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->ref = SDRL_INCREF(ref);
	return(SDVALUE(value));
}

void sdrl_reference_destroy(sdReference *value)
{
	SDRL_DECREF(value->ref);
	sdrl_heap_free(value);
}

sdValue *sdrl_reference_duplicate(sdMachine *mach, sdReference *org)
{
	return(sdrl_make_reference(mach->heap, SDVALUE(org)->type, org->ref));
}


sdValue *sdrl_make_pointer(sdHeap *heap, sdType *type, void *ptr)
{
	sdPointer *value;

	if (!(value = (sdPointer *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->ptr = ptr;
	return(SDVALUE(value));
}

sdValue *sdrl_pointer_duplicate(sdMachine *mach, sdPointer *org)
{
	return(sdrl_make_pointer(mach->heap, SDVALUE(org)->type, org->ptr));
}


