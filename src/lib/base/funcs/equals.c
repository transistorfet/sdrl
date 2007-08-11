/*
 * Builtin Name:	equals.c
 * Module Requirements:	number type
 * Description:		Equals Test Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are equal to eachother, 0 otherwise.
 */
int sdrl_base_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value->next, 1,
		if (cur->data.number != value->data.number) {
			result = 0;
			break;
		});

	sdrl_destroy_value(mach->heap, value);
	return(ret);
}



