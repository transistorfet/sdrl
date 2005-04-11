/*
 * Name:	value.h
 * Version:	0.2
 * Description:	Value Manager Header
 */


#ifndef _SDRL_CORE_VALUE_H
#define _SDRL_CORE_VALUE_H

#include "type.h"
#include "heap.h"
#include "globals.h"

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

#define sdrl_make_reference_m(value) \
	++value->refs ? value : NULL

#define sdrl_value_is_true_m(value) \
	((sdrl_base_type_m(value->type) == SDRL_BT_NUMBER) && (!value->data.number)) \
	|| ((sdrl_base_type_m(value->type) == SDRL_BT_STRING) && (!value->data.str || value->data.str[0] == '\0')) \
	|| ((sdrl_base_type_m(value->type) == SDRL_BT_POINTER) && (!value->data.ptr))

struct sdrl_value *sdrl_make_value(struct sdrl_heap *, struct sdrl_type *, sdrl_data_t, int, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_value(struct sdrl_heap *, struct sdrl_value *);
int sdrl_destroy_value(struct sdrl_heap *, struct sdrl_value *);

int sdrl_push_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_pop_value(struct sdrl_value **);
int sdrl_unshift_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_shift_value(struct sdrl_value **);

int sdrl_value_count(struct sdrl_value *);

#endif

