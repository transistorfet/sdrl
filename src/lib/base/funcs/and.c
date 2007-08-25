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
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERR_NOT_FOUND);
	SDRL_FOREACH_VALUE(value, cur) {
		if (cur->type != type)
			return(SDRL_ERR_INVALID_TYPE);
		if (!cur->data.number) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}



