/*
 * Builtin Name:	and.c
 * Module Requirements:	number type
 * Description:		And Boolean Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are 1, 0 otherwise.
 */
int sdrl_base_and(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value, 1,
		if (!cur->data.number) {
			result = 0;
			break;
		});

	return(ret);
}



