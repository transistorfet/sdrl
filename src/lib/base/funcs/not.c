/*
 * Function Name:	not.c
 * Module Requirements:	number type
 * Description:		Not Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>
 * Description:	Returns 1 if all number value is 0, 0 otherwise.
 */
int sdrl_base_not(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 1;
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else if (sdrl_value_count(args) != 1)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (args->type != type)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = (SDRL_NUMBER(args)->num == 0);
	mach->ret = sdrl_make_number(mach->heap, type, result);
	return(0);
}


