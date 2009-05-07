/*
 * Function Name:	lthan.c
 * Module Requirements:	number type
 * Description:		Less Than Test Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than eachother, 0 otherwise.
 */
int sdrl_base_less_than(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	for (i = 2; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		if (SDNUMBER(args->items[i - 1])->num >= SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, result));
	return(0);

    FAIL:
	return(mach->error->err);
}


