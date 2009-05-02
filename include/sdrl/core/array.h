/*
 * Name:	array.h
 * Description:	Array Type Header File
 */


#ifndef _SDRL_CORE_ARRAY_H
#define _SDRL_CORE_ARRAY_H

#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDARRAY(ptr)		( (sdArray *) (ptr) )

extern sdType sdArrayTypeDef;

struct sdArray {
	sdValue value;
	int size;
	int last;
	sdValue **items;
};

sdArray *sdrl_make_array(sdHeap *, sdType *, int);

int sdrl_array_resize(sdArray *, int);
sdArray *sdrl_array_duplicate(sdMachine *, sdArray *);
void sdrl_array_destroy(sdArray *);

void sdrl_array_clear(sdArray *);
int sdrl_array_set(sdArray *, int, sdValue *);
sdValue *sdrl_array_get(sdArray *, int);
int sdrl_array_push(sdArray *, sdValue *);
sdValue *sdrl_array_pop(sdArray *);
sdValue *sdrl_array_shift(sdArray *);
int sdrl_array_unshift(sdArray *, sdValue *);

#endif

