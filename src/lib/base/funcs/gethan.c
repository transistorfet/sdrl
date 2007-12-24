/*
 * Builtin Name:	gethan.c
 * Module Requirements:	number type
 * Description:		Greater Than Or Equal To Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are greater than or equal to eachother, 0 otherwise.
 */
int sdrl_base_greater_than_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	if (!value)
		return(SDRL_ERR_INVALID_PARAMS);
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERR_NOT_FOUND);
	if (value->type != type)
		return(SDRL_ERR_INVALID_TYPE);
	last = value;
	SDRL_FOREACH_VALUE(value->next, cur) {
		if (cur->type != type)
			return(SDRL_ERR_INVALID_TYPE);
		if (last->data.number < cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}




