/*
 * Function Name:	or.c
 * Module Requirements:	number type
 * Description:		Or Boolean Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if any of the number values are 1, 0 otherwise.
 */
int sdrl_base_or(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 0;

	for (i = 1; i <= args->last; i++) {
		if (!sdrl_value_isa(args->items[i], &sdNumberTypeDef))
			return(sdrl_set_type_error(mach, &sdNumberTypeDef, args->items[i]->type));
		if (SDNUMBER(args->items[i])->num) {
			result = 1;
			break;
		}
	}
	mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, result));
	return(0);
}


