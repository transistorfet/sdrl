/*
 * Name:	value.c
 * Version:	0.2
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
	if (sdrl_base_type_m(type) == SDRL_BT_STRING)
		size++;

	if (!(value = (struct sdrl_value *) sdrl_heap_alloc(heap, sizeof(struct sdrl_value) + size)))
		return(NULL);

	if (size) {
		value->data = (sdrl_data_t) (char *) ((size_t) value + sizeof(struct sdrl_value));
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
	struct sdrl_value *tmp, *prev, *head = NULL;

	while (value) {
		if (!(tmp = (struct sdrl_value *) sdrl_heap_alloc(heap, sizeof(struct sdrl_value) + value->size)))
			return(NULL);
		memcpy(tmp, value, sizeof(struct sdrl_value) + value->size);
		if (tmp->type->duplicate)
			tmp->data.ptr = tmp->type->duplicate(heap, value->data.ptr);
		else if (tmp->size)
			tmp->data = (sdrl_data_t) (char *) ((size_t) tmp + sizeof(struct sdrl_value));

		tmp->refs = 1;
		if (!head)
			head = tmp;
		else
			prev->next = tmp;
		prev = tmp;
		value = value->next;
	}

	return(head);
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
		return(ERR_NOT_FOUND);
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
		return(ERR_NOT_FOUND);
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


/**
 * Returns the number of elements in the linked-list of values.
 */
int sdrl_value_count(struct sdrl_value *array)
{
	int i = 0;

	while (array) {
		i++;
		array = array->next;
	}
	return(i);
}


