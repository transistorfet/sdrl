/*
 * Name:	value.c
 * Version:	0.1
 * Description:	Value Manager
 */


#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "type.h"
#include "globals.h"


/**
 * Allocate and initialize an sdrl_value to a generic-typed data value.
 */
struct sdrl_value *sdrl_make_value(struct sdrl_type *type, sdrl_data_t data, int size, struct sdrl_value *next)
{
	struct sdrl_value *value;

	if (!type)
		return(NULL);
	if (sdrl_base_type_m(type) == SDRL_BT_STRING)
		size++;

	if (!(value = (struct sdrl_value *) malloc(sizeof(struct sdrl_value) + size)))
		return(NULL);

	if (size) {
		value->data = (sdrl_data_t) (char *) ((size_t) value + sizeof(struct sdrl_value));
		memcpy(value->data.ptr, data.ptr, size);
	}
	else
		value->data = data;

	value->type = type;
	value->binds = 0;
	value->size = size;
	value->next = next;

	return(value);
}

/**
 * Duplicate the value by direct memory copy.  Doesn't copy deep structure
 */
struct sdrl_value *sdrl_duplicate_value(struct sdrl_value *value)
{
	struct sdrl_value *tmp, *prev, *head = NULL;

	while (value) {
		if (!(tmp = (struct sdrl_value *) malloc(sizeof(struct sdrl_value) + value->size)))
			return(NULL);
		memcpy(tmp, value, sizeof(struct sdrl_value) + value->size);
		if (tmp->type->duplicate)
			tmp->data.ptr = tmp->type->duplicate(value->data.ptr);
		else if (tmp->size)
			tmp->data = (sdrl_data_t) (char *) ((size_t) tmp + sizeof(struct sdrl_value));
			

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
 * Frees memory of value and all of values links.  (Garbage Collection to see if value should *actually* be freed?)
 */
int sdrl_destroy_value(struct sdrl_value *value)
{
	struct sdrl_value *tmp;

	while (value) {
		// TODO check garbage collection
		if (value->binds)
			value = value->next;
		else {
			if (value->type->destroy)
				value->type->destroy(value->data.ptr);
			tmp = value->next;
			free(value);
			value = tmp;
		}
	}

	return(0);
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


