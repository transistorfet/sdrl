/*
 * Name:	expr.c
 * Version:	0.1
 * Description:	Functions for creating and manipulating expression structures
 */

#include <stdlib.h>
#include <stdio.h>

#include "expr.h"
#include "types.h"


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
 * Return a newly allocated name expression using a malloc'd string, name.
 */
struct sdrl_expr *sdrl_make_name_expr(char *name, struct sdrl_expr *next)
{
	struct sdrl_expr *expr;

	if (!(expr = (struct sdrl_expr *) malloc(sizeof(struct sdrl_expr))))
		return(NULL);

	expr->type = SDRL_ET_NAME;
	expr->data.name = name;
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
int destroy_sdrl_expr(struct sdrl_expr *expr)
{
	struct sdrl_expr *tmp;

	while (expr) {
		if (expr->type == SDRL_ET_CALL)
			destroy_sdrl_expr(expr->data.expr);
		else if (expr->type == SDRL_ET_NAME)
			free(expr->data.name);
		tmp = expr->next;
		free(expr);
		expr = tmp;
	}
	
	return(0);
}

int sdrl_print_expr(struct sdrl_expr *expr, int tabs)
{
	int i;

	do {
		for (i = 0;i < tabs;i++)
			printf("  ");
		if (!expr)
			printf("null");
		else if (expr->type == SDRL_ET_NUMBER)
			printf("%f", expr->data.number);
		else if (expr->type == SDRL_ET_NAME)
			printf("%s", expr->data.name);
		else if (expr->type == SDRL_ET_CALL) {
			if (expr->data.expr && (expr->data.expr->type == SDRL_ET_NAME)) {
				printf("call:%s(\n", expr->data.expr->data.name);
				sdrl_print_expr(expr->data.expr->next, tabs + 1);
				printf(")");
			}
			else
				return(-1);
		}
		printf("\n");
		expr = expr->next;
	} while (expr);

	return(0);
}

