/*
 * Name:	utils.c
 * Description:	Core Utility Functions
 */

#include <sdrl/core/utils.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/expr.h>
#include <sdrl/core/type.h>
#include <sdrl/core/error.h>
#include <sdrl/core/value.h>
#include <sdrl/core/events.h>
#include <sdrl/core/bindings.h>
#include <sdrl/core/machine.h>
#include <sdrl/globals.h>


/**
 * Return the next available value and update the pointer to the next value.
 * If there is no available value then generate an invalid arguments error.
 * If a type is given, then check that the type of the value matches the given
 * type or generate an invalid type error.
 */
sdValue *sdrl_next_arg(sdMachine *mach, sdValue **next, int basetype, sdType *type)
{
	sdValue *value;

	if (!(value = *next)) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	*next = value->next;
	if ((basetype && (value->type->basetype != basetype)) || (type && (value->type != type))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	return(value);
}

/**
 * Return the next available value and update the pointer to the next value.
 * If a type is given, then check that the type of the value matches the given
 * type or generate an invalid type error.  If an error occurs, the pointer to
 * the next will not be updated allowing for a simple test for failure.
 */
sdValue *sdrl_next_optional_arg(sdMachine *mach, sdValue **next, int basetype, sdType *type)
{
	sdValue *value;

	if (!(value = *next))
		return(NULL);
	if ((basetype && (value->type->basetype != basetype)) || (type && (value->type != type))) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	*next = value->next;
	return(value);
}


