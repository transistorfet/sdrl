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
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (!value)
		return(SDRL_ERR_INVALID_PARAMS);
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERR_NOT_FOUND);
	if (value->type != type)
		return(SDRL_ERR_INVALID_TYPE);
	SDRL_FOREACH_VALUE(value->next, cur) {
		if (cur->type != value->type)
			return(SDRL_ERR_INVALID_TYPE);
		if (cur->data.number != value->data.number) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}



