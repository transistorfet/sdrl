/*
 * Name:	value.h
 * Description:	Value Manager Header
 */


#ifndef _SDRL_CORE_VALUE_H
#define _SDRL_CORE_VALUE_H

#include <sdrl/core/type.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>

#define sdrl_void_c	(sdrl_data_t) (void *)
#define sdrl_char_c	(sdrl_data_t) (char *)

typedef union sdrl_data {
	number_t number;
	char *str;
	void *ptr;
} sdrl_data_t;

struct sdrl_value {
	struct sdrl_type *type;
	short refs;
	sdrl_data_t data;
	int size;
	struct sdrl_value *next;
};


#define SDRL_VALUE_IS_FALSE(value) \
	((SDRL_BASE_TYPE((value)->type) == SDRL_BT_NUMBER) && (!(value)->data.number)) \
	|| ((SDRL_BASE_TYPE((value)->type) == SDRL_BT_STRING) && (!(value)->data.str || (value)->data.str[0] == '\0')) \
	|| ((SDRL_BASE_TYPE((value)->type) == SDRL_BT_POINTER) && (!(value)->data.ptr))

struct sdrl_value *sdrl_make_value(struct sdrl_heap *, struct sdrl_type *, sdrl_data_t, int, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_value(struct sdrl_heap *, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_single_value(struct sdrl_heap *, struct sdrl_value *);
int sdrl_destroy_value(struct sdrl_heap *, struct sdrl_value *);

int sdrl_push_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_pop_value(struct sdrl_value **);
int sdrl_unshift_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_shift_value(struct sdrl_value **);

int sdrl_value_count(struct sdrl_value *);

#endif

