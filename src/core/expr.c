/*
 * Name:	expr.c
 * Version:	0.2
 * Description:	Functions for creating and manipulating expression structures
 */

#include <stdlib.h>
#include <string.h>

#include "expr.h"
#include "globals.h"


/**
 * Return a newly allocated call expression using a make'd expr, expr.
 */
struct sdrl_expr *sdrl_make_call_expr(struct sdrl_expr *call, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);

	expr->type = SDRL_ET_CALL;
	expr->data.expr = call;
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated string expression using a malloc'd string, str.
 */
struct sdrl_expr *sdrl_make_string_expr(char *str, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!str)
		return(NULL);
	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr) + strlen(str) + 1)))
		return(NULL);

	expr->type = SDRL_ET_STRING;
	expr->data.str = (char *) ((size_t) expr + sizeof(struct sdrl_expr));
	strcpy(expr->data.str, str);
	expr->next = next;
	return(expr);
}

/**
 * Return a newly allocated number expression
 */
struct sdrl_expr *sdrl_make_number_expr(number_t number, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);

	expr->type = SDRL_ET_NUMBER;
	expr->data.number = number;
	expr->next = next;
	return(expr);
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



