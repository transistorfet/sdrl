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
struct sdrl_value *sdrl_next_arg_checked(struct sdrl_machine *mach, struct sdrl_value **next, struct sdrl_type *type)
{
	struct sdrl_value *value;

	if (!(value = *next)) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL);
		return(NULL);
	}
	*next = value->next;
	if (type && (value->type != type)) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
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
struct sdrl_value *sdrl_next_arg_optional(struct sdrl_machine *mach, struct sdrl_value **next, struct sdrl_type *type)
{
	struct sdrl_value *value;

	if (!(value = *next))
		return(NULL);
	if (type && (value->type != type)) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	*next = value->next;
	return(value);
}

