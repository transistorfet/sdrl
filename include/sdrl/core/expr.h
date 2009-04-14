/*
 * Name:	expr.h
 * Description:	Expression Functions Header File
 */


#ifndef _SDRL_CORE_EXPR_H
#define _SDRL_CORE_EXPR_H

#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_ED_NUMBER		0x1000
#define SDRL_ED_STRING		0x2000
#define SDRL_ED_EXPR		0x4000

#define SDRL_ET_NUMBER		0x1001
#define SDRL_ET_STRING		0x2001
#define SDRL_ET_IDENTIFIER	0x2002
#define SDRL_ET_CALL		0x4001

struct sdrl_expr {
	struct sdrl_value value;
	int type;
	linenumber_t line;
	union {
		number_t num;
		char *str;
		struct sdrl_expr *expr;
	} data;
	// TODO should you use the value's next member instead?
	struct sdrl_expr *next;
};

struct sdrl_expr *sdrl_make_number_expr(struct sdrl_type *, int, linenumber_t, number_t, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_string_expr(struct sdrl_type *, int, linenumber_t, const char *, struct sdrl_expr *);
struct sdrl_expr *sdrl_make_call_expr(struct sdrl_type *, int, linenumber_t, struct sdrl_expr *, struct sdrl_expr *);
struct sdrl_expr *sdrl_duplicate_expr(struct sdrl_expr *);
int sdrl_destroy_expr(struct sdrl_expr *);

#endif

