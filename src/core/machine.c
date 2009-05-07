/*
 * Name:	machine.c
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sdrl/core/machine.h>
#include <sdrl/core/env.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/undef.h>
#include <sdrl/core/array.h>
#include <sdrl/core/error.h>
#include <sdrl/core/value.h>
#include <sdrl/core/utils.h>
#include <sdrl/core/events.h>
#include <sdrl/core/basetypes.h>
#include <sdrl/globals.h>

static int sdrl_push_expr_list_events(sdMachine *, sdExpr *, sdArray *);
static int sdrl_append_return(sdMachine *, sdArray *);

/**
 * Create a machine for executing code
 */
sdMachine *sdrl_make_machine(void)
{
	sdMachine *mach;

	if (!(mach = (sdMachine *) malloc(sizeof(sdMachine))))
		return(NULL);
	memset(mach, '\0', sizeof(sdMachine));
	if (!(mach->heap = sdrl_make_heap()) || !(mach->cont = sdrl_make_cont()))
		goto FAIL;
	if (!(mach->type_env = sdrl_make_env(mach, &sdEnvTypeDef, SDRL_BBF_CONSTANT, NULL)))
		goto FAIL;
	if (!(mach->global = sdrl_make_env(mach, &sdEnvTypeDef, 0, (sdrl_destroy_t) sdrl_destroy_value)))
		goto FAIL;
	mach->env = SDRL_INCREF(mach->global);
	sdrl_env_add(mach->type_env, "array", &sdArrayTypeDef);
	sdrl_env_add(mach->type_env, "number", &sdNumberTypeDef);
	sdrl_env_add(mach->type_env, "string", &sdStringTypeDef);
	sdrl_env_add(mach->type_env, "expr", &sdExprTypeDef);
	sdrl_env_add(mach->type_env, "env", &sdEnvTypeDef);
	sdrl_env_add(mach->type_env, "error", &sdErrorTypeDef);

	sdrl_env_add(mach->global, "undef", sdrl_get_undef());
	return(mach);

    FAIL:
	sdrl_machine_destroy(mach);
	return(NULL);
}

/**
 * Free the resources allocated to mach
 */
void sdrl_machine_destroy(sdMachine *mach)
{
	SDRL_DECREF(mach->ret);
	sdrl_cont_destroy(mach->cont);
	SDRL_DECREF(mach->env);
	sdrl_env_destroy(mach->global);
	sdrl_env_destroy(mach->type_env);
	sdrl_error_destroy(mach->error);
	sdrl_heap_destroy(mach->heap);
	free(mach);
}

/**
 * Evaluate all the exprs in the list, expr.
 */
int sdrl_eval(sdMachine *mach, sdExpr *expr)
{
	// TODO should we create a new cont first? such that every eval is it's own fresh cont?
	// TODO do you have to incref the expr first?
	sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(expr), mach->env);
	return(sdrl_run(mach, -1));
}

/**
 * Call a function within a machine.
 */
int sdrl_call(sdMachine *mach, sdValue *func_value, int num_args, ...)
{
	int i;
	int ret;
	va_list va;
	sdArray *args;
	sdValue *value;

	// TODO this whole function is just a quick hack for now.
	SDRL_DECREF(mach->ret);
	mach->ret = NULL;

	// Build up an array of the function and arguments
	va_start(va, num_args);
	if (!(args = sdrl_make_array(mach, &sdArrayTypeDef, num_args + 1)))
		return(sdrl_set_memory_error(mach));
	sdrl_array_set(args, 0, SDRL_INCREF(func_value));
	for (i = 1; i <= num_args; i++) {
		value = va_arg(va, sdValue *);
		sdrl_array_set(args, i, SDRL_INCREF(value));
	}

	i = sdrl_event_level(mach->cont);
	if ((ret = sdrl_evaluate_value(mach, args)))
		return(ret);
	return(sdrl_run(mach, i));
}

/**
 * Run the machine evaluating all events on the current continuation until
 * the stack pointer reaches the given level.  If the level given is -1, then
 * the machine runs until no events remain.
 */
int sdrl_run(sdMachine *mach, int level)
{
	int ret = 0;
	sdValue *arg;
	sdEvent *event;
	sdrl_event_t func;

	while (sdrl_event_level(mach->cont) > level) {
		// TODO can we do this nicer, without using an event ptr?
		event = sdrl_event_get_top(mach->cont);
		SDRL_DECREF(mach->env);
		mach->env = SDRL_INCREF(event->env);
		arg = SDRL_INCREF(event->arg);
		func = event->func;
		sdrl_event_pop(mach->cont);
		ret = func(mach, arg);
		SDRL_DECREF(arg);
		// TODO check the error and see if we should return or continue
		if (ret)
			return(ret);
	}

	return(0);
}

/**
 * Evaluate the list of exprs
 */
int sdrl_evaluate_expr_list(sdMachine *mach, sdExpr *expr)
{
	if (!expr)
		return(0);
	else if (expr->next)
		sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(expr->next), mach->env);
	return(sdrl_evaluate_expr_value(mach, expr));
}

/**
 * Evaluate the single sdExpr value and either return 0 with the result
 * stored in mach->ret or return an error code.
 */
int sdrl_evaluate_expr_value(sdMachine *mach, sdExpr *expr)
{
	int ret;
	sdValue *func;
	sdArray *args;

	SDRL_DECREF(mach->ret);
	mach->ret = NULL;

	if (!expr)
		return(0);

	mach->current_line = expr->line;
	if (expr->type == SDRL_ET_NUMBER)
		mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, expr->data.num));
	else if (expr->type == SDRL_ET_STRING || expr->type == SDRL_ET_IDENTIFIER)
		mach->ret = SDVALUE(sdrl_make_string(mach, &sdStringTypeDef, expr->data.str, strlen(expr->data.str)));
	else if (expr->type == SDRL_ET_CALL) {
		if (!expr->data.expr)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_FUNCTION, NULL));
		if (!(args = sdrl_make_array(mach, &sdArrayTypeDef, SDRL_DEFAULT_ARGS)))
			return(sdrl_set_memory_error(mach));
		mach->current_line = expr->data.expr->line;
		if (expr->data.expr->type == SDRL_ET_STRING || expr->data.expr->type == SDRL_ET_IDENTIFIER) {
			if (!(func = sdrl_env_find(mach->env, expr->data.expr->data.str))) {
				SDRL_DECREF(args);
				return(sdrl_set_not_found_error(mach));
			}
			else if (func->type->evaluate && SDRL_BF_IS_SET(func->type, SDRL_TBF_PASS_EXPRS)) {
				sdrl_array_push(args, SDRL_INCREF(func));
				sdrl_array_push(args, SDVALUE(SDRL_INCREF(expr->data.expr->next)));
				ret = func->type->evaluate(mach, args);
				SDRL_DECREF(args);
				return(ret);
			}
			else {
				sdrl_array_push(args, SDRL_INCREF(func));
				sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(args), mach->env);
				sdrl_push_expr_list_events(mach, expr->data.expr->next, args);
				SDRL_DECREF(args);
				return(0);
			}
		}
		else if (expr->data.expr->type == SDRL_ET_CALL) {
			sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(args), mach->env);
			sdrl_push_expr_list_events(mach, expr->data.expr, args);
			SDRL_DECREF(args);
			return(0);
		}
		else {
			SDRL_DECREF(args);
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_FUNCTION, NULL));
		}
	}
	else
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_AST_TYPE, NULL));
	return(0);
}

/**
 * Call the function in the first position of the array and set the result
 * in mach->ret or store a duplicate of the value if it is not executable
 */
int sdrl_evaluate_value(sdMachine *mach, sdArray *args)
{
	int ret = 0;
	sdValue *func;

	func = sdrl_array_get(args, 0);
	if (!func)
		ret = sdrl_set_not_found_error(mach);
	else if (func->type->evaluate) {
		if (SDRL_BF_IS_SET(func->type, SDRL_TBF_PASS_EXPRS))
			ret = sdrl_set_args_error(mach);
		else {
			// TODO what are the advantages and disadvantages of these two ways of execution?
			//sdrl_push_new_event(mach->cont, (sdrl_event_t) func->type->evaluate, SDRL_INCREF(args), mach->env);
			ret = func->type->evaluate(mach, args);
		}
	}
	else {
		// TODO what do we do if the function is not evaluatable?
		if (args->last > 0)
			ret = sdrl_set_args_error(mach);
		else
			mach->ret = sdrl_duplicate_value(mach, args->items[0]);
	}
	return(ret);
}


/*** Local Functions ***/

/**
 * Push events to evaluate all exprs in a list and push the results into the
 * given array.
 */
static int sdrl_push_expr_list_events(sdMachine *mach, sdExpr *exprs, sdArray *args)
{
	if (!exprs)
		return(-1);
	if (exprs->next)
		sdrl_push_expr_list_events(mach, exprs->next, args);
	sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_append_return, SDVALUE(args), mach->env);
	sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_value, SDVALUE(exprs), mach->env);
	return(0);
}

/**
 * Append the current value of mach->ret to the end of ret and store the whole
 * thing in mach->ret.
 */
static int sdrl_append_return(sdMachine *mach, sdArray *array)
{
	sdrl_array_push(array, mach->ret ? mach->ret : sdrl_get_undef());
	mach->ret = NULL;
	return(0);
}


