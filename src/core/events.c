/*
 * Name:	events.c
 * Description:	Continuation Manager
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/events.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_STACK_SIZE		200

/**
 * Create a new continuation stack for sdrl_events.
 */
sdCont *sdrl_make_cont(void)
{
	sdCont *cont;

	if (!(cont = (sdCont *) malloc(sizeof(sdCont))))
		return(NULL);
	if (!(cont->stack = (sdEvent *) malloc(sizeof(sdEvent) * SDRL_STACK_SIZE))) {
		free(cont);
		return(NULL);
	}
	cont->size = SDRL_STACK_SIZE;
	cont->sp = -1;
	return(cont);
}

/**
 * Free the resources allocated to the continuation stack.
 */
void sdrl_cont_destroy(sdCont *cont)
{
	if (!cont)
		return;
	for (; cont->sp >= 0; cont->sp--) {
		SDRL_DECREF(cont->stack[cont->sp].arg);
		SDRL_DECREF(cont->stack[cont->sp].env);
	}
	free(cont);
}


/**
 * Make an sdEvent and push it onto the top of a continuation stack.
 */
int sdrl_event_push(sdCont *cont, sdrl_event_t func, sdValue *arg, sdEnv *env)
{
	sdEvent *newstack;

	if (++cont->sp >= cont->size) {
		if (!(newstack = (sdEvent *) realloc(cont->stack, sizeof(sdEvent) * (cont->size + SDRL_STACK_SIZE))))
			return(-1);
		cont->stack = newstack;
		cont->size += SDRL_STACK_SIZE;
	}
	cont->stack[cont->sp].func = func;
	cont->stack[cont->sp].arg = SDRL_INCREF(arg);
	cont->stack[cont->sp].env = SDRL_INCREF(env);
	return(0);
}

/**
 * Pop an sdrl_event off the top of a continuation stack.
 */
int sdrl_event_pop(sdCont *cont)
{
	if (cont->sp < 0)
		return(-1);
	// TODO should you resize down?
	SDRL_DECREF(cont->stack[cont->sp].arg);
	SDRL_DECREF(cont->stack[cont->sp].env);
	cont->sp--;
	return(0);
}



/*

sdrl_eval_expr:
	if number or string then set mach->ret
	if call then sdrl_eval_value

sdrl_eval_value:
	if no eval
		push-cont <func> <exprs>
	if eval args
		push-cont <func> <mach->ret>
		push-cont sdrl_apply_args <exprs>

sdrl_apply_args:
	pop-cont exprs
	push-cont sdrl_apply_args tail(exprs)
	push-cont stack-oprs <mach-ret> current-ret
	push-cont sdrl_eval_expr head(exprs)

*/



