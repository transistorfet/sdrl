/*
 * Library Name:	base.h
 * Description:		Basic SDRL Functions Header
 */


#ifndef _SDRL_LIB_BASE_H
#define _SDRL_LIB_BASE_H

#include <sdrl/sdrl.h>

int sdrl_load_base(sdMachine *);

/*** TYPES ***/

/** Environment Reference Type **/

sdType *sdrl_base_make_env_type(sdMachine *);
sdValue *sdrl_base_create_env(sdMachine *, sdType *type, sdValue *);

/** Expression Reference Type **/

#define SDEXPR(ptr)		( (sdExpr *) (ptr) )
sdType *sdrl_base_make_expr_type(sdMachine *);
sdValue *sdrl_base_create_expr(sdMachine *, sdType *, sdValue *);
int sdrl_base_evaluate_expr(sdMachine *, sdReference *, sdValue *);

/** Dynamic Block Type **/

sdType *sdrl_base_make_dynblock(sdMachine *);
int sdrl_base_evaluate_dynblock(sdMachine *, sdReference *, sdValue *);

/** C Function Type **/

sdType *sdrl_base_make_func_type(sdMachine *);
int sdrl_base_evaluate_func(sdMachine *, sdPointer *, sdValue *);

/** C Forms Type **/

sdType *sdrl_base_make_form_type(sdMachine *);
int sdrl_base_evaluate_form(sdMachine *, sdPointer *, sdValue *);

/** Lexical Block Type **/

// TODO should you make this inherit from expr which would allow for awesomeness
typedef struct sdLexBlock sdLexBlock;
struct sdLexBlock {
	sdValue value;
	sdExpr *code;
	sdEnv *env;
};

#define SDLEXBLOCK(ptr)		( (sdLexBlock *) (ptr) )
sdType *sdrl_base_make_lexblock_type(sdMachine *);
sdValue *sdrl_base_create_lexblock(sdMachine *, sdType *, sdValue *);
int sdrl_base_destroy_lexblock(sdLexBlock *);
sdValue *sdrl_base_duplicate_lexblock(sdMachine *, sdLexBlock *);
int sdrl_base_evaluate_lexblock(sdMachine *, sdLexBlock *, sdValue *);

/** List (Reference) Type **/

sdType *sdrl_base_make_list_type(sdMachine *);
sdValue *sdrl_base_create_list(sdMachine *, sdType *, sdValue *);






/*** Parsers ***/
sdExpr *sdrl_base_parse_lambda_input(sdMachine *, sdInput *);
sdExpr *sdrl_base_parse_lispy_input(sdMachine *, sdInput *);

/*** Types ***/
sdType *sdrl_base_make_dynblock_type(sdMachine *);
sdType *sdrl_base_make_expr_type(sdMachine *);
sdType *sdrl_base_make_form_type(sdMachine *);
sdType *sdrl_base_make_func_type(sdMachine *);
sdType *sdrl_base_make_lexblock_type(sdMachine *);
sdType *sdrl_base_make_list_type(sdMachine *);

/*** Forms ***/
int sdrl_base_code(sdMachine *, sdExpr *);
int sdrl_base_dynblock(sdMachine *, sdExpr *);
int sdrl_base_lexblock(sdMachine *, sdExpr *);

/*** Functions ***/
int sdrl_base_set(sdMachine *, sdValue *);
int sdrl_base_setlist(sdMachine *, sdValue *);
int sdrl_base_if(sdMachine *, sdValue *);
int sdrl_base_list(sdMachine *, sdValue *);
int sdrl_base_unlist(sdMachine *, sdValue *);

int sdrl_base_resolve(sdMachine *, sdValue *);
int sdrl_base_lookup(sdMachine *, sdValue *);
int sdrl_base_array(sdMachine *, sdValue *);
int sdrl_base_head(sdMachine *, sdValue *);
int sdrl_base_tail(sdMachine *, sdValue *);
int sdrl_base_null(sdMachine *, sdValue *);

int sdrl_base_add(sdMachine *, sdValue *);
int sdrl_base_subtract(sdMachine *, sdValue *);
int sdrl_base_multiply(sdMachine *, sdValue *);
int sdrl_base_divide(sdMachine *, sdValue *);
int sdrl_base_modulo(sdMachine *, sdValue *);

int sdrl_base_equals(sdMachine *, sdValue *);
int sdrl_base_not_equals(sdMachine *, sdValue *);
int sdrl_base_less_than(sdMachine *, sdValue *);
int sdrl_base_greater_than(sdMachine *, sdValue *);
int sdrl_base_less_than_equals(sdMachine *, sdValue *);
int sdrl_base_greater_than_equals(sdMachine *, sdValue *);

int sdrl_base_and(sdMachine *, sdValue *);
int sdrl_base_or(sdMachine *, sdValue *);
int sdrl_base_not(sdMachine *, sdValue *);

int sdrl_base_defenv(sdMachine *, sdValue *);

int sdrl_base_print(sdMachine *, sdValue *);

/*** Utilities ***/
int sdrl_base_display_error(sdError *);
int sdrl_base_display_expr(sdExpr *);
sdExpr *sdrl_base_parse_file(sdMachine *, sdrl_parser_t, const char *);
sdExpr *sdrl_base_parse_string(sdMachine *, sdrl_parser_t, const char *, int);

#endif

