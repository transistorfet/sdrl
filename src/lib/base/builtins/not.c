/*
 * Builtin Name:	not.c
 * Module Requirements:	number type
 * Description:		Not Boolean Expression
 */

#include <sdrl/sdrl.h>
#include "../common/traverse.h"


/**
 * Args:	<value>
 * Description:	Returns 1 if all number value is 0, 0 otherwise.
 */
int sdrl_base_not(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_type *type;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		ret = SDRL_ERR_NOT_FOUND;
	else if (sdrl_value_count(value) != 1)
		ret = SDRL_ERR_INVALID_PARAMS;
	else if (value->type != type)
		ret = SDRL_ERR_INVALID_TYPE;
	else {
		result = (value->data.number == 0);
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);
	}
	sdrl_destroy_value(mach->heap, value);
	return(ret);
}




