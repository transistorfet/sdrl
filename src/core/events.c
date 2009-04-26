/*
 * Name:	events.c
 * Description:	Continuation Manager
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/events.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>


/**
 * Create a new continuation stack for sdrl_events.
 */
sdCont *sdrl_make_cont(void)
{
	sdCont *cont;

	if (!(cont = (sdCont *) malloc(sizeof(sdCont))))
		return(NULL);
	cont->top = NULL;
	return(cont);
}

/**
 * Free the resources allocated to the continuation stack.
 */
int sdrl_cont_destroy(sdCont *cont)
{
	sdEvent *cur, *tmp;

	if (!cont)
		return(-1);

	cur = cont->top;
	while (cur) {
		tmp = cur->next;
		sdrl_event_destroy(cur);
		cur = tmp;
	}
	free(cont);
	return(0);
}

/**
 * Allocate and initialize a new sdrl_event (to be added to a continuation).
 */
sdEvent *sdrl_make_event(sdrl_event_t func, sdValue *arg, sdEnv *env)
{
	sdEvent *event;

	if (!(event = (sdEvent *) malloc(sizeof(sdEvent))))
		return(NULL);
	event->func = func;
	event->arg = SDRL_INCREF(arg);
	event->env = SDRL_INCREF(env);
	event->next = NULL;
	return(event);
}

/**
 * Free the resources used by an sdrl_event
 */
int sdrl_event_destroy(sdEvent *event)
{
	if (event) {
		SDRL_DECREF(event->arg);
		SDRL_DECREF(event->env);
		free(event);
	}
	return(0);
}

/**
 * Make an sdEvent and push it onto the top of a continuation stack.
 */
int sdrl_event_push_new(sdCont *cont, sdrl_event_t func, sdValue *arg, sdEnv *env)
{
	sdEvent *event;

	if (!(event = sdrl_make_event(func, arg, env)))
		return(-1);
	event->next = cont->top;
	cont->top = event;
	return(0);
}

/**
 * Push an sdEvent onto the top of a continuation stack.
 */
int sdrl_event_push(sdCont *cont, sdEvent *event)
{
	event->next = cont->top;
	cont->top = event;
	return(0);
}

/**
 * Pop an sdrl_event off the top of a continuation stack.
 */
sdEvent *sdrl_event_pop(sdCont *cont)
{
	sdEvent *event;

	if (!cont->top)
		return(NULL);
	event = cont->top;
	cont->top = event->next;
	event->next = NULL;
	return(event);
}

/**
 * Return the sdrl_event on the top of the continuation stack.
 */
sdEvent *sdrl_event_peek(sdCont *cont)
{
	if (!cont->top)
		return(NULL);
	return(cont->top);
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



