/*
 * Function Name:	gthan.c
 * Module Requirements:	number type
 * Description:		Greater Than Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are greater than eachother, 0 otherwise.
 */
int sdrl_base_greater_than(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	if (args->last < 1)
		return(sdrl_set_args_error(mach));
	if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_type_error(mach));
	for (i = 2; i <= args->last; i++) {
		if (args->items[i]->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_type_error(mach));
		if (SDNUMBER(args->items[i - 1])->num <= SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


