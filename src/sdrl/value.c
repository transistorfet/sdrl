/*
 * Name:	value.c
 * Version:	0.1
 * Description:	Value Manager
 */


#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "types.h"


/**
 * Allocate and initialize an sdrl_value to a generic-typed data value.
 */
struct sdrl_value *sdrl_make_value(int type, sdrl_data_t data, int size, struct sdrl_value *next)
{
	struct sdrl_value *value;

	if (!(value = (struct sdrl_value *) malloc(sizeof(struct sdrl_value))))
		return(NULL);
	value->type = type;
	value->binds = 0;
	value->data = data;
	value->size = size;
	value->next = next;

	return(value);
}

/**
 * Duplicate the value by direct memory copy including deep structure (Garbage Collection tracking code?)
 */
struct sdrl_value *sdrl_duplicate_value(struct sdrl_value *value)
{
	struct sdrl_value *tmp, *prev, *head = NULL;

	while (value) {
		if (!(tmp = (struct sdrl_value *) malloc(value->size)))
			return(NULL);
		memcpy(tmp, value, value->size);
		if (value->type == SDRL_VT_VALUE)
			tmp->data.value = sdrl_duplicate_value(value->data.value);
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
 * Frees memory of value and all of values links.  (Garbage Collection to see if value should *actually* be freed?)
 */
int sdrl_destroy_value(struct sdrl_value *value)
{
	struct sdrl_value *tmp;

	while (value) {
		tmp = value->next;
		// TODO free value if it has been allocated elsewhere (value, evironment, expr, etc)
		free(value);
		value = tmp;
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


