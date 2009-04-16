/*
 * Function Name:	multiply.c
 * Module Requirements:	number type
 * Description:		Multiplication Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the product of the list of number-values passes.
 */
int sdrl_base_multiply(sdMachine *mach, sdValue *args)
{
	number_t result = 1;
	sdValue *cur;

	for (cur = args; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		result *= SDNUMBER(cur)->num;
	}
	if (args)
		mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


