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
sdValue *sdrl_duplicate_value(sdMachine *mach, sdValue *value)
{
	sdValue *newvalue, *prev = NULL, *head = NULL;

	while (value) {
		if (!value->type->duplicate)
			return(NULL);
		newvalue = value->type->duplicate(mach, value);
		if (!prev)
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
sdValue *sdrl_duplicate_single_value(sdMachine *mach, sdValue *value)
{
	if (!value || !value->type->duplicate)
		return(NULL);
	return(value->type->duplicate(mach, value));
}

/**
 * Frees memory of value and all of values links.
 */
int sdrl_destroy_value(sdValue *value)
{
	sdValue *tmp;

	while (value) {
		if (--value->refs)
			return(1);
		tmp = value->next;
		if (value->type->destroy)
			value->type->destroy(value);
		value = tmp;
	}
	return(0);
}


/**
 * Adds the value to the linked-list of values.
 */
int sdrl_push_value(sdValue **array, sdValue *value)
{
	sdValue *cur;

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
sdValue *sdrl_pop_value(sdValue **array)
{
	sdValue *cur, *value;

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
int sdrl_unshift_value(sdValue **array, sdValue *value)
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
sdValue *sdrl_shift_value(sdValue **array)
{
	sdValue *value;

	if (!array)
		return(NULL);
	if (!(value = *array))
		return(NULL);
	*array = value->next;
	value->next = NULL;
	return(value);
}


