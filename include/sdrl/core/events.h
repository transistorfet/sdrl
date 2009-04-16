/*
 * Name:	events.h
 * Description:	Continuation Manager Header
 */


#ifndef _SDRL_CORE_EVENTS_H
#define _SDRL_CORE_EVENTS_H

#include <sdrl/core/bindings.h>
#include <sdrl/globals.h>

#define SDRL_EBF_USE_RET		0x0001		/** Pass mach->ret as the second parameter */
#define SDRL_EBF_PRESERVE_RET		0x0002		/** Don't clear mach->ret before calling function */

typedef int (*sdrl_event_t)(sdMachine *, ...);

struct sdEvent {
	int bitflags;
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

sdEvent *sdrl_make_event(int, sdrl_event_t, sdValue *, sdEnv *);
int sdrl_destroy_event(sdEvent *);
int sdrl_push_event(sdCont *, sdEvent *);
sdEvent *sdrl_pop_event(sdCont *);
sdEvent *sdrl_peek_event(sdCont *);

int sdrl_count_events(sdCont *);

#endif

