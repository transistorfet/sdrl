/*
 * Name:	sdrl.c
 * Version:	0.1
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>

#include "sdrl.h"
#include "expr.h"
#include "type.h"
#include "parse.h"
#include "value.h"
#include "bindings.h"
#include "types.h"


static int sdrl_evaluate_funcptr(struct sdrl_machine *, int (*)(struct sdrl_machine *));

/**
 * Create a machine for executing code
 */
struct sdrl_machine *sdrl_create_machine(void)
{
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->ret = NULL;
	mach->opr = NULL;
	mach->type_env = sdrl_create_environment(0, NULL);
	mach->env = sdrl_create_environment(0, NULL);
	mach->code = NULL;

	sdrl_bind_type(mach->type_env, "number", sdrl_make_type(0, 0, NULL, NULL));
	sdrl_bind_type(mach->type_env, "string", sdrl_make_type(SDRL_VARIABLE_SIZE, 0, NULL, NULL));
	sdrl_bind_type(mach->type_env, "expr", sdrl_make_type(0, 0, (sdrl_evaluate_t) sdrl_evaluate_expr, NULL));
	sdrl_bind_type(mach->type_env, "funcptr", sdrl_make_type(0, 0, (sdrl_evaluate_t) sdrl_evaluate_funcptr, NULL));

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
	sdrl_destroy_value(mach->opr);
	free(mach);
	return(0);
}

/**
 * Evaluate all the exprs in the list, expr.
 */
int sdrl_evaluate_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	int ret;
	struct sdrl_expr *cur;

	cur = expr;
	while (cur) {
		if (ret = sdrl_evaluate_single_expr(mach, cur))
			return(ret);
		cur = cur->next;
	}
	return(0);
}

/**
 * Evaluate recursively the single sdrl_expr and either return 0 with the result
 * stored in sdrl_machine.ret or return an error code.
 */
int sdrl_evaluate_single_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	if (!expr)
		return(0);
	else if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "number"), (sdrl_data_t) expr->data.number, 0, NULL);
	else if (expr->type == SDRL_ET_NAME)
		mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "string"), (sdrl_data_t) expr->data.name, strlen(expr->data.name), NULL);
	else if (expr->type == SDRL_ET_CALL) {
		if (expr->data.expr->type != SDRL_ET_NAME)
			return(ERR_INVALID_FUNCTION);
		return(sdrl_evaluate_call(mach, expr->data.expr->data.name, expr->data.expr->next));
	}
	else
		return(ERR_INVALID_AST_TYPE);
	return(0);
}

/**
 * Evaluate expression as a function call
 */
int sdrl_evaluate_call(struct sdrl_machine *mach, char *name, struct sdrl_expr *expr)
{
	int ret;
	struct sdrl_expr *cur;
	struct sdrl_value *func;
	struct sdrl_value *args = NULL;
	struct sdrl_environment *parent;

	if (!(func = sdrl_find_value(mach->env, name)))
		return(ERR_NOT_FOUND);
	parent = mach->env;
	if (!(mach->env = sdrl_create_environment(0, parent))) {
		mach->env = parent;
		return(ERR_OUT_OF_MEMORY);
	}

	sdrl_destroy_value(mach->ret);
	sdrl_destroy_value(mach->opr);
	mach->ret = mach->opr = NULL;

	if (func->type->evaluate) {
		if (sdrl_type_eval_params_m(func->type)) {
			cur = expr;
			while (cur) {
				sdrl_evaluate_single_expr(mach, expr);
				sdrl_push_value(&args, mach->ret);
				mach->ret = NULL;
				cur = cur->next;
			}
			mach->opr = args;
			func->type->evaluate(mach, func->data.ptr);
		}
		else {
			mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "expr"), (sdrl_data_t) (void *) expr, 0, NULL);
			func->type->evaluate(mach, func->data.ptr);
		}
	}
	else {
		if (expr)
			return(ERR_INVALID_PARAMS);
		mach->ret = sdrl_duplicate_value(func);
	}

	sdrl_destroy_environment(mach->env);
	mach->env = parent;
	return(ret);
}

/*** Local Functions ***/

/**
 * Evaluate function for the funcptr type.
 */
static int sdrl_evaluate_funcptr(struct sdrl_machine *mach, int (*func)(struct sdrl_machine *))
{
	return(func(mach));
}


