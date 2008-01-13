/*
 * Library Name:	base.h
 * Description:		Basic SDRL Functions Header
 */


#ifndef _SDRL_LIB_BASE_H
#define _SDRL_LIB_BASE_H

#include <sdrl/sdrl.h>

int sdrl_load_base(struct sdrl_machine *);

/*** TYPES ***/

/** Environment Reference Type **/

struct sdrl_type *sdrl_base_make_env_type(struct sdrl_machine *);
struct sdrl_value *sdrl_base_create_env(struct sdrl_machine *, struct sdrl_type *type, struct sdrl_value *);

/** Expression Reference Type **/

#define SDRL_EXPR(ptr)		( (struct sdrl_expr *) (ptr) )
struct sdrl_type *sdrl_base_make_expr_type(struct sdrl_machine *);
struct sdrl_value *sdrl_base_create_expr(struct sdrl_machine *, struct sdrl_type *, struct sdrl_value *);
int sdrl_base_evaluate_expr(struct sdrl_machine *, struct sdrl_reference *, struct sdrl_value *);

/** Dynamic Block Type **/

struct sdrl_type *sdrl_base_make_dblock(struct sdrl_machine *);
int sdrl_base_evaluate_dblock(struct sdrl_machine *, struct sdrl_reference *, struct sdrl_value *);

/** C Function Type **/

struct sdrl_type *sdrl_base_make_func_type(struct sdrl_machine *);
int sdrl_base_evaluate_func(struct sdrl_machine *, struct sdrl_pointer *, struct sdrl_value *);

/** C Forms Type **/

struct sdrl_type *sdrl_base_make_form_type(struct sdrl_machine *);
int sdrl_base_evaluate_form(struct sdrl_machine *, struct sdrl_pointer *, struct sdrl_value *);

/** Lexical Block Type **/

// TODO should you make this inherit from expr which would allow for awesomeness
struct sdrl_lblock {
	struct sdrl_value value;
	struct sdrl_expr *code;
	struct sdrl_environment *env;
};

#define SDRL_LBLOCK(ptr)		( (struct sdrl_lblock *) (ptr) )
struct sdrl_type *sdrl_base_make_lblock_type(struct sdrl_machine *);
struct sdrl_value *sdrl_base_create_lblock(struct sdrl_machine *, struct sdrl_type *, struct sdrl_value *);
int sdrl_base_destroy_lblock(struct sdrl_lblock *);
struct sdrl_value *sdrl_base_duplicate_lblock(struct sdrl_machine *, struct sdrl_lblock *);
int sdrl_base_evaluate_lblock(struct sdrl_machine *, struct sdrl_lblock *, struct sdrl_value *);

/** List (Reference) Type **/

struct sdrl_type *sdrl_base_make_list_type(struct sdrl_machine *);
struct sdrl_value *sdrl_base_create_list(struct sdrl_machine *, struct sdrl_type *, struct sdrl_value *);






/*** Parsers ***/
struct sdrl_expr *sdrl_base_parse_lambda_input(struct sdrl_input *, void *);

/*** Types ***/
struct sdrl_type *sdrl_base_make_dblock_type(struct sdrl_machine *);
struct sdrl_type *sdrl_base_make_expr_type(struct sdrl_machine *);
struct sdrl_type *sdrl_base_make_form_type(struct sdrl_machine *);
struct sdrl_type *sdrl_base_make_func_type(struct sdrl_machine *);
struct sdrl_type *sdrl_base_make_lblock_type(struct sdrl_machine *);
struct sdrl_type *sdrl_base_make_list_type(struct sdrl_machine *);

/*** Forms ***/
int sdrl_base_code(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_base_dblock(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_base_lblock(struct sdrl_machine *, struct sdrl_expr *);

/*** Functions ***/
int sdrl_base_set(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_if(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_list(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_unlist(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_resolve(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_lookup(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_array(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_head(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_tail(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_null(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_add(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_subtract(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_multiply(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_divide(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_modulo(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_not_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_less_than(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_greater_than(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_less_than_equals(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_greater_than_equals(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_and(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_or(struct sdrl_machine *, struct sdrl_value *);
int sdrl_base_not(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_defenv(struct sdrl_machine *, struct sdrl_value *);

int sdrl_base_print(struct sdrl_machine *, struct sdrl_value *);

/*** Utilities ***/
int sdrl_base_display_error(struct sdrl_error *);
int sdrl_base_display_expr(struct sdrl_expr *);
struct sdrl_expr *sdrl_base_parse_file(const char *, sdrl_parser_t, void *);
struct sdrl_expr *sdrl_base_parse_string(const char *, int, sdrl_parser_t, void *);

#endif

