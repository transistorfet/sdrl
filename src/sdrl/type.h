/*
 * Name:	type.h
 * Version:	0.1
 * Description:	Type Manager Header
 */


#ifndef TYPE_H
#define TYPE_H

#include "types.h"

#define SDRL_VARIABLE_SIZE	-1

#define SDRL_BT_NUMBER		0x01
#define SDRL_BT_STRING		0x02
#define SDRL_BT_POINTER		0x03

#define SDRL_TBF_BASE_TYPE	0x0003
#define SDRL_TBF_PASS_EXPRS	0x0004

#define sdrl_base_type_m(type)		(type->bitflags & SDRL_TBF_BASE_TYPE)
#define sdrl_type_pass_exprs_m(type)	(type->bitflags & SDRL_TBF_PASS_EXPRS)

struct sdrl_machine;

typedef int (*sdrl_evaluate_t)(struct sdrl_machine *, ...);
typedef int (*sdrl_destroy_t)(void *);

struct sdrl_type {
	int size;
	int bitflags;
	sdrl_evaluate_t evaluate;
	sdrl_destroy_t destroy;
};

struct sdrl_type *sdrl_make_type(int, int, sdrl_evaluate_t, sdrl_destroy_t);
int sdrl_destroy_type(struct sdrl_type *);

#endif

