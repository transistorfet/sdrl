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
 * Allocate and initialize an sdrl_value to a generic-typed data value.
 */
struct sdrl_value *sdrl_make_value(struct sdrl_heap *heap, struct sdrl_type *type, sdrl_data_t data, int size, struct sdrl_value *next)
{
	struct sdrl_value *value;

	if (!type)
		return(NULL);
	if (SDRL_BASE_TYPE(type) == SDRL_BT_STRING)
		size++;

	if (!(value = (struct sdrl_value *) sdrl_heap_alloc(heap, sizeof(struct sdrl_value) + size)))
		return(NULL);

	if (size) {
		value->data = (sdrl_data_t) (char *) (value + 1);
		memcpy(value->data.ptr, data.ptr, size);
	}
	else
		value->data = data;

	value->type = type;
	value->refs = 1;
	value->size = size;
	value->next = next;

	return(value);
}

/**
 * Duplicate the value by direct memory copy.  Doesn't copy deep structure
 */
struct sdrl_value *sdrl_duplicate_value(struct sdrl_heap *heap, struct sdrl_value *value)
{
	struct sdrl_value *newvalue, *prev = NULL, *head = NULL;

	while (value) {
		if (!(newvalue = (struct sdrl_value *) sdrl_heap_alloc(heap, sizeof(struct sdrl_value) + value->size)))
			return(NULL);
		memcpy(newvalue, value, sizeof(struct sdrl_value) + value->size);
		if (value->type->duplicate)
			newvalue->data.ptr = value->type->duplicate(heap, value->data.ptr);
		else if (newvalue->size)
			newvalue->data = (sdrl_data_t) (char *) (newvalue + 1);

		newvalue->refs = 1;
		if (!head)
			head = newvalue;
		else
			prev->next = newvalue;
		prev = newvalue;
		value = value->next;
	}

	return(head);
}

/**
 * Duplicate the value by direct memory copy.  Doesn't copy deep structure.
 * Only the single address value is copied and not the values pointed to by
 * "next".
 */
struct sdrl_value *sdrl_duplicate_single_value(struct sdrl_heap *heap, struct sdrl_value *value)
{
	struct sdrl_value *newvalue;

	if (!(newvalue = (struct sdrl_value *) sdrl_heap_alloc(heap, sizeof(struct sdrl_value) + value->size)))
		return(NULL);
	memcpy(newvalue, value, sizeof(struct sdrl_value) + value->size);
	if (value->type->duplicate)
		newvalue->data.ptr = value->type->duplicate(heap, value->data.ptr);
	else if (newvalue->size)
		newvalue->data = (sdrl_data_t) (char *) (newvalue + 1);
	newvalue->refs = 1;
	newvalue->next = NULL;

	return(newvalue);
}

/**
 * Frees memory of value and all of values links.
 */
int sdrl_destroy_value(struct sdrl_heap *heap, struct sdrl_value *value)
{
	struct sdrl_value *tmp;

	while (value) {
		if (--value->refs)
			return(1);
		if (value->type->destroy)
			value->type->destroy(heap, value->data.ptr);
		tmp = value->next;
		sdrl_heap_free(heap, value);
		value = tmp;
	}

	return(0);
}


/**
 * Adds the value to the linked-list of values.
 */
int sdrl_push_value(struct sdrl_value **array, struct sdrl_value *value)
{
	struct sdrl_value *cur;

	if (!array)
		return(SDRL_ERR_NOT_FOUND);
	else if (!*array)
		*array = value;
	else {
		cur = *array;
		while (cur->next)
			cur = cur->next;
		cur->next = value;
	}
	return(0);
}

/**
 * Removes the last value from the linked-list of values and returns it.
 */
struct sdrl_value *sdrl_pop_value(struct sdrl_value **array)
{
	struct sdrl_value *cur, *value;

	if (!array)
		return(NULL);

	cur = *array;
	if (!cur)
		return(NULL);
	else if (!cur->next) {
		value = cur;
		*array = NULL;
	}
	else {
		while (cur->next->next)
			cur = cur->next;
		value = cur->next;
		cur->next = NULL;
	}
	return(value);
}

/**
 * Add the value to the front of the array.
 */
int sdrl_unshift_value(struct sdrl_value **array, struct sdrl_value *value)
{
	if (!array)
		return(SDRL_ERR_NOT_FOUND);
	value->next = *array;
	*array = value;
	return(0);
}

/**
 * Removes the first value from the linked-list of values and returns it.
 */
struct sdrl_value *sdrl_shift_value(struct sdrl_value **array)
{
	struct sdrl_value *value;

	if (!array)
		return(NULL);
	if (!(value = *array))
		return(NULL);
	*array = value->next;
	value->next = NULL;
	return(value);
}


