/*
 * Library Name:	base.h
 * Description:		Basic SDRL Functions Header
 */


#ifndef _SDRL_LIB_BASE_H
#define _SDRL_LIB_BASE_H

#include <sdrl/sdrl.h>

#include <sdrl/lib/base/form.h>
#include <sdrl/lib/base/func.h>
#include <sdrl/lib/base/list.h>
#include <sdrl/lib/base/dynblock.h>
#include <sdrl/lib/base/lexblock.h>

int sdrl_load_base(sdMachine *, sdEnv *);

/*** Parsers ***/
sdExpr *sdrl_base_parse_lambda_input(sdMachine *, sdInput *);
sdExpr *sdrl_base_parse_lispy_input(sdMachine *, sdInput *);

/*** Forms ***/
int sdrl_base_code(sdMachine *, sdArray *);
int sdrl_base_dynblock(sdMachine *, sdArray *);
int sdrl_base_lexblock(sdMachine *, sdArray *);

/*** Functions ***/

int sdrl_base_add(sdMachine *, sdArray *);
int sdrl_base_and(sdMachine *, sdArray *);
int sdrl_base_array(sdMachine *, sdArray *);
int sdrl_base_defenv(sdMachine *, sdArray *);
int sdrl_base_deref(sdMachine *, sdArray *);
int sdrl_base_divide(sdMachine *, sdArray *);
int sdrl_base_envget(sdMachine *, sdArray *);
int sdrl_base_envset(sdMachine *, sdArray *);
int sdrl_base_equals(sdMachine *, sdArray *);
int sdrl_base_greater_than(sdMachine *, sdArray *);
int sdrl_base_greater_than_equals(sdMachine *, sdArray *);
int sdrl_base_head(sdMachine *, sdArray *);
int sdrl_base_if(sdMachine *, sdArray *);
int sdrl_base_less_than(sdMachine *, sdArray *);
int sdrl_base_less_than_equals(sdMachine *, sdArray *);
int sdrl_base_list(sdMachine *, sdArray *);
int sdrl_base_modulo(sdMachine *, sdArray *);
int sdrl_base_multiply(sdMachine *, sdArray *);
int sdrl_base_namespace(sdMachine *, sdArray *);
int sdrl_base_new(sdMachine *, sdArray *);
int sdrl_base_not(sdMachine *, sdArray *);
int sdrl_base_not_equals(sdMachine *, sdArray *);
int sdrl_base_null(sdMachine *, sdArray *);
int sdrl_base_or(sdMachine *, sdArray *);
int sdrl_base_print(sdMachine *, sdArray *);
int sdrl_base_pop(sdMachine *, sdArray *);
int sdrl_base_push(sdMachine *, sdArray *);
int sdrl_base_ref(sdMachine *, sdArray *);
int sdrl_base_resolve(sdMachine *, sdArray *);
int sdrl_base_set(sdMachine *, sdArray *);
int sdrl_base_setlist(sdMachine *, sdArray *);
int sdrl_base_shift(sdMachine *, sdArray *);
int sdrl_base_subtract(sdMachine *, sdArray *);
int sdrl_base_tail(sdMachine *, sdArray *);
int sdrl_base_unshift(sdMachine *, sdArray *);

/*** Utilities ***/
int sdrl_base_display_error(sdError *);
int sdrl_base_display_expr(sdExpr *);
sdExpr *sdrl_base_parse_file(sdMachine *, const char *);
sdExpr *sdrl_base_parse_string(sdMachine *, const char *, int);

#endif

