/*
 * Name:	events.c
 * Version:	0.2
 * Description:	Continuation Manager
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/events.h>
#include <sdrl/globals.h>


/**
 * Create a new continuation stack for sdrl_events.
 */
struct sdrl_continuation *sdrl_create_continuation(void)
{
	struct sdrl_continuation *cont;

	if (!(cont = (struct sdrl_continuation *) malloc(sizeof(struct sdrl_continuation))))
		return(NULL);
	cont->top = NULL;
	return(cont);
}

/**
 * Free the resources allocated to the continuation stack.
 */
int sdrl_destroy_continuation(struct sdrl_continuation *cont)
{
	struct sdrl_event *cur, *tmp;

	if (!cont)
		return(-1);

	cur = cont->top;
	while (cur) {
		tmp = cur->next;
		sdrl_destroy_event(cur);
		cur = tmp;
	}
	free(cont);
	return(0);
}

/**
 * Allocate and initialize a new sdrl_event (to be added to a continuation).
 */
struct sdrl_event *sdrl_make_event(int bitflags, sdrl_event_t func, void *param, struct sdrl_environment *env)
{
	struct sdrl_event *event;

	if (!(event = (struct sdrl_event *) malloc(sizeof(struct sdrl_event))))
		return(NULL);
	event->bitflags = bitflags;
	event->func = func;
	event->param = param;
	event->env = sdrl_make_reference_m(env);
	event->next = NULL;

	return(event);
}

/**
 * Free the resources used by an sdrl_event
 */
int sdrl_destroy_event(struct sdrl_event *event)
{
	if (event) {
		sdrl_destroy_reference_m(event->env, sdrl_retract_environment);
		free(event);
	}
	return(0);
}

/**
 * Push an sdrl_event onto the top of a continuation stack.
 */
int sdrl_push_event(struct sdrl_continuation *cont, struct sdrl_event *event)
{
	event->next = cont->top;
	cont->top = event;
	return(0);
}

/**
 * Pop an sdrl_event off the top of a continuation stack.
 */
struct sdrl_event *sdrl_pop_event(struct sdrl_continuation *cont)
{
	struct sdrl_event *event;

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
struct sdrl_event *sdrl_peek_event(struct sdrl_continuation *cont)
{
	if (!cont->top)
		return(NULL);
	return(cont->top);
}

/**
 * Count the number of events on the continuation.
 */
int sdrl_count_events(struct sdrl_continuation *cont)
{
	int i = 0;
	struct sdrl_event *cur;

	cur = cont->top;
	while (cur) {
		i++;
		cur = cur->next;
	}
	return(i);
}

/*

sdrl_eval_expr:
	if number or string set mach->ret
	if call then sdrl_eval_value

sdrl_eval_value:
	if no eval
		push-cont <func> <exprs>
	if eval params
		push-cont <func> <mach->ret>
		push-cont sdrl_apply_params <exprs>

sdrl_apply_params:
	pop-cont exprs
	push-cont sdrl_apply_params tail(exprs)
	push-cont stack-oprs <mach-ret> current-ret
	push-cont sdrl_eval_expr head(exprs)

*/



