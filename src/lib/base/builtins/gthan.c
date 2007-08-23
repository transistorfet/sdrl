/*
 * Builtin Name:	gthan.c
 * Module Requirements:	number type
 * Description:		Greater Than Test Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are greater than eachother, 0 otherwise.
 */
int sdrl_base_greater_than(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number <= cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	return(ret);
}




