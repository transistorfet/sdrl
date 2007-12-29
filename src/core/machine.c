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
#include <sdrl/core/error.h>
#include <sdrl/core/value.h>
#include <sdrl/core/events.h>
#include <sdrl/core/bindings.h>
#include <sdrl/globals.h>

#define RETURN_FATAL_ERROR(mach, err)			\
	{						\
		sdrl_destroy_machine(mach);		\
		sdrl_set_error(0, err, NULL);		\
		return(NULL);				\
	}

/**
 * Create a machine for executing code
 */
struct sdrl_machine *sdrl_create_machine(void)
{
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->ret = NULL;
	mach->error = NULL;
	if (!(mach->heap = sdrl_create_heap()))
		RETURN_FATAL_ERROR(mach, SDRL_ERR_OUT_OF_MEMORY);
	if (!(mach->cont = sdrl_create_continuation()))
		RETURN_FATAL_ERROR(mach, SDRL_ERR_OUT_OF_MEMORY);
	if (!(mach->type_env = sdrl_create_environment(SDRL_BBF_CONSTANT, mach->heap, (sdrl_destroy_t) sdrl_destroy_type)))
		RETURN_FATAL_ERROR(mach, SDRL_ERR_OUT_OF_MEMORY);
	if (!(mach->global = sdrl_create_environment(0, mach->heap, (sdrl_destroy_t) sdrl_destroy_value)))
		RETURN_FATAL_ERROR(mach, SDRL_ERR_OUT_OF_MEMORY);
	mach->env = SDRL_MAKE_REFERENCE(mach->global);

	sdrl_add_binding(mach->type_env, "number", sdrl_make_type(mach->heap, 0, SDRL_BT_NUMBER, NULL, NULL, NULL, NULL));
	sdrl_add_binding(mach->type_env, "string", sdrl_make_type(mach->heap, SDRL_VARIABLE_SIZE, SDRL_BT_STRING, NULL, NULL, NULL, NULL));

	return(mach);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(struct sdrl_machine *mach)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	sdrl_destroy_continuation(mach->cont);
	SDRL_DESTROY_REFERENCE(mach->env, sdrl_retract_environment);
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
int sdrl_evaluate(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	int ret = 0;
	struct sdrl_event *event;

	if (!(event = sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr, mach->env)))
		return(SDRL_ERR_OUT_OF_MEMORY);
	do {
		ret = sdrl_evaluate_event(mach, event);
		sdrl_destroy_event(event);
		if (ret)
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

	// TODO we need to make a decision on this whole environment reference thing
	SDRL_DESTROY_REFERENCE(mach->env, sdrl_retract_environment);
	mach->env = SDRL_MAKE_REFERENCE(event->env);
//	mach->env = event->env;

	if (SDRL_USE_RET(event)) {
		value = mach->ret;
		mach->ret = NULL;
		ret = event->func(mach, event->param, value);
		sdrl_destroy_value(mach->heap, value);
	}
	else 
		ret = event->func(mach, event->param);
	return(ret);
}

/**
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

	sdrl_set_linenumber(expr->line);
	if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "number"), (sdrl_data_t) expr->data.number, 0, NULL);
	else if (expr->type == SDRL_ET_STRING)
		mach->ret = sdrl_make_value(mach->heap, sdrl_find_binding(mach->type_env, "string"), (sdrl_data_t) expr->data.str, strlen(expr->data.str), NULL);
	else if (expr->type == SDRL_ET_CALL) {
		if (!expr->data.expr)
			return(SDRL_ERROR(mach, SDRL_ERR_INVALID_FUNCTION, NULL));
		sdrl_set_linenumber(expr->data.expr->line);
		if (expr->data.expr->type == SDRL_ET_STRING) {
			if (!(func = sdrl_find_binding(mach->env, expr->data.expr->data.str)))
				return(SDRL_ERROR(mach, SDRL_ERR_NOT_FOUND, expr->data.expr->data.str));
			else if (func->type->evaluate && SDRL_TYPE_PASS_EXPRS(func->type))
				return(func->type->evaluate(mach, func, expr->data.expr->next));
			else {
				sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_call_value, func, mach->env));
				sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_params, expr->data.expr->next, mach->env));
				return(0);
			}
		}
		else if (expr->data.expr->type == SDRL_ET_CALL) {
			sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_call_value, NULL, mach->env));
			sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_params, expr->data.expr, mach->env));
			return(0);
		}
		else
			return(SDRL_ERROR(mach, SDRL_ERR_INVALID_FUNCTION, NULL));
	}
	else
		return(SDRL_ERROR(mach, SDRL_ERR_INVALID_AST_TYPE, NULL));
	return(0);
}

/**
 * Call the function stored in the value and set the result
 * in mach->ret or store a duplicate of the value if it is not executable
 */
int sdrl_call_value(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	int ret = 0;

	if (!func && args) {
		// TODO this is wrong, we can't clobber args
		func = args;
		args = args->next;
		func->next = NULL;
	}

	if (!func)
		ret = SDRL_ERROR(mach, SDRL_ERR_NOT_FOUND, NULL);
	else if (func->type->evaluate) {
		if (SDRL_TYPE_PASS_EXPRS(func->type))
			ret = SDRL_ERROR(mach, SDRL_ERR_INVALID_PARAMS, NULL);
		else {
//			mach->ret = args;
//			sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) func->type->evaluate, SDRL_MAKE_REFERENCE(func), mach->env));
			func->type->evaluate(mach, func, args);
		}
	}
	else {
		// TODO what do we do if the function is not evaluatable?
		if (args)
			ret = SDRL_ERROR(mach, SDRL_ERR_INVALID_PARAMS, NULL);
		else {
	//		mach->ret = SDRL_MAKE_REFERENCE(func);
			mach->ret = sdrl_duplicate_value(mach->heap, func);
		}
	}
	//sdrl_destroy_value(mach->heap, func);
	return(ret);
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
 * Destory the given reference to a value and if no further references
 * exist, the value itself will be destroyed.  (Note: this function is
 * primarily for use as an event handler).
 */
int sdrl_destroy_reference(struct sdrl_machine *mach, struct sdrl_value *value)
{
	return(sdrl_destroy_value(mach->heap, value));
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


