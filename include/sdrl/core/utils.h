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
	sdrl_add_binding(mach->env, name, sdrl_make_pointer(mach->heap, type, func))

sdValue *sdrl_next_arg(sdMachine *, sdValue **, int, sdType *);
sdValue *sdrl_next_optional_arg(sdMachine *, sdValue **, int, sdType *);

#endif

