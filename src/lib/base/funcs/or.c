/*
 * Builtin Name:	or.c
 * Module Requirements:	number type
 * Description:		Or Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if any of the number values are 1, 0 otherwise.
 */
int sdrl_base_or(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERR_NOT_FOUND);
	SDRL_FOREACH_VALUE(value, cur) {
		if (cur->type != type)
			return(SDRL_ERR_INVALID_TYPE);
		if (cur->data.number) {
			result = 1;
			break;
		}
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}



