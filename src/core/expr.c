/*
 * Name:	expr.c
 * Description:	Functions for creating and manipulating expression structures
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/expr.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

/**
 * Return a newly allocated number expression
 */
sdExpr *sdrl_make_number_expr(sdType *type, int etype, linenumber_t line, number_t num, sdExpr *next)
{
	sdExpr *expr;

	if (!(expr = (sdExpr *) malloc(sizeof(sdExpr))))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = NULL;
	SDVALUE(expr)->next = NULL;
	expr->type = etype;
	expr->line = line;
	expr->data.num = num;
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated string expression using a malloc'd string, str.
 */
sdExpr *sdrl_make_string_expr(sdType *type, int etype, linenumber_t line, const char *str, sdExpr *next)
{
	sdExpr *expr;

	if (!str)
		return(NULL);
	if (!(expr = (sdExpr *) malloc(sizeof(sdExpr) + strlen(str) + 1)))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = NULL;
	SDVALUE(expr)->next = NULL;
	expr->type = etype;
	expr->line = line;
	expr->data.str = (char *) (expr + 1);
	strcpy(expr->data.str, str);
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated call expression using a make'd expr, expr.
 */
sdExpr *sdrl_make_call_expr(sdType *type, int etype, linenumber_t line, sdExpr *call, sdExpr *next)
{
	sdExpr *expr;

	if (!(expr = (sdExpr *) malloc(sizeof(sdExpr))))
		return(NULL);
	SDVALUE(expr)->refs = 1;
	SDVALUE(expr)->type = NULL;
	SDVALUE(expr)->next = NULL;
	expr->type = etype;
	expr->line = line;
	expr->data.expr = call;
	expr->next = next;
	return(expr);
}

/**
 * Create a duplicate of the expr.
 */
sdExpr *sdrl_duplicate_expr(sdExpr *expr)
{
	if (!expr)
		return(NULL);
	else if (expr->type & SDRL_ED_NUMBER)
		return(sdrl_make_number_expr(SDVALUE(expr)->type, expr->type, expr->line, expr->data.num, sdrl_duplicate_expr(expr->next)));
	else if (expr->type & SDRL_ED_STRING)
		return(sdrl_make_string_expr(SDVALUE(expr)->type, expr->type, expr->line, expr->data.str, sdrl_duplicate_expr(expr->next)));
	else if (expr->type & SDRL_ED_EXPR)
		return(sdrl_make_call_expr(SDVALUE(expr)->type, expr->type, expr->line, sdrl_duplicate_expr(expr->data.expr), sdrl_duplicate_expr(expr->next)));
	else
		return(NULL);
}

/**
 * Free all memory associated with the sdrl_expr
 */
int sdrl_destroy_expr(sdExpr *expr)
{
	sdExpr *tmp;

	// TODO should we check/decrement the refcounter since we are re-entering this function recursively
	while (expr) {
		if (expr->type & SDRL_ED_EXPR)
			sdrl_destroy_expr(expr->data.expr);
		tmp = expr->next;
		free(expr);
		expr = tmp;
	}
	
	return(0);
}



