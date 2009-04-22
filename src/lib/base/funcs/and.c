/*
 * Function Name:	and.c
 * Module Requirements:	number type
 * Description:		And Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are 1, 0 otherwise.
 */
int sdrl_base_and(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	for (i = 1; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	if (args)
		mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


