/*
 * Builtin Name:	or.c
 * Module Requirements:	number type
 * Description:		Or Boolean Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<value>, ...
 * Description:	Returns 1 if any of the number values are 1, 0 otherwise.
 */
int sdrl_base_or(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value, 0,
		if (cur->data.number) {
			result = 1;
			break;
		});

	return(ret);
}



