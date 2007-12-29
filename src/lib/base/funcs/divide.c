/*
 * Builtin Name:	divide.c
 * Module Requirements:	number type
 * Description:		Division Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the quotient of the list of number-values passes.
 */
int sdrl_base_divide(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t result;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (!value)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (value->type != type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = value->data.number;
	SDRL_FOREACH_VALUE(value->next, cur) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!cur->data.number)
			return(SDRL_ERROR(mach, SDRL_ES_LOW, SDRL_ERR_DIVIDE_BY_ZERO, NULL));
		result /= cur->data.number;
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}


