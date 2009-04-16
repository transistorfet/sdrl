/*
 * Function Name:	divide.c
 * Module Requirements:	number type
 * Description:		Division Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the quotient of the list of number-values passes.
 */
int sdrl_base_divide(sdMachine *mach, sdValue *args)
{
	number_t result;
	sdValue *cur;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = SDNUMBER(args)->num;
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!SDNUMBER(cur)->num)
			return(SDRL_ERROR(mach, SDRL_ES_LOW, SDRL_ERR_DIVIDE_BY_ZERO, NULL));
		result /= SDNUMBER(cur)->num;
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


