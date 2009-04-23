/*
 * Name:	machine.c
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/machine.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/type.h>
#include <sdrl/core/array.h>
#include <sdrl/core/error.h>
#include <sdrl/core/value.h>
#include <sdrl/core/events.h>
#include <sdrl/core/bindings.h>
#include <sdrl/core/basetypes.h>
#include <sdrl/globals.h>

static int sdrl_push_expr_list_events(sdMachine *, sdExpr *, sdArray *);
static int sdrl_append_return(sdMachine *, sdArray *);

/**
 * Create a machine for executing code
 */
sdMachine *sdrl_create_machine(void)
{
	sdType *type;
	sdMachine *mach;

	if (!(mach = (sdMachine *) malloc(sizeof(sdMachine))))
		return(NULL);
	memset(mach, '\0', sizeof(sdMachine));
	if (!(mach->heap = sdrl_create_heap()) || !(mach->cont = sdrl_create_continuation()))
		goto FAIL;
	// TODO clean this up
	if (!(type = sdrl_make_environment_type())
	    || !(mach->type_env = sdrl_make_environment(mach->heap, type, SDRL_BBF_CONSTANT, (sdrl_destroy_t) sdrl_destroy_type))) {
		if (type)
			sdrl_destroy_type(type);
		goto FAIL;
	}
	sdrl_add_binding(mach->type_env, "env", type);
	if (!(mach->global = sdrl_make_environment(mach->heap, type, 0, (sdrl_destroy_t) sdrl_destroy_value)))
		goto FAIL;
	mach->env = SDRL_INCREF(mach->global);

	sdrl_add_binding(mach->type_env, "number", sdrl_make_number_type());
	sdrl_add_binding(mach->type_env, "string", sdrl_make_string_type());
	sdrl_add_binding(mach->type_env, "expr", sdrl_make_expr_type());
	sdrl_add_binding(mach->type_env, "array", sdrl_make_array_type());

	return(mach);

    FAIL:
	sdrl_destroy_machine(mach);
	return(NULL);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(sdMachine *mach)
{
	sdrl_destroy_value(mach->ret);
	sdrl_destroy_continuation(mach->cont);
	SDRL_DECREF(mach->env);
	sdrl_destroy_environment(mach->global);
	sdrl_destroy_environment(mach->type_env);
	sdrl_destroy_error(mach->error);
	sdrl_destroy_heap(mach->heap);
	free(mach);
	return(0);
}

/**
 * Evaluate all the exprs in the list, expr.
 */
int sdrl_evaluate(sdMachine *mach, sdExpr *expr)
{
	int ret = 0;
	sdEvent *event;

	if (!(event = sdrl_make_event((sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(expr), mach->env)))
		return(sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	do {
		SDRL_DECREF(mach->env);
		mach->env = SDRL_INCREF(event->env);
		ret = event->func(mach, event->arg);
		sdrl_destroy_event(event);
		// TODO check the error and see if we should return or continue
		if (ret)
			return(ret);
		event = sdrl_pop_event(mach->cont);
	} while (event);

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
		sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(expr->next), mach->env);
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
		mach->ret = sdrl_make_number(mach->heap, sdrl_find_binding(mach->type_env, "number"), expr->data.num);
	else if (expr->type == SDRL_ET_STRING || expr->type == SDRL_ET_IDENTIFIER)
		mach->ret = sdrl_make_string(mach->heap, sdrl_find_binding(mach->type_env, "string"), expr->data.str, strlen(expr->data.str));
	else if (expr->type == SDRL_ET_CALL) {
		if (!expr->data.expr)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_FUNCTION, NULL));
		if (!(args = sdrl_make_array(mach->heap, sdrl_find_binding(mach->type_env, "array"), SDRL_DEFAULT_ARGS)))
			return(sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
		mach->current_line = expr->data.expr->line;
		if (expr->data.expr->type == SDRL_ET_STRING || expr->data.expr->type == SDRL_ET_IDENTIFIER) {
			if (!(func = sdrl_find_binding(mach->env, expr->data.expr->data.str))) {
				SDRL_DECREF(args);
				return(sdrl_set_error(mach, SDRL_ES_MEDIUM, SDRL_ERR_NOT_FOUND, NULL));
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
				sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(args), mach->env);
				sdrl_push_expr_list_events(mach, expr->data.expr->next, args);
				SDRL_DECREF(args);
				return(0);
			}
		}
		else if (expr->data.expr->type == SDRL_ET_CALL) {
			sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(args), mach->env);
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
		ret = sdrl_set_error(mach, SDRL_ES_LOW, SDRL_ERR_NOT_FOUND, NULL);
	else if (func->type->evaluate) {
		if (SDRL_BF_IS_SET(func->type, SDRL_TBF_PASS_EXPRS))
			ret = sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		else {
			// TODO what are the advantages and disadvantages of these two ways of execution?
			//sdrl_push_new_event(mach->cont, (sdrl_event_t) func->type->evaluate, SDRL_INCREF(args), mach->env);
			ret = func->type->evaluate(mach, args);
		}
	}
	else {
		// TODO what do we do if the function is not evaluatable?
		if (args->last > 0)
			ret = sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
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
	sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_append_return, SDVALUE(args), mach->env);
	sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_value, SDVALUE(exprs), mach->env);
	return(0);
}

/**
 * Append the current value of mach->ret to the end of ret and store the whole
 * thing in mach->ret.
 */
static int sdrl_append_return(sdMachine *mach, sdArray *array)
{
	sdrl_array_push(array, mach->ret);
	mach->ret = NULL;
	return(0);
}


