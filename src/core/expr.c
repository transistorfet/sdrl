/*
 * Name:	expr.c
 * Description:	Functions for creating and manipulating expression structures
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/expr.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>

sdType sdExprTypeDef = {
	&sdValueTypeDef,
	sizeof(sdExpr),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_expr_destroy,
	(sdrl_duplicate_t) sdrl_expr_duplicate,
	(sdrl_evaluate_t) sdrl_expr_evaluate
};


/**
 * Return a newly allocated number expression
 */
sdExpr *sdrl_make_number_expr(sdHeap *heap, sdType *type, int etype, linenumber_t line, number_t num, sdExpr *next)
{
	sdExpr *expr;

	if (!(expr = (sdExpr *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = type;
	expr->type = etype;
	expr->line = line;
	expr->data.num = num;
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated string expression using a malloc'd string, str.
 */
sdExpr *sdrl_make_string_expr(sdHeap *heap, sdType *type, int etype, linenumber_t line, const char *str, sdExpr *next)
{
	sdExpr *expr;

	if (!str)
		return(NULL);
	if (!(expr = (sdExpr *) sdrl_heap_alloc(heap, type->size + strlen(str) + 1)))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = type;
	expr->type = etype;
	expr->line = line;
	expr->data.str = (char *) (((char *) expr) + type->size);
	strcpy(expr->data.str, str);
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated call expression using a make'd expr, expr.
 */
sdExpr *sdrl_make_expr_expr(sdHeap *heap, sdType *type, int etype, linenumber_t line, sdExpr *call, sdExpr *next)
{
	sdExpr *expr;

	if (!(expr = (sdExpr *) sdrl_heap_alloc(heap, type->size)))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = type;
	expr->type = etype;
	expr->line = line;
	expr->data.expr = call;
	expr->next = next;
	return(expr);
}

/**
 * Create a duplicate of the expr.
 */
sdExpr *sdrl_expr_duplicate(sdMachine *mach, sdExpr *expr)
{
	// TODO modify to use mach->heap for allocation
	if (!expr)
		return(NULL);
	else if (expr->type & SDRL_ED_NUMBER)
		return(sdrl_make_number_expr(mach->heap, SDVALUE(expr)->type, expr->type, expr->line, expr->data.num, sdrl_expr_duplicate(mach, expr->next)));
	else if (expr->type & SDRL_ED_STRING)
		return(sdrl_make_string_expr(mach->heap, SDVALUE(expr)->type, expr->type, expr->line, expr->data.str, sdrl_expr_duplicate(mach, expr->next)));
	else if (expr->type & SDRL_ED_EXPR)
		return(sdrl_make_expr_expr(mach->heap, SDVALUE(expr)->type, expr->type, expr->line, sdrl_expr_duplicate(mach, expr->data.expr), sdrl_expr_duplicate(mach, expr->next)));
	else
		return(NULL);
}

/**
 * Free all memory associated with the sdrl_expr
 */
void sdrl_expr_destroy(sdExpr *expr)
{
	sdExpr *tmp;

	// TODO should we check/decrement the refcounter since we are re-entering this function recursively
	while (expr) {
		if (expr->type & SDRL_ED_EXPR)
			sdrl_expr_destroy(expr->data.expr);
		tmp = expr->next;
		sdrl_heap_free(expr);
		expr = tmp;
	}
}

/**
 * Use the given machine to evaluate the expression.
 */
int sdrl_expr_evaluate(sdMachine *mach, sdArray *args)
{
	// TODO what do you do (if anything) with the arguments?
	SDRL_INCREF(args);
	if (sdrl_env_add(mach->env, "_", args))
		sdrl_env_replace(mach->env, "_", args);
	sdrl_event_push_new(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, args->items[0], mach->env);
	return(0);
}

