/*
 * Name:	value.h
 * Version:	0.1
 * Description:	Value Manager Header
 */


#ifndef VALUE_H
#define VALUE_H

#include "types.h"

#define SDRL_VT_NULL		0x00
#define SDRL_VT_NUMBER		0x01
#define SDRL_VT_STRING		0x02
#define SDRL_VT_EXPR		0x03
#define SDRL_VT_VALUE		0x04
#define SDRL_VT_ENVIRONMENT	0x05

#define SDRL_VT_FUNCPTR		0x06

struct sdrl_expr;
struct sdrl_value;
struct sdrl_environment;

typedef union sdrl_data {
	number_t number;
	char *str;
	struct sdrl_expr *expr;
	struct sdrl_value *value;
	struct sdrl_envrionment *env;
} sdrl_data_t;

struct sdrl_value {
	int type;
	int binds;
	sdrl_data_t data;
	int size;
	struct sdrl_value *next;
};

struct sdrl_value *sdrl_make_value(int, sdrl_data_t, int, struct sdrl_value *);
struct sdrl_value *sdrl_duplicate_value(struct sdrl_value *);
int sdrl_push_value(struct sdrl_value **, struct sdrl_value *);
struct sdrl_value *sdrl_pop_value(struct sdrl_value **);
int sdrl_destroy_value(struct sdrl_value *);

int sdrl_value_count(struct sdrl_value *);

#endif

