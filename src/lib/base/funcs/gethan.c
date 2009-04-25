/*
 * Function Name:	gethan.c
 * Module Requirements:	number type
 * Description:		Greater Than Or Equal To Test Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are greater than or equal to eachother, 0 otherwise.
 */
int sdrl_base_greater_than_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	for (i = 2; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		if (SDNUMBER(args->items[i - 1])->num < SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, &sdNumberTypeDef, result);
	return(0);

    FAIL:
	return(mach->error->err);
}


