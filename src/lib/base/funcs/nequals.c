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
int sdrl_base_not_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	if (args->last < 1)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	for (i = 2; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDNUMBER(args->items[i])->num == SDNUMBER(args->items[1])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


