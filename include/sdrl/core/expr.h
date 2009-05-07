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

#define SDEXPR(ptr)		( (sdExpr *) (ptr) )

extern sdType sdExprTypeDef;

struct sdExpr {
	sdValue value;
	int type;
	linenumber_t line;
	union {
		number_t num;
		char *str;
		sdExpr *expr;
	} data;
	sdExpr *next;
};

sdExpr *sdrl_make_number_expr(sdMachine *, sdType *, int, linenumber_t, number_t, sdExpr *);
sdExpr *sdrl_make_string_expr(sdMachine *, sdType *, int, linenumber_t, const char *, sdExpr *);
sdExpr *sdrl_make_expr_expr(sdMachine *, sdType *, int, linenumber_t, sdExpr *, sdExpr *);
sdExpr *sdrl_expr_duplicate(sdMachine *, sdExpr *);
void sdrl_expr_destroy(sdExpr *);
int sdrl_expr_evaluate(sdMachine *, sdArray *);

#endif

