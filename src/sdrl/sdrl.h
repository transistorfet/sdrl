/*
 * Name:	sdrl.h
 * Version:	0.1
 * Description:	SDRL Header
 */


#ifndef SDRL_H
#define SDRL_H

#include "input.h"
#include "value.h"
#include "parse.h"
#include "bindings.h"
#include "types.h"


struct sdrl_machine {
	struct sdrl_value *ret;
	struct sdrl_value *opr;
	struct sdrl_environment *env;
	struct sdrl_expr *code;
};

struct sdrl_machine *sdrl_create_machine(void);
int sdrl_destroy_machine(struct sdrl_machine *);

int sdrl_evaluate_expr(struct sdrl_machine *, struct sdrl_expr *);
int sdrl_evaluate_function(struct sdrl_machine *, char *, struct sdrl_value *);

#endif

