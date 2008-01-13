/*
 * Function Name:	lthan.c
 * Module Requirements:	number type
 * Description:		Less Than Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than eachother, 0 otherwise.
 */
int sdrl_base_less_than(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 1;
	struct sdrl_type *type;
	struct sdrl_value *cur, *prev;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (args->type != type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	prev = args;
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDRL_NUMBER(prev)->num >= SDRL_NUMBER(cur)->num) {
			result = 0;
			break;
		}
		else
			prev = cur;
	}
	mach->ret = sdrl_make_number(mach->heap, type, result);
	return(0);
}


