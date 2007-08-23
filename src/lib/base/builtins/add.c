/*
 * Builtin Name:	add.c
 * Module Requirements:	number type
 * Description:		Addition Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<number>, ...
 * Description:	Returns a number of the sum of the list of number-values passes.
 */
int sdrl_base_add(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value, 0, result += cur->data.number);
	return(ret);
}


