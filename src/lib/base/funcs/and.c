/*
 * Function Name:	and.c
 * Module Requirements:	number type
 * Description:		And Boolean Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are 1, 0 otherwise.
 */
int sdrl_base_and(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	for (i = 1; i <= args->last; i++) {
		if (!sdrl_value_isa(args->items[i], &sdNumberTypeDef))
			return(sdrl_set_type_error(mach, &sdNumberTypeDef, args->items[i]->type));
		if (!SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, result));
	return(0);
}


