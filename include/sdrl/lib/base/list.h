/*
 * Header Name:	list.h
 * Description:	List Type Header
 */

#ifndef _SDRL_LIB_BASE_LIST_H
#define _SDRL_LIB_BASE_LIST_H

#include <sdrl/sdrl.h>

#define SDLIST(ptr)		( (sdList *) (ptr) )

typedef struct sdList sdList;

struct sdList {
	sdValue value;
	sdValue *item;
	sdList *next;
};

sdType *sdrl_base_make_list_type(sdMachine *);
sdValue *sdrl_base_create_list(sdMachine *, sdType *, sdArray *);

#endif

