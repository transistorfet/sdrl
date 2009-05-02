/*
 * Name:	macros.h
 * Description:	Helpful Macros and Inline Functions for SDRL
 */

#ifndef _SDRL_UTILS_H
#define _SDRL_UTILS_H

#include <sdrl/core/env.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/value.h>
#include <sdrl/core/error.h>
#include <sdrl/core/events.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>


/*** Try a piece of code and jump to a failure point if it produces an error.  This assumes that
     there is an sdrl_machine pointer called "mach" in the current scope and a label called "FAIL". */
#define SDRL_TRY(CODE) {	\
	(CODE);			\
	if (mach->error)	\
		goto FAIL;	\
}

/*** Generate an error and set it in the current machine. ***/
static inline int sdrl_set_error(sdMachine *mach, short severity, int err, const char *msg) {
	SDRL_DECREF(mach->error);
	mach->error = sdrl_make_error(mach->heap, &sdErrorTypeDef, mach->current_line, severity, err, msg);
	return(err);
}

/*** Generate an out of memory error and set it in the machine. ***/
static inline int sdrl_set_memory_error(sdMachine *mach) {
	SDRL_DECREF(mach->error);
	mach->error = SDRL_INCREF(&sdMemoryError);
	return(0);
}

/*** Generate an invalid type error and set it in the machine. ***/
static inline int sdrl_set_type_error(sdMachine *mach, sdType *expected, sdType *received) {
	SDRL_DECREF(mach->error);
	mach->error = sdrl_make_error(mach->heap, &sdErrorTypeDef, mach->current_line,
		SDRL_ES_FATAL, SDRL_ERR_INVALID_TYPE,
		"Invalid type. Expected %s, got %s.", expected ? expected->name : "(unknown)", received ? received->name : "(unknown)");
	return(mach->error->err);
}

/*** Generate an invalid args error and set it in the machine. ***/
#define sdrl_set_args_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_INVALID_ARGS, NULL)

/*** Generate a not found error and set it in the machine. ***/
#define sdrl_set_not_found_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_NOT_FOUND, NULL)

/*** Generate an out of bounds error and set it in the machine. ***/
#define sdrl_set_bounds_error(mach) \
	sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_BOUNDS, NULL)


/**
 * Check that the array has at least min items and no more than max items.  If it
 * doesn't meet this criteria, an invalid arguments error is generated.
 */
static inline int sdrl_check_args(sdMachine *mach, sdArray *args, int min, int max) {
	if ((min != -1 && args->last < min) || (max != -1 && args->last > max))
		return(sdrl_set_args_error(mach));
	return(0);
}

/**
 * Check that the value is of the given type.  If it isn't, an invalid arguments
 * error is generated.
 */
static inline int sdrl_check_type(sdMachine *mach, sdValue *cur, sdType *type) {
	if (!sdrl_value_isa(cur, type))
		return(sdrl_set_type_error(mach, type, cur->type));
	return(0);
}


/**
 * Bind a C function to the given name and type in the current environment of
 * the given machine.
 */
#define SDRL_BIND_FUNCTION(mach, type, name, func) \
	sdrl_env_add(mach->env, name, sdrl_make_pointer(mach->heap, type, func))

/**
 * Find the item with the given name in the given environment and if an item
 * is not found, a "NOT FOUND" error will be set in the machine.
 */
static inline void *sdrl_find(sdMachine *mach, sdEnv *env, const char *name) {
	void *ptr;

	if (!(ptr = sdrl_env_find(env, name))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL);
		return(NULL);
	}
	return(ptr);
}


#endif

