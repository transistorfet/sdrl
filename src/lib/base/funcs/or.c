/*
 * Function Name:	or.c
 * Module Requirements:	number type
 * Description:		Or Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if any of the number values are 1, 0 otherwise.
 */
int sdrl_base_or(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 0;

	for (i = 1; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_type_error(mach));
		if (SDNUMBER(args->items[i])->num) {
			result = 1;
			break;
		}
	}
	if (args->last)
		mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


