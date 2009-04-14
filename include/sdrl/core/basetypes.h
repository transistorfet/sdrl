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

#define SDRL_NUMBER(ptr)	( (struct sdrl_number *) (ptr) )
#define SDRL_STRING(ptr)	( (struct sdrl_string *) (ptr) )
#define SDRL_REFERENCE(ptr)	( (struct sdrl_reference *) (ptr) )
#define SDRL_POINTER(ptr)	( (struct sdrl_pointer *) (ptr) )

struct sdrl_number {
	struct sdrl_value value;
	number_t num;
};

struct sdrl_string {
	struct sdrl_value value;
	int len;
	char *str;
};

struct sdrl_reference {
	struct sdrl_value value;
	struct sdrl_value *ref;
};

struct sdrl_pointer {
	struct sdrl_value value;
	void *ptr;
};

struct sdrl_type *sdrl_make_number_type(void);
struct sdrl_value *sdrl_make_number(struct sdrl_heap *, struct sdrl_type *, number_t);
struct sdrl_value *sdrl_duplicate_number(struct sdrl_machine *, struct sdrl_number *);

struct sdrl_type *sdrl_make_string_type(void);
struct sdrl_value *sdrl_make_string(struct sdrl_heap *, struct sdrl_type *, const char *, int);
struct sdrl_value *sdrl_duplicate_string(struct sdrl_machine *, struct sdrl_string *);

struct sdrl_value *sdrl_make_reference(struct sdrl_heap *, struct sdrl_type *, struct sdrl_value *);
int sdrl_destroy_reference(struct sdrl_reference *);
struct sdrl_value *sdrl_duplicate_reference(struct sdrl_machine *, struct sdrl_reference *);

struct sdrl_value *sdrl_make_pointer(struct sdrl_heap *, struct sdrl_type *, void *);
struct sdrl_value *sdrl_duplicate_pointer(struct sdrl_machine *, struct sdrl_pointer *);

struct sdrl_type *sdrl_make_expression_type(void);
struct sdrl_type *sdrl_make_environment_type(void);


static inline number_t sdrl_check_number(struct sdrl_machine *mach, struct sdrl_value *cur, short basetype, struct sdrl_type *type) {
	if (!cur) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(0);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(0);
	}
	return(SDRL_NUMBER(cur)->num);
}

static inline const char *sdrl_check_string(struct sdrl_machine *mach, struct sdrl_value *cur, short basetype, struct sdrl_type *type) {
	if (!cur) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	return(SDRL_STRING(cur)->str);
}

static inline void *sdrl_check_pointer(struct sdrl_machine *mach, struct sdrl_value *cur, short basetype, struct sdrl_type *type) {
	if (!cur) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	if (cur->type->basetype != basetype || (type && (cur->type != type))) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	return(SDRL_POINTER(cur)->ptr);
}

static inline int sdrl_check_end(struct sdrl_machine *mach, struct sdrl_value *cur) {
	if (!cur)
		return(0);
	SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
	return(-1);
}

#endif

