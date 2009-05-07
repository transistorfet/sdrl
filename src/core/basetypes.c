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
	"number",
	sizeof(sdNumber),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_number_duplicate,
	NULL
};


sdNumber *sdrl_make_number(sdMachine *mach, sdType *type, number_t num)
{
	sdNumber *value;

	if (!(value = (sdNumber *) sdrl_heap_alloc(mach->heap, type->size)))
		return(NULL);
	memset(value, '\0', type->size);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->num = num;
	return(value);
}

sdNumber *sdrl_number_duplicate(sdMachine *mach, sdNumber *org)
{
	return(sdrl_make_number(mach, SDVALUE(org)->type, org->num));
}

/*** String Type ***/

sdType sdStringTypeDef = {
	&sdValueTypeDef,
	"string",
	sizeof(sdString),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_string_duplicate,
	NULL
};


sdString *sdrl_make_string(sdMachine *mach, sdType *type, const char *str, int len)
{
	sdString *value;

	if (!(value = (sdString *) sdrl_heap_alloc(mach->heap, type->size + len + 1)))
		return(NULL);
	memset(value, '\0', type->size);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->len = len;
	value->str = (char *) (((char *) value) + type->size);
	memcpy(value->str, str, len);
	value->str[len] = '\0';		/** Just in case someone uses unbounded cstring functions */
	return(value);
}

sdString *sdrl_string_duplicate(sdMachine *mach, sdString *org)
{
	return(sdrl_make_string(mach, SDVALUE(org)->type, org->str, org->len));
}

/*** Reference Type ***/

sdType sdReferenceTypeDef = {
	&sdValueTypeDef,
	"reference",
	sizeof(sdReference),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_reference_destroy,
	(sdrl_duplicate_t) sdrl_reference_duplicate,
	NULL
};

sdReference *sdrl_make_reference(sdMachine *mach, sdType *type, sdValue *ref)
{
	sdReference *value;

	if (!(value = (sdReference *) sdrl_heap_alloc(mach->heap, type->size)))
		return(NULL);
	memset(value, '\0', type->size);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->ref = SDRL_INCREF(ref);
	return(value);
}

void sdrl_reference_destroy(sdReference *value)
{
	SDRL_DECREF(value->ref);
	sdrl_heap_free(value);
}

sdReference *sdrl_reference_duplicate(sdMachine *mach, sdReference *org)
{
	return(sdrl_make_reference(mach, SDVALUE(org)->type, org->ref));
}


sdPointer *sdrl_make_pointer(sdMachine *mach, sdType *type, void *ptr)
{
	sdPointer *value;

	if (!(value = (sdPointer *) sdrl_heap_alloc(mach->heap, type->size)))
		return(NULL);
	memset(value, '\0', type->size);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->ptr = ptr;
	return(value);
}

sdPointer *sdrl_pointer_duplicate(sdMachine *mach, sdPointer *org)
{
	return(sdrl_make_pointer(mach, SDVALUE(org)->type, org->ptr));
}


