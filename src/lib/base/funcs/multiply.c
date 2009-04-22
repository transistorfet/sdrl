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
int sdrl_base_multiply(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	for (i = 1; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		result *= SDNUMBER(args->items[i])->num;
	}
	if (args->last)
		mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


