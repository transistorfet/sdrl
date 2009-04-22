/*
 * Name:	basetypes.h
 * Description:	Base Types Header
 */


#ifndef _SDRL_CORE_BASETYPES_H
#define _SDRL_CORE_BASETYPES_H

#include <sdrl/core/type.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>

#define SDNUMBER(ptr)		( (sdNumber *) (ptr) )
#define SDSTRING(ptr)		( (sdString *) (ptr) )
#define SDREFERENCE(ptr)	( (sdReference *) (ptr) )
#define SDPOINTER(ptr)		( (sdPointer *) (ptr) )

typedef struct sdNumber sdNumber;
typedef struct sdString sdString;
typedef struct sdReference sdReference;
typedef struct sdPointer sdPointer;

struct sdNumber {
	sdValue value;
	number_t num;
};

struct sdString {
	sdValue value;
	int len;
	char *str;
};

struct sdReference {
	sdValue value;
	sdValue *ref;
};

struct sdPointer {
	sdValue value;
	void *ptr;
};

sdType *sdrl_make_number_type(void);
sdValue *sdrl_make_number(sdHeap *, sdType *, number_t);
sdValue *sdrl_duplicate_number(sdMachine *, sdNumber *);

sdType *sdrl_make_string_type(void);
sdValue *sdrl_make_string(sdHeap *, sdType *, const char *, int);
sdValue *sdrl_duplicate_string(sdMachine *, sdString *);

sdValue *sdrl_make_reference(sdHeap *, sdType *, sdValue *);
int sdrl_destroy_reference(sdReference *);
sdValue *sdrl_duplicate_reference(sdMachine *, sdReference *);

sdValue *sdrl_make_pointer(sdHeap *, sdType *, void *);
sdValue *sdrl_duplicate_pointer(sdMachine *, sdPointer *);


static inline number_t sdrl_check_number(sdMachine *mach, sdValue *cur, short basetype, sdType *type) {
	if (!cur) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(0);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(0);
	}
	return(SDNUMBER(cur)->num);
}

static inline const char *sdrl_check_string(sdMachine *mach, sdValue *cur, short basetype, sdType *type) {
	if (!cur) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	return(SDSTRING(cur)->str);
}

static inline void *sdrl_check_pointer(sdMachine *mach, sdValue *cur, short basetype, sdType *type) {
	if (!cur) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	return(SDPOINTER(cur)->ptr);
}

static inline int sdrl_check_end(sdMachine *mach, sdValue *cur) {
	if (!cur)
		return(0);
	sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
	return(-1);
}

#endif

