/*
 * Module Name:	basetypes.c
 * Description:	Base Types
 */

#include <string.h>

#include <sdrl/core/heap.h>
#include <sdrl/core/type.h>
#include <sdrl/core/value.h>
#include <sdrl/core/machine.h>
#include <sdrl/core/basetypes.h>
#include <sdrl/globals.h>

/*** Number Type ***/

struct sdrl_type *sdrl_make_number_type(void)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_number),
		0,
		SDRL_BT_NUMBER,
		NULL,
		(sdrl_destroy_t) sdrl_heap_free,
		(sdrl_duplicate_t) sdrl_duplicate_number,
		NULL
	));
}

struct sdrl_value *sdrl_make_number(struct sdrl_heap *heap, struct sdrl_type *type, number_t num)
{
	struct sdrl_number *value;

	if (!(value = (struct sdrl_number *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = type;
	SDRL_VALUE(value)->next = NULL;
	value->num = num;
	return(SDRL_VALUE(value));
}

struct sdrl_value *sdrl_duplicate_number(struct sdrl_machine *mach, struct sdrl_number *org)
{
	return(sdrl_make_number(mach->heap, SDRL_VALUE(org)->type, org->num));
}

/*** String Type ***/

struct sdrl_type *sdrl_make_string_type(void)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_string),
		0,
		SDRL_BT_STRING,
		NULL,
		(sdrl_destroy_t) sdrl_heap_free,
		(sdrl_duplicate_t) sdrl_duplicate_string,
		NULL
	));
}

struct sdrl_value *sdrl_make_string(struct sdrl_heap *heap, struct sdrl_type *type, const char *str, int len)
{
	struct sdrl_string *value;

	if (!(value = (struct sdrl_string *) sdrl_heap_alloc(heap, type->size + len + 1)))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = type;
	SDRL_VALUE(value)->next = NULL;
	value->len = len;
	value->str = (char *) (SDRL_STRING(value) + 1);
	memcpy(value->str, str, len);
	value->str[len] = '\0';		/** Just in case someone uses unbounded cstring functions */
	return(SDRL_VALUE(value));
}

struct sdrl_value *sdrl_duplicate_string(struct sdrl_machine *mach, struct sdrl_string *org)
{
	return(sdrl_make_string(mach->heap, SDRL_VALUE(org)->type, org->str, org->len));
}


struct sdrl_value *sdrl_make_reference(struct sdrl_heap *heap, struct sdrl_type *type, struct sdrl_value *ref)
{
	struct sdrl_reference *value;

	if (!(value = (struct sdrl_reference *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = type;
	SDRL_VALUE(value)->next = NULL;
	value->ref = SDRL_MAKE_REFERENCE(ref);
	return(SDRL_VALUE(value));
}

int sdrl_destroy_reference(struct sdrl_reference *value)
{
	SDRL_DESTROY_REFERENCE(value->ref);
	sdrl_heap_free(value);
	return(0);
}

struct sdrl_value *sdrl_duplicate_reference(struct sdrl_machine *mach, struct sdrl_reference *org)
{
	return(sdrl_make_reference(mach->heap, SDRL_VALUE(org)->type, org->ref));
}


struct sdrl_value *sdrl_make_pointer(struct sdrl_heap *heap, struct sdrl_type *type, void *ptr)
{
	struct sdrl_pointer *value;

	if (!(value = (struct sdrl_pointer *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = type;
	SDRL_VALUE(value)->next = NULL;
	value->ptr = ptr;
	return(SDRL_VALUE(value));
}

struct sdrl_value *sdrl_duplicate_pointer(struct sdrl_machine *mach, struct sdrl_pointer *org)
{
	return(sdrl_make_pointer(mach->heap, SDRL_VALUE(org)->type, org->ptr));
}


