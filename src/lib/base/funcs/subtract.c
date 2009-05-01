/*
 * Function Name:	subtract.c
 * Module Requirements:	number type
 * Description:		Subtraction Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the difference of the list of number-values passes.
 */
int sdrl_base_subtract(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result = 0;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	result = SDNUMBER(args->items[1])->num;
	for (i = 2; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		result -= SDNUMBER(args->items[i])->num;
	}
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, args->items[1]->type, result));
	return(0);

    FAIL:
	return(mach->error->err);
}


