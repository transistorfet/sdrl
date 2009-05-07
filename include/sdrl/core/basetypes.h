/*
 * Name:	basetypes.h
 * Description:	Base Types Header
 */


#ifndef _SDRL_CORE_BASETYPES_H
#define _SDRL_CORE_BASETYPES_H

#include <sdrl/core/heap.h>
#include <sdrl/core/utils.h>
#include <sdrl/globals.h>

#define SDNUMBER(ptr)		( (sdNumber *) (ptr) )
#define SDSTRING(ptr)		( (sdString *) (ptr) )
#define SDREFERENCE(ptr)	( (sdReference *) (ptr) )
#define SDPOINTER(ptr)		( (sdPointer *) (ptr) )

extern sdType sdNumberTypeDef;
extern sdType sdStringTypeDef;
extern sdType sdReferenceTypeDef;

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

sdNumber *sdrl_make_number(sdMachine *, sdType *, number_t);
sdNumber *sdrl_number_duplicate(sdMachine *, sdNumber *);

sdString *sdrl_make_string(sdMachine *, sdType *, const char *, int);
sdString *sdrl_string_duplicate(sdMachine *, sdString *);

sdReference *sdrl_make_reference(sdMachine *, sdType *, sdValue *);
void sdrl_reference_destroy(sdReference *);
sdReference *sdrl_reference_duplicate(sdMachine *, sdReference *);

sdPointer *sdrl_make_pointer(sdMachine *, sdType *, void *);
sdPointer *sdrl_pointer_duplicate(sdMachine *, sdPointer *);


static inline number_t sdrl_check_number(sdMachine *mach, sdValue *cur, sdType *type) {
	if (!cur) {
		sdrl_set_args_error(mach);
		return(0);
	}
	if ((type && cur->type != type) || (!type && cur->type != &sdNumberTypeDef)) {
		sdrl_set_type_error(mach, type ? type : &sdNumberTypeDef, cur->type);
		return(0);
	}
	return(SDNUMBER(cur)->num);
}

static inline const char *sdrl_check_string(sdMachine *mach, sdValue *cur, sdType *type) {
	if (!cur) {
		sdrl_set_args_error(mach);
		return(NULL);
	}
	if ((type && cur->type != type) || (!type && cur->type != &sdStringTypeDef)) {
		sdrl_set_type_error(mach, type ? type : &sdStringTypeDef, cur->type);
		return(NULL);
	}
	return(SDSTRING(cur)->str);
}

static inline void *sdrl_check_pointer(sdMachine *mach, sdValue *cur, sdType *type) {
	if (!cur) {
		sdrl_set_args_error(mach);
		return(NULL);
	}
	if (cur->type != type) {
		sdrl_set_type_error(mach, type, cur->type);
		return(NULL);
	}
	return(SDPOINTER(cur)->ptr);
}

static inline int sdrl_check_end(sdMachine *mach, sdValue *cur) {
	if (!cur)
		return(0);
	sdrl_set_args_error(mach);
	return(-1);
}

#endif

