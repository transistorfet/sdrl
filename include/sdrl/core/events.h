/*
 * Name:	events.h
 * Description:	Continuation Manager Header
 */


#ifndef _SDRL_CORE_EVENTS_H
#define _SDRL_CORE_EVENTS_H

#include <sdrl/core/bindings.h>
#include <sdrl/globals.h>

typedef int (*sdrl_event_t)(sdMachine *, sdValue *);

struct sdEvent {
	sdrl_event_t func;
	sdValue *arg;
	sdEnv *env;
	sdEvent *next;
};

struct sdCont {
	sdEvent *top;
};

sdCont *sdrl_create_continuation(void);
int sdrl_destroy_continuation(sdCont *);

sdEvent *sdrl_make_event(sdrl_event_t, sdValue *, sdEnv *);
int sdrl_destroy_event(sdEvent *);
int sdrl_push_new_event(sdCont *, sdrl_event_t, sdValue *, sdEnv *);
int sdrl_push_event(sdCont *, sdEvent *);
sdEvent *sdrl_pop_event(sdCont *);
sdEvent *sdrl_peek_event(sdCont *);

int sdrl_count_events(sdCont *);

#endif

