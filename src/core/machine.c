/*
 * Name:	machine.c
 * Version:	0.2
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>

#include "machine.h"
#include "expr.h"
#include "type.h"
#include "parse.h"
#include "value.h"
#include "bindings.h"
#include "globals.h"


static int sdrl_evaluate_expr_type(struct sdrl_machine *, struct sdrl_expr *, struct sdrl_value *);
static int sdrl_evaluate_form_type(struct sdrl_machine *, int (*)(struct sdrl_machine *, struct sdrl_expr *), struct sdrl_expr *);
static int sdrl_evaluate_builtin_type(struct sdrl_machine *, int (*)(struct sdrl_machine *, struct sdrl_value *), struct sdrl_value *);

/**
 * Create a machine for executing code
 */
struct sdrl_machine *sdrl_create_machine(void)
{
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->ret = NULL;
	mach->type_env = sdrl_create_environment(0, NULL);
	mach->env = sdrl_create_environment(0, NULL);
	mach->code = NULL;

	sdrl_bind_type(mach->type_env, "number", sdrl_make_type(0, SDRL_BT_NUMBER, NULL, NULL, NULL));
	sdrl_bind_type(mach->type_env, "string", sdrl_make_type(SDRL_VARIABLE_SIZE, SDRL_BT_STRING, NULL, NULL, NULL));
	sdrl_bind_type(mach->type_env, "list", sdrl_make_type(0, SDRL_BT_POINTER, NULL, (sdrl_duplicate_t) sdrl_duplicate_value, (sdrl_destroy_t) sdrl_destroy_value));
	sdrl_bind_type(mach->type_env, "expr", sdrl_make_type(0, SDRL_BT_POINTER, (sdrl_evaluate_t) sdrl_evaluate_expr_type, NULL, NULL));
	sdrl_bind_type(mach->type_env, "form", sdrl_make_type(0, SDRL_BT_POINTER | SDRL_TBF_PASS_EXPRS, (sdrl_evaluate_t) sdrl_evaluate_form_type, NULL, NULL));
	sdrl_bind_type(mach->type_env, "builtin", sdrl_make_type(0, SDRL_BT_POINTER, (sdrl_evaluate_t) sdrl_evaluate_builtin_type, NULL, NULL));

	return(mach);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(struct sdrl_machine *mach)
{
	sdrl_destroy_expr(mach->code);
	sdrl_destroy_environment(mach->env);
	sdrl_destroy_environment(mach->type_env);
	sdrl_destroy_value(mach->ret);
	free(mach);
	return(0);
}

/**
 * Evaluate all the exprs in the list, expr.
 */
int sdrl_evaluate_expr_list(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	int ret;
	struct sdrl_expr *cur;

	cur = expr;
	while (cur) {
		if (ret = sdrl_evaluate_expr(mach, cur))
			return(ret);
		cur = cur->next;
	}
	return(0);
}

/**
 * Evaluate recursively the single sdrl_expr and either return 0 with the result
 * stored in sdrl_machine.ret or return an error code.
 */
int sdrl_evaluate_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	sdrl_destroy_value(mach->ret);
	mach->ret = NULL;

	if (!expr)
		return(0);
	else if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "number"), (sdrl_data_t) expr->data.number, 0, NULL);
	else if (expr->type == SDRL_ET_STRING)
		mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "string"), (sdrl_data_t) expr->data.str, strlen(expr->data.str), NULL);
	else if (expr->type == SDRL_ET_CALL) {
		if (expr->data.expr->type != SDRL_ET_STRING)
			return(ERR_INVALID_FUNCTION);
		return(sdrl_evaluate_call(mach, expr->data.expr->data.str, expr->data.expr->next));
	}
	else
		return(ERR_INVALID_AST_TYPE);
	return(0);
}

/**
 * Resolve name and evaluate as a function.
 */
int sdrl_evaluate_call(struct sdrl_machine *mach, char *name, struct sdrl_expr *exprs)
{
	struct sdrl_value *func;

	if (!(func = sdrl_find_value(mach->env, name)))
		return(ERR_NOT_FOUND);
	return(sdrl_evaluate_value(mach, func, exprs));
}

/**
 * Evaluate expression as a function call
 */
int sdrl_evaluate_value(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_expr *exprs)
{
	int ret = 0;
	struct sdrl_value *args = NULL;

	if (func->type->evaluate) {
		if (sdrl_type_pass_exprs_m(func->type))
			ret = func->type->evaluate(mach, func->data.ptr, exprs);
		else {
			if (ret = sdrl_evaluate_params(mach, exprs, &args))
				return(ret);
			ret = func->type->evaluate(mach, func->data.ptr, args);
		}
	}
	else {
		if (exprs)
			return(ERR_INVALID_PARAMS);
		mach->ret = sdrl_duplicate_value(func);
	}

	return(ret);
}

/**
 * Evaluate all the exprs and build a list of the corresponding return values.
 */
int sdrl_evaluate_params(struct sdrl_machine *mach, struct sdrl_expr *exprs, struct sdrl_value **args)
{
	int ret;
	struct sdrl_expr *cur;

	cur = exprs;
	while (cur) {
		if (ret = sdrl_evaluate_expr(mach, cur))
			return(ret);
		sdrl_push_value(args, mach->ret);
		mach->ret = NULL;
		cur = cur->next;
	}
	return(0);
}

/*** Local Functions ***/

/**
 * Evaluate function for the cmdptr type.
 */
static int sdrl_evaluate_expr_type(struct sdrl_machine *mach, struct sdrl_expr *expr, struct sdrl_value *params)
{
	int ret = 0;
	struct sdrl_environment *parent;

	parent = mach->env;
	if (!(mach->env = sdrl_create_environment(0, parent))) {
		mach->env = parent;
		return(ERR_OUT_OF_MEMORY);
	}

	sdrl_bind_value(mach->env, "_", params);
	ret = sdrl_evaluate_expr_list(mach, expr);

	sdrl_destroy_environment(mach->env);
	mach->env = parent;
	return(ret);
}

/**
 * Evaluate function for the form type.
 */
static int sdrl_evaluate_form_type(struct sdrl_machine *mach, int (*func)(struct sdrl_machine *, struct sdrl_expr *), struct sdrl_expr *params)
{
	return(func(mach, params));
}

/**
 * Evaluate function for the builtin type.
 */
static int sdrl_evaluate_builtin_type(struct sdrl_machine *mach, int (*func)(struct sdrl_machine *, struct sdrl_value *), struct sdrl_value *args)
{
	return(func(mach, args));
}

