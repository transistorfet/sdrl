/*
 * Name:	events.h
 * Description:	Continuation Manager Header
 */


#ifndef _SDRL_CORE_EVENTS_H
#define _SDRL_CORE_EVENTS_H

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

sdCont *sdrl_make_cont(void);
int sdrl_cont_destroy(sdCont *);

sdEvent *sdrl_make_event(sdrl_event_t, sdValue *, sdEnv *);
int sdrl_event_destroy(sdEvent *);
int sdrl_event_push_new(sdCont *, sdrl_event_t, sdValue *, sdEnv *);
int sdrl_event_push(sdCont *, sdEvent *);
sdEvent *sdrl_event_pop(sdCont *);
sdEvent *sdrl_event_peek(sdCont *);

#endif

