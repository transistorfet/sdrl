/*
 * Builtin Name:	nequals.c
 * Version:		0.1
 * Module Requirements:	number type
 * Description:		Not Equals Test Expression
 */

#include <sdrl/sdrl.h>
#include "common/traverse.h"


/**
 * !=(<value>, ...)
 * Returns 1 if number value is not equal to eachother, 0 otherwise.
 */
int sdrl_base_not_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value->next, 1,
		if (cur->data.number == value->data.number) {
			result = 0;
			break;
		});

	sdrl_destroy_value(mach->heap, value);
	return(ret);
}




