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
#include <sdrl/core/basetypes.h>
#include <sdrl/globals.h>

static int sdrl_machine_merge_return(struct sdrl_machine *, struct sdrl_value *);

/**
 * Create a machine for executing code
 */
struct sdrl_machine *sdrl_create_machine(void)
{
	struct sdrl_type *type;
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->current_line = 0;
	mach->ret = NULL;
	mach->error = NULL;
	if (!(mach->heap = sdrl_create_heap()) || !(mach->cont = sdrl_create_continuation())) {
		sdrl_destroy_machine(mach);
		return(NULL);
	}
	// TODO clean this up
	if (!(type = sdrl_make_environment_type())
	    || !(mach->type_env = sdrl_create_environment(mach->heap, type, SDRL_BBF_CONSTANT, (sdrl_destroy_t) sdrl_destroy_type))) {
		if (type)
			sdrl_destroy_type(type);
		sdrl_destroy_machine(mach);
		return(NULL);
	}
	sdrl_add_binding(mach->type_env, "*env*", type);
	if (!(mach->global = sdrl_create_environment(mach->heap, type, 0, (sdrl_destroy_t) sdrl_destroy_value))) {
		sdrl_destroy_machine(mach);
		return(NULL);
	}
	mach->env = SDRL_MAKE_REFERENCE(mach->global);

	sdrl_add_binding(mach->type_env, "number", sdrl_make_number_type());
	sdrl_add_binding(mach->type_env, "string", sdrl_make_string_type());
	sdrl_add_binding(mach->type_env, "*expr*", sdrl_make_expression_type());

	return(mach);
}

/**
 * Free the resources allocated to mach
 */
int sdrl_destroy_machine(struct sdrl_machine *mach)
{
	sdrl_destroy_value(mach->ret);
	sdrl_destroy_continuation(mach->cont);
	SDRL_DESTROY_REFERENCE(mach->env);
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

	if (!(event = sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, SDRL_VALUE(expr), mach->env)))
		return(SDRL_ERROR(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	do {
		ret = sdrl_evaluate_event(mach, event);
		sdrl_destroy_event(event);
		// TODO check the error and see if we should return or continue
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
	struct sdrl_value *args;

	SDRL_DESTROY_REFERENCE(mach->env);
	mach->env = SDRL_MAKE_REFERENCE(event->env);

	if (SDRL_BF_IS_SET(event, SDRL_EBF_USE_RET)) {
		args = mach->ret;
		mach->ret = NULL;
		ret = event->func(mach, event->param, args);
		SDRL_DESTROY_REFERENCE(args);
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
		sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, SDRL_VALUE(expr->next), mach->env));
	return(sdrl_evaluate_expr(mach, expr));
}

/**
 * Evaluate the single sdrl_expr and either return 0 with the result
 * stored in sdrl_machine.ret or return an error code.
 */
int sdrl_evaluate_expr(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	struct sdrl_value *func;

	SDRL_DESTROY_REFERENCE(mach->ret);
	mach->ret = NULL;

	if (!expr)
		return(0);

	mach->current_line = expr->line;
	if (expr->type == SDRL_ET_NUMBER)
		mach->ret = sdrl_make_number(mach->heap, sdrl_find_binding(mach->type_env, "number"), expr->data.num);
	else if (expr->type == SDRL_ET_STRING)
		mach->ret = sdrl_make_string(mach->heap, sdrl_find_binding(mach->type_env, "string"), expr->data.str, strlen(expr->data.str));
	else if (expr->type == SDRL_ET_CALL) {
		if (!expr->data.expr)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_FUNCTION, NULL));
		mach->current_line = expr->data.expr->line;
		if (expr->data.expr->type == SDRL_ET_STRING) {
			if (!(func = sdrl_find_binding(mach->env, expr->data.expr->data.str)))
				return(SDRL_ERROR(mach, SDRL_ES_MEDIUM, SDRL_ERR_NOT_FOUND, NULL));
			else if (func->type->evaluate && SDRL_BF_IS_SET(func->type, SDRL_TBF_PASS_EXPRS))
				return(func->type->evaluate(mach, func, SDRL_VALUE(expr->data.expr->next)));
			else {
				sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_evaluate_value, func, mach->env));
				sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_args, SDRL_VALUE(expr->data.expr->next), mach->env));
				return(0);
			}
		}
		else if (expr->data.expr->type == SDRL_ET_CALL) {
			sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_evaluate_value, NULL, mach->env));
			sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_args, SDRL_VALUE(expr->data.expr), mach->env));
			return(0);
		}
		else
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_FUNCTION, NULL));
	}
	else
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_AST_TYPE, NULL));
	return(0);
}

/**
 * Call the function stored in the value and set the result
 * in mach->ret or store a duplicate of the value if it is not executable
 */
int sdrl_evaluate_value(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	int ret = 0;

	if (!func && args) {
		func = args;
		args = args->next;
	}

	if (!func)
		ret = SDRL_ERROR(mach, SDRL_ES_LOW, SDRL_ERR_NOT_FOUND, NULL);
	else if (func->type->evaluate) {
		if (SDRL_BF_IS_SET(func->type, SDRL_TBF_PASS_EXPRS))
			ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		else {
			// TODO what are the advantages and disadvantages of these two ways of execution?
			//mach->ret = args;
			//sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) func->type->evaluate, SDRL_MAKE_REFERENCE(func), mach->env));
			ret = func->type->evaluate(mach, func, args);
		}
	}
	else {
		// TODO what do we do if the function is not evaluatable?
		if (args)
			ret = SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		else
			mach->ret = sdrl_duplicate_value(mach, func);
	}
	return(ret);
}

/**
 * Evaluate all the exprs and build a list of the corresponding return values.
 */
int sdrl_evaluate_args(struct sdrl_machine *mach, struct sdrl_expr *exprs)
{
	if (!exprs)
		return(0);
	if (exprs->next)
		sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_args, SDRL_VALUE(exprs->next), mach->env));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_machine_merge_return, mach->ret, mach->env));
	mach->ret = NULL;
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr, SDRL_VALUE(exprs), mach->env));
	return(0);
}

/*** Local Functions ***/

/**
 * Append the current value of mach->ret to the end of ret and store the whole
 * thing in mach->ret.
 */
static int sdrl_machine_merge_return(struct sdrl_machine *mach, struct sdrl_value *ret)
{
	sdrl_push_value(&ret, mach->ret);
	mach->ret = ret;
	return(0);
}


