/*
 * Name:	expr.h
 * Version:	0.1
 * Description:	Expression Functions Header File
 */


#ifndef EXPR_H
#define EXPR_H

#include "types.h"

#define SDRL_ET_CALL		0x01
#define SDRL_ET_NAME		0x02
#define SDRL_ET_NUMBER		0x03

struct sdrl_expr {
	int type;
	union {
		number_t number;
		char *name;
		struct sdrl_expr *expr;
	} data;
	struct sdrl_expr *next;
};

struct sdrl_expr *sdrl_make_call_expr(struct sdrl_expr *, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_name_expr(char *, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_number_expr(number_t, struct sdrl_expr *);
int sdrl_destroy_expr(struct sdrl_expr *);

#endif

