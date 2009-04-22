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

sdType *sdrl_make_expr_type(void);

sdExpr *sdrl_make_number_expr(sdType *, int, linenumber_t, number_t, sdExpr *);
sdExpr *sdrl_make_string_expr(sdType *, int, linenumber_t, const char *, sdExpr *);
sdExpr *sdrl_make_call_expr(sdType *, int, linenumber_t, sdExpr *, sdExpr *);
sdExpr *sdrl_duplicate_expr(sdMachine *, sdExpr *);
int sdrl_destroy_expr(sdExpr *);
int sdrl_evaluate_expr(sdMachine *, sdArray *);

#endif

