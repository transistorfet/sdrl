/*
 * Name:	value.h
 * Description:	Value Manager Header
 */


#ifndef _SDRL_CORE_VALUE_H
#define _SDRL_CORE_VALUE_H

#include <sdrl/core/type.h>
#include <sdrl/globals.h>

#define SDVALUE(ptr)		( (sdValue *) (ptr) )

struct sdValue {
	int refs;
	sdType *type;
};

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


sdValue *sdrl_duplicate_value(sdMachine *, sdValue *);
void sdrl_destroy_value(sdValue *);

#endif

