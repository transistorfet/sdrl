/*
 * Builtin Name:	lthan.c
 * Version:		0.1
 * Module Requirements:	number type
 * Description:		Less Than Test Expression
 */

#include <sdrl.h>
#include "common/traverse.h"


/**
 * <(<value>, ...)
 * Returns 1 if number value are less than eachother, 0 otherwise.
 */
int sdrl_base_less_than(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number >= cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	sdrl_destroy_value(mach->heap, value);
	return(ret);
}




