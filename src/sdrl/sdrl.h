/*
 * Name:	sdrl.h
 * Version:	0.1
 * Description:	SDRL Header
 */


#ifndef SDRL_H
#define SDRL_H

#include "io.h"
#include "value.h"
#include "bindings.h"
#include "types.h"

#define SDRL_INPUT_STACK	64

#define SBF_NOPRIMATIVES	0x01

#define SEBF_IOSTRING		0x00ff
#define SEBF_REPEAT		IBF_REPEAT


struct sdrl_machine {
	struct value *ret;
	struct value *opr;
	struct environment *env;
	struct input_stack *input;
};

struct sdrl_machine *create_sdrl_machine(uubyte);
int destroy_sdrl_machine(struct sdrl_machine *);

int sdrl_run(struct sdrl_machine *);
int sdrl_evaluate(struct sdrl_machine *, char *, uuword);
struct value *sdrl_function_call(struct sdrl_machine *, char *, struct value *);
int sdrl_loadfile(struct sdrl_machine *, char *);
int sdrl_loadopr(struct sdrl_machine *, struct value *);

int sdrl_add_binding(struct sdrl_machine *, char *, struct value *);
int sdrl_remove_binding(struct sdrl_machine *, char *);
int sdrl_replace_binding(struct sdrl_machine *, char *, struct value *);
struct binding *sdrl_find_binding(struct sdrl_machine *, char *);

int sdrl_evaluate_value(struct sdrl_machine *, struct value *, uuword);
int sdrl_function_call_value(struct sdrl_machine *, struct value *, uuword);
int sdrl_changeflags(struct sdrl_machine *, int, uubyte, uubyte);
int sdrl_step(struct sdrl_machine *);

#endif

