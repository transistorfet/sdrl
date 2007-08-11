/*
 * Builtin Name:	divide.c
 * Module Requirements:	number type
 * Description:		Division Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<number>, ...
 * Description:	Returns a number of the quotient of the list of number-values passes.
 */
int sdrl_base_divide(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (value) {
		traverse_value_m("number", value->next, value->data.number, result /= cur->data.number);
	}
	sdrl_destroy_value(mach->heap, value);
	return(ret);
}


