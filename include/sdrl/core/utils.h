/*
 * Name:	macros.h
 * Description:	Helpful Macros and Inline Functions for SDRL
 */


#ifndef _SDRL_UTILS_H
#define _SDRL_UTILS_H

#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/type.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/core/events.h>
#include <sdrl/core/bindings.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>

/**
 * Bind a C function to the given name and type in the current environment of
 * the given machine.
 */
#define SDRL_BIND_FUNCTION(mach, type, name, func) \
	sdrl_add_binding(mach->env, name, sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) func, 0, NULL));

/**
 * Return the next available value and update the pointer to the next value.
 */
static inline struct sdrl_value *sdrl_next_arg(struct sdrl_value **next) {
	struct sdrl_value *value;

	if (!(value = *next))
		return(NULL);
	*next = value->next;
	return(value);
}

struct sdrl_value *sdrl_next_arg_check(struct sdrl_machine *, struct sdrl_value **, struct sdrl_type *);

#endif

