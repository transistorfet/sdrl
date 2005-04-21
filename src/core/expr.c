/*
 * Name:	expr.c
 * Version:	0.2
 * Description:	Functions for creating and manipulating expression structures
 */

#include <stdlib.h>
#include <string.h>

#include <sdrl/core/expr.h>
#include <sdrl/globals.h>


/**
 * Return a newly allocated number expression
 */
struct sdrl_expr *sdrl_make_number_expr(linenumber_t line, number_t number, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);

	expr->type = SDRL_ET_NUMBER;
	expr->line = line;
	expr->data.number = number;
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated string expression using a malloc'd string, str.
 */
struct sdrl_expr *sdrl_make_string_expr(linenumber_t line, char *str, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!str)
		return(NULL);
	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr) + strlen(str) + 1)))
		return(NULL);

	expr->type = SDRL_ET_STRING;
	expr->line = line;
	expr->data.str = (char *) ((size_t) expr + sizeof(struct sdrl_expr));
	strcpy(expr->data.str, str);
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated call expression using a make'd expr, expr.
 */
struct sdrl_expr *sdrl_make_call_expr(linenumber_t line, struct sdrl_expr *call, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);

	expr->type = SDRL_ET_CALL;
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
	else if (expr->type == SDRL_ET_NUMBER)
		return(sdrl_make_number_expr(expr->line, expr->data.number, sdrl_duplicate_expr(expr->next)));
	else if (expr->type == SDRL_ET_STRING)
		return(sdrl_make_string_expr(expr->line, expr->data.str, sdrl_duplicate_expr(expr->next)));
	else if (expr->type == SDRL_ET_CALL)
		return(sdrl_make_call_expr(expr->line, sdrl_duplicate_expr(expr->data.expr), sdrl_duplicate_expr(expr->next)));
	else
		return(NULL);
}

/**
 * Free all memory associated with the sdrl_expr
 */
int sdrl_destroy_expr(struct sdrl_expr *expr)
{
	struct sdrl_expr *tmp;

	while (expr) {
		if (expr->type == SDRL_ET_CALL)
			sdrl_destroy_expr(expr->data.expr);
		tmp = expr->next;
		free(expr);
		expr = tmp;
	}
	
	return(0);
}



