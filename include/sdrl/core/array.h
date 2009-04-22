/*
 * Name:	array.h
 * Description:	Array Type Header File
 */


#ifndef _SDRL_CORE_ARRAY_H
#define _SDRL_CORE_ARRAY_H

#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDARRAY(ptr)		( (sdArray *) (ptr) )

struct sdArray {
	sdValue value;
	int size;
	int last;
	sdValue **items;
};

sdType *sdrl_make_array_type(void);

sdArray *sdrl_make_array(sdHeap *, sdType *, int);
int sdrl_resize_array(sdArray *, int);
sdArray *sdrl_duplicate_array(sdMachine *, sdArray *);
int sdrl_destroy_array(sdArray *);

int sdrl_array_set(sdArray *, int, sdValue *);
sdValue *sdrl_array_get(sdArray *, int);
int sdrl_array_push(sdArray *, sdValue *);
sdValue *sdrl_array_pop(sdArray *);
sdValue *sdrl_array_shift(sdArray *);
int sdrl_array_unshift(sdArray *, sdValue *);

#endif

