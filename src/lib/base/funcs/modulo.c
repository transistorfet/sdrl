/*
 * Function Name:	modulo.c
 * Module Requirements:	number type
 * Description:		Modulo Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the remainder of the list of number-values passes.
 */
int sdrl_base_modulo(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result;

	if (args->last < 1)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = SDNUMBER(args->items[1])->num;
	for (i = 2; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!SDNUMBER(args->items[i])->num)
			return(sdrl_set_error(mach, SDRL_ES_LOW, SDRL_ERR_DIVIDE_BY_ZERO, NULL));
		result = (int) result % (int) SDNUMBER(args->items[i])->num;
	}
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


