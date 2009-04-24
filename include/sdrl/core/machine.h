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
	linenumber_t current_line;
	sdHeap *heap;
	sdValue *ret;
	sdError *error;
	sdCont *cont;
	sdEnv *type_env;
	sdEnv *global;
	sdEnv *env;
};

/*** Generate an error and set it in the current machine. ***/
static inline int sdrl_set_error(sdMachine *mach, short severity, int err, const char *msg) {
	if (mach->error)
		sdrl_destroy_error(mach->error);
	mach->error = sdrl_make_error(mach->heap, mach->current_line, severity, err, msg);
	return(err);
}

/*** Try a piece of code and jump to a failure point if it produces an error.  This assumes that
     there is an sdrl_machine pointer called "mach" in the current scope and a label called "FAIL". */
#define SDRL_TRY(CODE) {	\
	(CODE);			\
	if (mach->error)	\
		goto FAIL;	\
}

sdMachine *sdrl_create_machine(void);
int sdrl_destroy_machine(sdMachine *);

int sdrl_evaluate(sdMachine *, sdExpr *);
int sdrl_evaluate_expr_list(sdMachine *, sdExpr *);
int sdrl_evaluate_expr_value(sdMachine *, sdExpr *);
int sdrl_evaluate_value(sdMachine *, sdArray *);

#endif

