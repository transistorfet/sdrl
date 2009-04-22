/*
 * Function Name:	lethan.c
 * Module Requirements:	number type
 * Description:		Less Than Or Equal To Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than or equal to eachother, 0 otherwise.
 */
int sdrl_base_less_than_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	if (args->last < 1)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	for (i = 1; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDNUMBER(args->items[i - 1])->num > SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


