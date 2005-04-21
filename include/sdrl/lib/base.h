/*
 * Library Name:	base.h
 * Version:		0.1
 * Description:		Basic SDRL Functions Header
 */


#ifndef _SDRL_LIB_BASE_H
#define _SDRL_LIB_BASE_H

#include <sdrl/sdrl.h>

int sdrl_load_base(struct sdrl_machine *);

/*** Parsers ***/
struct sdrl_expr *sdrl_base_parse_input(struct sdrl_input *, void *);
struct sdrl_expr *sdrl_base_parse_expr(struct sdrl_input *, void *);

struct sdrl_expr *sdrl_lispy_parse_input(struct sdrl_input *, void *);
struct sdrl_expr *sdrl_lispy_parse_expr(struct sdrl_input *, void *);

/*** Types ***/
int sdrl_base_load_builtin_type(struct sdrl_machine *);
int sdrl_base_load_expr_type(struct sdrl_machine *);
int sdrl_base_load_form_type(struct sdrl_machine *);
int sdrl_base_load_list_type(struct sdrl_machine *);

/*** Forms ***/
int sdrl_base_code(struct sdrl_machine *, struct sdrl_expr *);

/*** Builtins ***/
int sdrl_base_set(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_if(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_list(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_unlist(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_resolve(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_array(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_head(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_tail(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_null(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_add(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_subtract(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_multiply(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_divide(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_not_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_less_than(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_greater_than(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_less_than_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_greater_than_equals(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_and(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_or(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_not(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_print(struct sdrl_machine *, struct sdrl_value *);

/*** Utilities ***/
int sdrl_base_display_expr(struct sdrl_expr *);

#endif

