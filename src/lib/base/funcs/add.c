/*
 * Function Name:	add.c
 * Module Requirements:	number type
 * Description:		Addition Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the sum of the list of number-values passes.
 */
int sdrl_base_add(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 0;

	for (i = 1; i <= args->last; i++) {
		if (!sdrl_value_isa(args->items[i], &sdNumberTypeDef))
			return(sdrl_set_type_error(mach));
		result += SDNUMBER(args->items[i])->num;
	}
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, result));
	return(0);
}


