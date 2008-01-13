/*
 * Function Name:	add.c
 * Module Requirements:	number type
 * Description:		Addition Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the sum of the list of number-values passes.
 */
int sdrl_base_add(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 0;
	struct sdrl_type *type;
	struct sdrl_value *cur;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	for (cur = args; cur; cur = cur->next) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		result += SDRL_NUMBER(cur)->num;
	}
	mach->ret = sdrl_make_number(mach->heap, type, result);
	return(0);
}


