/*
 * Name:	sdrl.c
 * Version:	0.1
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>

#include "sdrl.h"
#include "expr.h"
#include "parse.h"
#include "value.h"
#include "bindings.h"
#include "types.h"


static int sdrl_returnvalue(struct sdrl_machine *, char *);
static char sdrl_getinput(struct sdrl_machine *, char *);

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
	mach->env = sdrl_create_environment(0, NULL);
	mach->code = NULL;

	return(mach);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(struct sdrl_machine *mach)
{
	sdrl_destroy_expr(mach->code);
	sdrl_destroy_environment(mach->env);
	sdrl_destroy_value(mach->ret);
	sdrl_destroy_value(mach->opr);
	free(mach);
	return(0);
}

/**
 * Evaluate recursively the sdrl_expr and either return 0 with the result
 * stored in sdrl_machine.ret or return an error code.
 */
int sdrl_evaluate_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	int err;
	struct sdrl_expr *cur;
	struct sdrl_value *args = NULL;

	if (!expr)
		return(0);
	else if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_value(SDRL_VT_NUMBER, (sdrl_data_t) expr->data.number, 0, NULL);
	else if (expr->type == SDRL_ET_NAME)
		mach->ret = sdrl_make_value(SDRL_VT_STRING, (sdrl_data_t) expr->data.name, strlen(expr->data.name), NULL);
	else if (expr->type == SDRL_ET_CALL) {
		// TODO check to see if the function should be called using params or args
		cur = expr->data.expr;
		while (cur) {
			if (err = sdrl_evaluate_expr(mach, cur))
				return(err);
			sdrl_push_value(&args, mach->ret);
			cur = cur->next;
		}
		if (args->type != SDRL_VT_STRING)
			return(ERR_INVALID_FUNCTION);
		if (err = sdrl_evaluate_function(mach, args->data.str, args->next))
			return(err);
	}
	else
		return(ERR_INVALID_AST_TYPE);
	return(0);
}

/**
 * Lookup func (must be type expr) and evaluate as a function call
 */
int sdrl_evaluate_function(struct sdrl_machine *mach, char *func, struct sdrl_value *params)
{
	int ret;
	struct sdrl_value *bind;
	struct sdrl_environment *parent;

	if (!(bind = sdrl_find_binding(mach->env, func)))
		return(ERR_NOT_FOUND);
	parent = mach->env;
	if (!(mach->env = sdrl_create_environment(0, parent))) {
		mach->env = parent;
		return(ERR_OUT_OF_MEMORY);
	}

	if (bind->type == SDRL_VT_EXPR)
		ret = sdrl_evaluate_expr(mach, bind->data.expr);
	else if (bind->type == SDRL_VT_FUNCPTR)
		ret = ((int (*)(struct sdrl_machine *)) bind->data.str)(mach);
	else
		ret = ERR_INVALID_TYPE;

	sdrl_destroy_environment(mach->env);
	mach->env = parent;
	return(ret);
}


