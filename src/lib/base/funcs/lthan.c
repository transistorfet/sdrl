/*
 * Builtin Name:	lthan.c
 * Module Requirements:	number type
 * Description:		Less Than Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than eachother, 0 otherwise.
 */
int sdrl_base_less_than(struct sdrl_machine *mach, struct sdrl_value *value)
{
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	if (!value)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (value->type != type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	last = value;
	SDRL_FOREACH_VALUE(value->next, cur) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (last->data.number >= cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;
	}
	mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	return(0);
}




