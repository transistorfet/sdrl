/*
 * Header Name:	list.h
 * Description:	List Type Header
 */

#ifndef _SDRL_LIB_BASE_LIST_H
#define _SDRL_LIB_BASE_LIST_H

#include <sdrl/sdrl.h>

#define SDLIST(ptr)		( (sdList *) (ptr) )

extern sdType sdListTypeDef;

typedef struct sdList sdList;

struct sdList {
	sdValue value;
	sdValue *item;
	sdList *next;
};

sdValue *sdrl_base_list_create(sdMachine *, sdType *, sdArray *);

#endif

