/*
 * Name:	expr.h
 * Version:	0.2
 * Description:	Expression Functions Header File
 */


#ifndef _SDRL_CORE_EXPR_H
#define _SDRL_CORE_EXPR_H

#include "globals.h"

#define SDRL_ET_NUMBER		0x01
#define SDRL_ET_STRING		0x02
#define SDRL_ET_CALL		0x03

struct sdrl_expr {
	int type;
	union {
		number_t number;
		char *str;
		struct sdrl_expr *expr;
	} data;
	struct sdrl_expr *next;
};

struct sdrl_expr *sdrl_make_number_expr(number_t, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_string_expr(char *, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_call_expr(struct sdrl_expr *, struct sdrl_expr *);
struct sdrl_expr *sdrl_duplicate_expr(struct sdrl_expr *);
int sdrl_destroy_expr(struct sdrl_expr *);

#endif

