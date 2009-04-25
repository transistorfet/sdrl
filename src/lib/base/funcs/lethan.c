/*
 * Function Name:	lethan.c
 * Module Requirements:	number type
 * Description:		Less Than Or Equal To Test Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than or equal to eachother, 0 otherwise.
 */
int sdrl_base_less_than_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 1;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	for (i = 1; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		if (SDNUMBER(args->items[i - 1])->num > SDNUMBER(args->items[i])->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, &sdNumberTypeDef, result);
	return(0);

    FAIL:
	return(mach->error->err);
}


