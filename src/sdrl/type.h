/*
 * Name:	type.h
 * Version:	0.1
 * Description:	Type Manager Header
 */


#ifndef TYPE_H
#define TYPE_H

#include "types.h"

#define SDRL_VARIABLE_SIZE	-1

#define SDRL_TBF_EVAL_EXPRS	0x0001
#define SDRL_TBF_EVAL_ARGS	0x0000

#define sdrl_type_eval_params_m(type)	(type->bitflags & SDRL_TBF_EVAL_EXPRS)

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

