/*
 * Name:	array.c
 * Description:	Array Type
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/array.h>
#include <sdrl/core/value.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>

#define SDRL_RESIZE_MAX		1024

/**
 * Create and return a type for sdArray.
 */
sdType *sdrl_make_array_type(void)
{
	return(sdrl_make_type(
		sizeof(sdArray),
		0,
		SDRL_BT_ARRAY,
		NULL,
		(sdrl_destroy_t) sdrl_destroy_array,
		(sdrl_duplicate_t) sdrl_duplicate_array,
		NULL
	));
}


/**
 * Return a newly allocated array of the given size
 */
sdArray *sdrl_make_array(sdHeap *heap, sdType *type, int size)
{
	sdArray *array;

	if (size <= 0)
		return(NULL);
	if (!(array = (sdArray *) sdrl_heap_alloc(heap, sizeof(sdArray))))
		return(NULL);
	array->items = NULL;
	if (size && !(array->items = (sdValue **) malloc(sizeof(sdValue *) * size))) {
		sdrl_heap_free(array);
		return(NULL);
	}
	memset(array->items, '\0', sizeof(sdValue *) * size);
	SDVALUE(array)->refs = 1;
	SDVALUE(array)->type = type;
	array->size = size;
	array->last = -1;
	return(array);
}

/**
 * Change the array to be the new given size.  If the array size is shrunk,
 * the references of any values at those indexes are decremented.
 */
int sdrl_resize_array(sdArray *array, int size)
{
	int i;
	sdValue **newitems;

	if (size == array->size)
		return(0);
	if (size <= 0)
		return(-1);
	// Destroy references at indicies that will disappear
	for (i = size; i < array->size; i++) {
		SDRL_DECREF(array->items[i]);
		array->items[i] = NULL;
	}
	if (!(newitems = (sdValue **) realloc(array->items, sizeof(sdValue *) * size)))
		return(-1);
	if (array->last >= size)
		array->last = size - 1;
	array->size = size;
	array->items = newitems;
	return(0);
}

/**
 * Create a duplicate of the array.
 */
sdArray *sdrl_duplicate_array(sdMachine *mach, sdArray *org)
{
	int i;
	sdArray *array;

	if (!(array = sdrl_make_array(mach->heap, SDVALUE(org)->type, org->size)))
		return(NULL);
	// TODO do you make a reference to each value or duplicate each value?
	for (i = 0; i < org->size; i++)
		array->items[i] = SDRL_INCREF(org->items[i]);
	array->last = org->last;
	return(array);
}

/**
 * Free all memory associated with the array
 */
int sdrl_destroy_array(sdArray *array)
{
	int i;

	for (i = 0; i < array->size; i++) {
		SDRL_DECREF(array->items[i]);
	}
	free(array->items);
	sdrl_heap_free(array);
	return(0);
}

/**
 * Set the item at the given index in the array to the given value.  The
 * ownership of the reference to the value given is taken.
 */
int sdrl_array_set(sdArray *array, int index, sdValue *value)
{
	if (index < 0 || index > array->size)
		return(-1);
	if (array->items[index])
		SDRL_DECREF(array->items[index]);
	array->items[index] = value;

	// Update the last entry marker
	if (!value && index == array->last) {
		for (; array->last >= 0 && !array->items[array->last]; array->last--)
			;
	}
	else if (index > array->last)
		array->last = index;
	return(0);
}

/**
 * A reference to the item at the given index in the array is returned.
 */
sdValue *sdrl_array_get(sdArray *array, int index)
{
	if (index < 0 || index > array->size)
		return(NULL);
	return(SDRL_INCREF(array->items[index]));
}

/**
 * Add the given value onto the end of the array growing it by one.
 */
int sdrl_array_push(sdArray *array, sdValue *value)
{
	// Resize the array if required
	if ((array->last + 1 >= array->size) && sdrl_resize_array(array, (array->size * 2 < SDRL_RESIZE_MAX) ? array->size * 2 : SDRL_RESIZE_MAX))
		return(-1);
	array->items[++array->last] = value;
	return(0);
}

/**
 * Remove a value from the end of the array and return it, decreasing the
 * array by one.
 */
sdValue *sdrl_array_pop(sdArray *array)
{
	sdValue *ret;

	// TODO shrink the array if it's appropriate
	if (array->last < 0)
		return(NULL);
	ret = array->items[array->last];
	array->items[array->last--] = NULL;
	return(ret);
}

/**
 * Remove a value from the start of the array and return it, decreasing the
 * array by one and shifting all elements down one.
 */
sdValue *sdrl_array_shift(sdArray *array)
{
	int i;
	sdValue *ret;

	// TODO shrink the array if it's appropriate
	ret = array->items[0];
	for (i = 0; i < array->last; i++)
		array->items[i] = array->items[i + 1];
	array->items[array->last] = NULL;
	array->last--;
	return(ret);
}

/**
 * Add the given value onto the start of the array growing it by one and
 * shifting all elements up one.
 */
int sdrl_array_unshift(sdArray *array, sdValue *value)
{
	int i;

	// Resize the array if required
	if ((array->last + 1 >= array->size) && sdrl_resize_array(array, (array->size * 2 < SDRL_RESIZE_MAX) ? array->size * 2 : SDRL_RESIZE_MAX))
		return(-1);
	for (i = array->last; i >= 0; i--)
		array->items[i + 1] = array->items[i];
	array->items[0] = value;
	array->last++;
	return(0);
}


