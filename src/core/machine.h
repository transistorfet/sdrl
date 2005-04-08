/*
 * Name:	machine.h
 * Version:	0.2
 * Description:	SDRL Header
 */


#ifndef _SDRL_CORE_MACHINE_H
#define _SDRL_CORE_MACHINE_H

#include "expr.h"
#include "value.h"
#include "events.h"
#include "bindings.h"
#include "globals.h"


struct sdrl_machine {
	struct sdrl_value *ret;
	struct sdrl_environment *type_env;
	struct sdrl_environment *env;
	struct sdrl_continuation *cont;
	struct sdrl_expr *code;
};

struct sdrl_machine *sdrl_create_machine(void);
int sdrl_destroy_machine(struct sdrl_machine *);

int sdrl_evaluate(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_event(struct sdrl_machine *, struct sdrl_event *);
int sdrl_evaluate_expr_list(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_expr(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_call_value(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);
int sdrl_evaluate_params(struct sdrl_machine *, struct sdrl_expr *);

int sdrl_merge_return(struct sdrl_machine *, struct sdrl_value *);
int sdrl_push_environment(struct sdrl_machine *);
int sdrl_pop_environment(struct sdrl_machine *);

#endif

