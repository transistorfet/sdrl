/*
 * Function Name:	not.c
 * Module Requirements:	number type
 * Description:		Not Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>
 * Description:	Returns 1 if all number value is 0, 0 otherwise.
 */
int sdrl_base_not(sdMachine *mach, sdArray *args)
{
	number_t result = 1;

	if (args->last < 1)
		return(sdrl_set_args_error(mach));
	else if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_type_error(mach));
	result = (SDNUMBER(args->items[1])->num == 0);
	mach->ret = sdrl_make_number(mach->heap, args->items[1]->type, result);
	return(0);
}


