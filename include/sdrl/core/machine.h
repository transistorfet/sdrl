/*
 * Name:	machine.h
 * Description:	SDRL Header
 */


#ifndef _SDRL_CORE_MACHINE_H
#define _SDRL_CORE_MACHINE_H

#include <sdrl/core/env.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/array.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/core/events.h>
#include <sdrl/globals.h>

#define SDRL_DEFAULT_ARGS	4

struct sdMachine {
	sdrl_parser_t parser;
	sdrl_preprocessor_t preproc;
	linenumber_t current_line;
	sdHeap *heap;
	sdValue *ret;
	sdError *error;
	sdCont *cont;
	sdEnv *type_env;
	sdEnv *global;
	sdEnv *env;
};

sdMachine *sdrl_make_machine(void);
void sdrl_machine_destroy(sdMachine *);

int sdrl_eval(sdMachine *, sdExpr *);
int sdrl_call(sdMachine *, sdValue *, int, ...);
int sdrl_run(sdMachine *, int);

int sdrl_evaluate_expr_list(sdMachine *, sdExpr *);
int sdrl_evaluate_expr_value(sdMachine *, sdExpr *);
int sdrl_evaluate_value(sdMachine *, sdArray *);

/*** Set the parser function in the given machine */
#define sdrl_set_parser(mach, func)	\
	( (mach)->parser = (sdrl_parser_t) (func) )

/*** Set the preprocessor function in the given machine */
#define sdrl_set_preprocessor(mach, func)	\
	( (mach)->preproc= (sdrl_preprocessor_t) (func) )

#endif

