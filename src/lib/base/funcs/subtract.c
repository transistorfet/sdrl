/*
 * Function Name:	subtract.c
 * Module Requirements:	number type
 * Description:		Subtraction Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the difference of the list of number-values passes.
 */
int sdrl_base_subtract(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 0;
	struct sdrl_value *cur;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = SDRL_NUMBER(args)->num;
	for (cur = args->next; cur; cur = cur->next) {
		if (args->type->basetype != SDRL_BT_NUMBER)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		result -= SDRL_NUMBER(cur)->num;
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


