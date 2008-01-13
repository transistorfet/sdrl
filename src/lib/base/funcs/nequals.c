/*
 * Function Name:	nequals.c
 * Module Requirements:	number type
 * Description:		Not Equals Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value is not equal to eachother, 0 otherwise.
 */
int sdrl_base_not_equals(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 1;
	struct sdrl_type *type;
	struct sdrl_value *cur;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	if (args->type != type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDRL_NUMBER(cur)->num == SDRL_NUMBER(args)->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, type, result);
	return(0);
}


