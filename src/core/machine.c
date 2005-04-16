/*
 * Name:	machine.c
 * Version:	0.2
 * Description:	SDRL Interpreter
 */


#include <stdlib.h>
#include <string.h>

#include "machine.h"
#include "heap.h"
#include "expr.h"
#include "type.h"
#include "parse.h"
#include "value.h"
#include "events.h"
#include "bindings.h"
#include "globals.h"


static int sdrl_evaluate_expr_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);
static int sdrl_evaluate_form_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_expr *);
static int sdrl_evaluate_builtin_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

/**
 * Create a machine for executing code
 */
struct sdrl_machine *sdrl_create_machine(void)
{
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->ret = NULL;
	mach->heap = sdrl_create_heap(0, 0);
	mach->cont = sdrl_create_continuation();

	if (!mach->heap || !mach->cont) {
		sdrl_destroy_machine(mach);
		return(NULL);
	}

	mach->type_env = sdrl_create_environment(SDRL_BBF_CONSTANT, mach->heap, (sdrl_destroy_t) sdrl_destroy_type);
	mach->global = sdrl_create_environment(0, mach->heap, (sdrl_destroy_t) sdrl_destroy_value);
	mach->env = mach->global;

	if (!mach->type_env || !mach->global) {
		sdrl_destroy_machine(mach);
		return(NULL);
	}

	sdrl_add_binding(mach->type_env, "number", sdrl_make_type(mach->heap, 0, SDRL_BT_NUMBER, NULL, NULL, NULL));
	sdrl_add_binding(mach->type_env, "string", sdrl_make_type(mach->heap, SDRL_VARIABLE_SIZE, SDRL_BT_STRING, NULL, NULL, NULL));
	sdrl_add_binding(mach->type_env, "list", sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, NULL, (sdrl_duplicate_t) sdrl_duplicate_value, (sdrl_destroy_t) sdrl_destroy_value));
	sdrl_add_binding(mach->type_env, "expr", sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, (sdrl_evaluate_t) sdrl_evaluate_expr_type, NULL, NULL));
	sdrl_add_binding(mach->type_env, "form", sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER | SDRL_TBF_PASS_EXPRS, (sdrl_evaluate_t) sdrl_evaluate_form_type, NULL, NULL));
	sdrl_add_binding(mach->type_env, "builtin", sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, (sdrl_evaluate_t) sdrl_evaluate_builtin_type, NULL, NULL));

	return(mach);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(struct sdrl_machine *mach)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	sdrl_destroy_continuation(mach->cont);
	sdrl_retract_environment(mach->global);
	sdrl_destroy_environment(mach->type_env);
	sdrl_destroy_heap(mach->heap);
	free(mach);
	return(0);
}

/**
 * Evaluate all the exprs in the list, expr.
 */
int sdrl_evaluate(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	int ret = 0;
	struct sdrl_event *event;

	if (!(event = sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr, mach->env)))
		return(ERR_OUT_OF_MEMORY);
	do {
		if (ret = sdrl_evaluate_event(mach, event))
			return(ret);
		event = sdrl_pop_event(mach->cont);
	} while (event);

	return(0);
}

/**
 * Evaluate the continuation event by calling the event function
 */
int sdrl_evaluate_event(struct sdrl_machine *mach, struct sdrl_event *event)
{
	int ret = 0;
	struct sdrl_value *value;

	mach->env = event->env;
	if (sdrl_use_ret_m(event)) {
		value = mach->ret;
		mach->ret = NULL;
		ret = event->func(mach, event->param, value);
	}
	else 
		ret = event->func(mach, event->param);
	sdrl_destroy_event(event);
	return(ret);
}

/*
 * Evaluate the list of exprs
 */
int sdrl_evaluate_expr_list(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	if (!expr)
		return(0);
	else if (expr->next)
		sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->next, mach->env));
	return(sdrl_evaluate_expr(mach, expr));
}

/**
 * Evaluate the single sdrl_expr and either return 0 with the result
 * stored in sdrl_machine.ret or return an error code.
 */
int sdrl_evaluate_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	struct sdrl_value *func;

	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;

	if (!expr)
		return(0);
	else if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "number"), (sdrl_data_t) expr->data.number, 0, NULL);
	else if (expr->type == SDRL_ET_STRING)
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "string"), (sdrl_data_t) expr->data.str, strlen(expr->data.str), NULL);
	else if (expr->type == SDRL_ET_CALL) {
		if (expr->data.expr->type == SDRL_ET_STRING) {
			if (!(func = sdrl_find_binding(mach->env, expr->data.expr->data.str)))
				return(ERR_NOT_FOUND);
			else if (func->type->evaluate && sdrl_type_pass_exprs_m(func->type))
				return(func->type->evaluate(mach, func, expr->data.expr->next));
			else {
				sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_call_value, func, mach->env));
				sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_params, expr->data.expr->next, mach->env));
				return(0);
			}
		}
		else
			return(ERR_INVALID_FUNCTION);
	}
	else
		return(ERR_INVALID_AST_TYPE);
	return(0);
}

/**
 * Call the function stored in the value and set the result
 * in mach->ret or store a duplicate of the value if it is not executable
 */
int sdrl_call_value(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	if (!func)
		return(ERR_NOT_FOUND);
	else if (func->type->evaluate) {
		if (sdrl_type_pass_exprs_m(func->type))
			return(ERR_INVALID_PARAMS);
		else {
			mach->ret = args;
			sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) func->type->evaluate, func, mach->env));
//			func->type->evaluate(mach, func, args);
		}
	}
	else {
		if (args)
			return(ERR_INVALID_PARAMS);
//		mach->ret = sdrl_make_reference_m(func);
		mach->ret = sdrl_duplicate_value(mach->heap, func);
	}
	return(0);
}

/**
 * Evaluate all the exprs and build a list of the corresponding return values.
 */
int sdrl_evaluate_params(struct sdrl_machine *mach, struct sdrl_expr *exprs)
{
	if (!exprs)
		return(0);
	if (exprs->next)
		sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_params, exprs->next, mach->env));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_merge_return, mach->ret, mach->env));
	mach->ret = NULL;
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr, exprs, mach->env));
	return(0);
}

/**
 * Append the current value of mach->ret to the end of ret and store the whole
 * thing in mach->ret.
 */
int sdrl_merge_return(struct sdrl_machine *mach, struct sdrl_value *ret)
{
	sdrl_push_value(&ret, mach->ret);
	mach->ret = ret;
	return(0);
}


/*** Local Functions ***/

/**
 * Evaluate function for the cmdptr type.
 */
static int sdrl_evaluate_expr_type(struct sdrl_machine *mach, struct sdrl_value *expr, struct sdrl_value *params)
{
	struct sdrl_environment *env;

	if (!(env = sdrl_extend_environment(mach->env)))
		return(ERR_OUT_OF_MEMORY);
	sdrl_add_binding(env, "_", params);
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->data.ptr, env));
	sdrl_destroy_reference_m(env, sdrl_retract_environment);
	return(0);
}

/**
 * Evaluate function for the form type.
 */
static int sdrl_evaluate_form_type(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_expr *params)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;
	return(((int (*)(struct sdrl_machine *, struct sdrl_expr *)) func->data.ptr)(mach, params));
}

/**
 * Evaluate function for the builtin type.
 */
static int sdrl_evaluate_builtin_type(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;
	return(((int (*)(struct sdrl_machine *, struct sdrl_value *)) func->data.ptr)(mach, args));
}


