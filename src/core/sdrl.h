/*
 * Name:	sdrl.h
 * Version:	0.2
 * Description:	SDRL Header
 */


#ifndef _SDRL_H
#define _SDRL_H

#include "heap.h"
#include "expr.h"
#include "type.h"
#include "input.h"
#include "value.h"
#include "parse.h"
#include "events.h"
#include "bindings.h"
#include "machine.h"
#include "globals.h"

#define sdrl_bind_function_m(mach, type, name, func) \
	sdrl_add_binding(mach->env, name, sdrl_make_value(mach->heap, type, (sdrl_data_t) (void *) func, 0, NULL));

#endif

