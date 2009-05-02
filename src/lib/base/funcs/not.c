/*
 * Function Name:	not.c
 * Module Requirements:	number type
 * Description:		Not Boolean Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>
 * Description:	Returns 1 if all number value is 0, 0 otherwise.
 */
int sdrl_base_not(sdMachine *mach, sdArray *args)
{
	number_t result = 1;

	if (args->last < 1)
		return(sdrl_set_args_error(mach));
	else if (!sdrl_value_isa(args->items[1], &sdNumberTypeDef))
		return(sdrl_set_type_error(mach, &sdNumberTypeDef, args->items[1]->type));
	result = (SDNUMBER(args->items[1])->num == 0);
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, result));
	return(0);
}


