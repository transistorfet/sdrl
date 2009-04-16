/*
 * Module Name:	dispexpr.c
 * Description:	Expression Display
 */

#include <stdio.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

static int dispexpr_display_expr(sdExpr *);

/**
 * Display the expression tree.
 */
int sdrl_base_display_expr(sdExpr *expr)
{
	while (expr) {
		dispexpr_display_expr(expr);
		printf(" ");
		expr = expr->next;
	}
	return(0);
}

/*** Local Functions ***/

/**
 * Display a single expression.
 */
static int dispexpr_display_expr(sdExpr *expr)
{
	if (!expr)
		return(1);
	else if (expr->type == SDRL_ET_NUMBER)
		printf("<%f>", expr->data.num);
	else if (expr->type == SDRL_ET_STRING)
		printf("\"%s\"", expr->data.str);
	else if (expr->type == SDRL_ET_IDENTIFIER)
		printf("%s", expr->data.str);
	else if (expr->type == SDRL_ET_CALL) {
		printf("[");
		sdrl_base_display_expr(expr->data.expr);
		printf("]");
	}
	else
		printf("*%d*", expr->type);
	return(0);
}


