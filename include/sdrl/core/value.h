/*
 * Name:	value.h
 * Description:	Value Manager Header
 */


#ifndef _SDRL_CORE_VALUE_H
#define _SDRL_CORE_VALUE_H

#include <sdrl/core/type.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>

#define SDRL_VALUE(ptr)		( (struct sdrl_value *) (ptr) )

struct sdrl_value {
	int refs;
	struct sdrl_type *type;
	struct sdrl_value *next;
};

/** Increments the count of a reference for values and so on. (destroy decrements) **/
#define SDRL_MAKE_REFERENCE(datum)				\
	( ((datum) && ++SDRL_VALUE(datum)->refs) ?		\
		(datum)						\
		: NULL )

/** Decrements the count of a reference for values and so on and calls destroy if the count is 0 **/
#define SDRL_DESTROY_REFERENCE(datum)					\
	( (datum) ?							\
		( (SDRL_VALUE(datum)->refs == 1) ?			\
			( sdrl_destroy_value(SDRL_VALUE(datum)), 1 )	\
			: ( --SDRL_VALUE(datum)->refs, 0 ) )		\
		: 0 )

struct sdrl_value *sdrl_duplicate_value(struct sdrl_machine *, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_single_value(struct sdrl_machine *, struct sdrl_value *);
int sdrl_destroy_value(struct sdrl_value *);

int sdrl_push_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_pop_value(struct sdrl_value **);
int sdrl_unshift_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_shift_value(struct sdrl_value **);

/**
 * Returns the number of elements in the linked-list of values.
 */
static inline int sdrl_value_count(struct sdrl_value *array) {
	int i = 0;

	for (;array;array = array->next)
		i++;
	return(i);
}

#endif

