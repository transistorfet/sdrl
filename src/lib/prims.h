/*
 * Name:	prims.h
 * Version:	0.1
 * Description:	Primative Functions Header
 */


#ifndef _SDRL_LIB_PRIMS_H
#define _SDRL_LIB_PRIMS_H

#include <sdrl.h>

int load_prims(struct sdrl_machine *);

int prim_code(struct sdrl_machine *, struct sdrl_expr *);

int prim_set(struct sdrl_machine *, struct sdrl_value *);
int prim_if(struct sdrl_machine *, struct sdrl_value *);
int prim_list(struct sdrl_machine *, struct sdrl_value *);
int prim_unlist(struct sdrl_machine *, struct sdrl_value *);

int prim_resolve(struct sdrl_machine *, struct sdrl_value *);
int prim_array(struct sdrl_machine *, struct sdrl_value *);
int prim_head(struct sdrl_machine *, struct sdrl_value *);
int prim_tail(struct sdrl_machine *, struct sdrl_value *);
int prim_null(struct sdrl_machine *, struct sdrl_value *);

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

int prim_and(struct sdrl_machine *, struct sdrl_value *);
int prim_or(struct sdrl_machine *, struct sdrl_value *);
int prim_not(struct sdrl_machine *, struct sdrl_value *);

int prim_print(struct sdrl_machine *, struct sdrl_value *);

#endif

