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

sdMachine *sdrl_make_machine(void);
int sdrl_machine_destroy(sdMachine *);

int sdrl_evaluate(sdMachine *, sdExpr *);
int sdrl_evaluate_expr_list(sdMachine *, sdExpr *);
int sdrl_evaluate_expr_value(sdMachine *, sdExpr *);
int sdrl_evaluate_value(sdMachine *, sdArray *);


/*** Try a piece of code and jump to a failure point if it produces an error.  This assumes that
     there is an sdrl_machine pointer called "mach" in the current scope and a label called "FAIL". */
#define SDRL_TRY(CODE) {	\
	(CODE);			\
	if (mach->error)	\
		goto FAIL;	\
}

/*** Generate an error and set it in the current machine. ***/
static inline int sdrl_set_error(sdMachine *mach, short severity, int err, const char *msg) {
	SDRL_DECREF(mach->error);
	mach->error = sdrl_make_error(mach->heap, &sdErrorTypeDef, mach->current_line, severity, err, msg);
	return(err);
}

/*** Generate an out of memory error and set it in the machine. ***/
static inline int sdrl_set_memory_error(sdMachine *mach) {
	SDRL_DECREF(mach->error);
	mach->error = SDRL_INCREF(&sdMemoryError);
	return(0);
}

/*** Generate an invalid type error and set it in the machine. ***/
#define sdrl_set_type_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_INVALID_TYPE, NULL)

/*** Generate an invalid args error and set it in the machine. ***/
#define sdrl_set_args_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_INVALID_ARGS, NULL)

/*** Generate a not found error and set it in the machine. ***/
#define sdrl_set_not_found_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_NOT_FOUND, NULL)

/*** Generate an out of bounds error and set it in the machine. ***/
#define sdrl_set_bounds_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_BOUNDS, NULL)

#endif

