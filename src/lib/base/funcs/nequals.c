/*
 * Function Name:	nequals.c
 * Module Requirements:	number type
 * Description:		Not Equals Test Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value is not equal to eachother, 0 otherwise.
 */
int sdrl_base_not_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	for (i = 2; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		if (SDNUMBER(args->items[i])->num == SDNUMBER(args->items[1])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, &sdNumberTypeDef, result);
	return(0);

    FAIL:
	return(mach->error->err);
}


