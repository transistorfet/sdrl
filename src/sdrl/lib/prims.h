/*
 * Name:	prims.h
 * Version:	0.1
 * Description:	Primative Functions Header
 */


#ifndef PRIMS_H
#define PRIMS_H

#include <sdrl.h>

int prim_initialize(struct sdrl_machine *);

int prim_code(struct sdrl_machine *, struct sdrl_expr *);
int prim_define(struct sdrl_machine *, struct sdrl_value *);
int prim_resolve(struct sdrl_machine *, struct sdrl_value *);
int prim_add(struct sdrl_machine *, struct sdrl_value *);
int prim_subtract(struct sdrl_machine *, struct sdrl_value *);
int prim_multiply(struct sdrl_machine *, struct sdrl_value *);
int prim_divide(struct sdrl_machine *, struct sdrl_value *);

int prim_print(struct sdrl_machine *, struct sdrl_value *);

/*
int prim_add(struct sdrl_machine *);
int prim_subtract(struct sdrl_machine *);
int prim_multiply(struct sdrl_machine *);
int prim_divide(struct sdrl_machine *);
int prim_bitand(struct sdrl_machine *);
int prim_bitor(struct sdrl_machine *);
int prim_bitnot(struct sdrl_machine *);
int prim_logicaland(struct sdrl_machine *);
int prim_logicalor(struct sdrl_machine *);
int prim_logicalnot(struct sdrl_machine *);
int prim_lessthan(struct sdrl_machine *);
int prim_lessthanequal(struct sdrl_machine *);
int prim_greaterthan(struct sdrl_machine *);
int prim_greaterthanequal(struct sdrl_machine *);
int prim_equals(struct sdrl_machine *);
int prim_stringappend(struct sdrl_machine *);
int prim_listappend(struct sdrl_machine *);
int prim_value(struct sdrl_machine *);
int prim_list(struct sdrl_machine *);
int prim_begin(struct sdrl_machine *);
int prim_evaluate(struct sdrl_machine *);
int prim_evaluaterepeat(struct sdrl_machine *);
int prim_stoprepeat(struct sdrl_machine *);
int prim_conditional(struct sdrl_machine *);
int prim_multiconditional(struct sdrl_machine *);
int prim_defineenvironment(struct sdrl_machine *);
int prim_define(struct sdrl_machine *);
int prim_definereplace(struct sdrl_machine *);
*/

#endif

