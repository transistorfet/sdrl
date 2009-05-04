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
};

struct sdCont {
	int size;
	int sp;
	sdEvent *stack;
};

sdCont *sdrl_make_cont(void);
void sdrl_cont_destroy(sdCont *);

int sdrl_event_push(sdCont *, sdrl_event_t, sdValue *, sdEnv *);
int sdrl_event_pop(sdCont *);

#define sdrl_event_get_top(cont)	\
	( (cont)->sp >= 0 ? &(cont)->stack[(cont)->sp] : NULL )

#define sdrl_event_level(cont)	\
	( (cont)->sp )

#endif

