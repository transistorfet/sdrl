/*
 * Name:	prims.h
 * Version:	0.1
 * Description:	Primative Functions Header
 */


#ifndef _SDRL_LIB_PRIMS_H
#define _SDRL_LIB_PRIMS_H

#include <sdrl.h>

int prim_initialize(struct sdrl_machine *);

int prim_code(struct sdrl_machine *, struct sdrl_expr *);

int prim_set(struct sdrl_machine *, struct sdrl_value *);
int prim_list(struct sdrl_machine *, struct sdrl_value *);
int prim_if(struct sdrl_machine *, struct sdrl_value *);

int prim_resolve(struct sdrl_machine *, struct sdrl_value *);
int prim_add(struct sdrl_machine *, struct sdrl_value *);
int prim_subtract(struct sdrl_machine *, struct sdrl_value *);
int prim_multiply(struct sdrl_machine *, struct sdrl_value *);
int prim_divide(struct sdrl_machine *, struct sdrl_value *);

int prim_equals(struct sdrl_machine *, struct sdrl_value *);
int prim_not_equals(struct sdrl_machine *, struct sdrl_value *);
int prim_less_than(struct sdrl_machine *, struct sdrl_value *);
int prim_greater_than(struct sdrl_machine *, struct sdrl_value *);
int prim_less_than_equals(struct sdrl_machine *, struct sdrl_value *);
int prim_greater_than_equals(struct sdrl_machine *, struct sdrl_value *);

int prim_print(struct sdrl_machine *, struct sdrl_value *);

#endif

