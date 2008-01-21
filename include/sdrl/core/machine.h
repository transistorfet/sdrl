/*
 * Name:	machine.h
 * Description:	SDRL Header
 */


#ifndef _SDRL_CORE_MACHINE_H
#define _SDRL_CORE_MACHINE_H

#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/core/events.h>
#include <sdrl/core/bindings.h>
#include <sdrl/globals.h>

struct sdrl_machine {
	linenumber_t current_line;
	struct sdrl_heap *heap;
	struct sdrl_value *ret;
	struct sdrl_error *error;
	struct sdrl_continuation *cont;
	struct sdrl_environment *type_env;
	struct sdrl_environment *global;
	struct sdrl_environment *env;
};

/*** Generate an error and set it in the current machine. ***/
static inline int SDRL_ERROR(struct sdrl_machine *mach, short severity, int err, const char *msg) {
	if (mach->error)
		sdrl_destroy_error(mach->error);
	mach->error = sdrl_make_error(mach->current_line, severity, err, msg);
	return(err);
}

struct sdrl_machine *sdrl_create_machine(void);
int sdrl_destroy_machine(struct sdrl_machine *);

int sdrl_evaluate(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_event(struct sdrl_machine *, struct sdrl_event *);
int sdrl_evaluate_expr_list(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_expr(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_value(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);
int sdrl_evaluate_args(struct sdrl_machine *, struct sdrl_expr *);

#endif

