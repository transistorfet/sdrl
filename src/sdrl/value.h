/*
 * Name:	value.h
 * Version:	0.1
 * Description:	Value Manager Header
 */


#ifndef _SDRL_VALUE_H
#define _SDRL_VALUE_H

#include "type.h"
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
	int binds;
	sdrl_data_t data;
	int size;
	struct sdrl_value *next;
};

struct sdrl_value *sdrl_make_value(struct sdrl_type *, sdrl_data_t, int, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_value(struct sdrl_value *);
int sdrl_push_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_pop_value(struct sdrl_value **);
int sdrl_unshift_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_shift_value(struct sdrl_value **);
int sdrl_destroy_value(struct sdrl_value *);

int sdrl_value_count(struct sdrl_value *);

#endif

