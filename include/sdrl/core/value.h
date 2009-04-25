/*
 * Name:	value.h
 * Description:	Value Manager Header
 */


#ifndef _SDRL_CORE_VALUE_H
#define _SDRL_CORE_VALUE_H

#include <sdrl/globals.h>

#define SDRL_TBF_PASS_EXPRS	0x0001		/** Pass the (unevaluated) expressions to evaluate function */

struct sdType {
	sdType *parent;
	int size;
	int bitflags;
	sdrl_create_t create;
	sdrl_destroy_t destroy;
	sdrl_duplicate_t duplicate;
	sdrl_evaluate_t evaluate;
};

#define SDVALUE(ptr)		( (sdValue *) (ptr) )

extern sdType sdValueTypeDef;

struct sdValue {
	int refs;
	sdType *type;
};

sdValue *sdrl_duplicate_value(sdMachine *, sdValue *);
void sdrl_destroy_value(sdValue *);

/** Increments the count of a reference for values and so on. (destroy decrements) **/
#define SDRL_INCREF(datum)				\
	( ((datum) && ++SDVALUE(datum)->refs) ?		\
		(datum)					\
		: NULL )

/** Decrements the count of a reference for values and so on and calls destroy if the count is 0 **/
#define SDRL_DECREF(datum)						\
	( (datum) ?							\
		( (SDVALUE(datum)->refs == 1) ?				\
			( sdrl_destroy_value(SDVALUE(datum)), 1 )	\
			: ( --SDVALUE(datum)->refs, 0 ) )		\
		: 0 )

/**
 * Returns 1 if the type matches the type or a parent type of the value.
 */
static inline int sdrl_value_isa(sdValue *value, sdType *type)
{
	sdType *cur;

	for (cur = value->type; cur; cur = cur->parent) {
		if (cur == type)
			return(1);
	}
	return(0);
}

/**
 * Notes on sdrl_type:
 *	- The 'size' field is meant to be the fixed allocation size of the type
 *	  not including memory allocated seperately and not including variable
 * 	  data such as strings that might be allocated along with the value
 *	  itself.  All value creating functions should, if given a type struct,
 *	  use the 'size' field as the base size plus any additional memory
 *	  needed.
 */

#endif

