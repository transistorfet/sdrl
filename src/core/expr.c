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
struct sdrl_expr *sdrl_make_number_expr(struct sdrl_type *type, int etype, linenumber_t line, number_t num, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);
	SDRL_VALUE(expr)->refs = 1;
	SDRL_VALUE(expr)->type = NULL;
	SDRL_VALUE(expr)->next = NULL;
	expr->type = etype;
	expr->line = line;
	expr->data.num = num;
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated string expression using a malloc'd string, str.
 */
struct sdrl_expr *sdrl_make_string_expr(struct sdrl_type *type, int etype, linenumber_t line, const char *str, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!str)
		return(NULL);
	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr) + strlen(str) + 1)))
		return(NULL);
	SDRL_VALUE(expr)->refs = 1;
	SDRL_VALUE(expr)->type = NULL;
	SDRL_VALUE(expr)->next = NULL;
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
struct sdrl_expr *sdrl_make_call_expr(struct sdrl_type *type, int etype, linenumber_t line, struct sdrl_expr *call, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);
	SDRL_VALUE(expr)->refs = 1;
	SDRL_VALUE(expr)->type = NULL;
	SDRL_VALUE(expr)->next = NULL;
	expr->type = etype;
	expr->line = line;
	expr->data.expr = call;
	expr->next = next;
	return(expr);
}

/**
 * Create a duplicate of the expr.
 */
struct sdrl_expr *sdrl_duplicate_expr(struct sdrl_expr *expr)
{
	if (!expr)
		return(NULL);
	else if (expr->type & SDRL_ED_NUMBER)
		return(sdrl_make_number_expr(SDRL_VALUE(expr)->type, expr->type, expr->line, expr->data.num, sdrl_duplicate_expr(expr->next)));
	else if (expr->type & SDRL_ED_STRING)
		return(sdrl_make_string_expr(SDRL_VALUE(expr)->type, expr->type, expr->line, expr->data.str, sdrl_duplicate_expr(expr->next)));
	else if (expr->type & SDRL_ED_EXPR)
		return(sdrl_make_call_expr(SDRL_VALUE(expr)->type, expr->type, expr->line, sdrl_duplicate_expr(expr->data.expr), sdrl_duplicate_expr(expr->next)));
	else
		return(NULL);
}

/**
 * Free all memory associated with the sdrl_expr
 */
int sdrl_destroy_expr(struct sdrl_expr *expr)
{
	struct sdrl_expr *tmp;

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



