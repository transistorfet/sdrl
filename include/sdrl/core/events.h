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

struct sdrl_machine;
typedef int (*sdrl_event_t)(struct sdrl_machine *, ...);

struct sdrl_event {
	int bitflags;
	sdrl_event_t func;
	void *param;
	struct sdrl_environment *env;
	struct sdrl_event *next;
};

struct sdrl_continuation {
	struct sdrl_event *top;
};

struct sdrl_continuation *sdrl_create_continuation(void);
int sdrl_destroy_continuation(struct sdrl_continuation *);

struct sdrl_event *sdrl_make_event(int, sdrl_event_t, void *, struct sdrl_environment *);
int sdrl_destroy_event(struct sdrl_event *);
int sdrl_push_event(struct sdrl_continuation *, struct sdrl_event *);
struct sdrl_event *sdrl_pop_event(struct sdrl_continuation *);
struct sdrl_event *sdrl_peek_event(struct sdrl_continuation *);

int sdrl_count_events(struct sdrl_continuation *);

#endif

