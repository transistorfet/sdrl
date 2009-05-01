/*
 * Function Name:	equals.c
 * Module Requirements:	number type
 * Description:		Equals Test Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are equal to eachother, 0 otherwise.
 */
int sdrl_base_equals(sdMachine *mach, sdArray *args)
{
	int i;
	number_t comp, num, result = 1;

	SDRL_TRY(comp = sdrl_check_number(mach, args->items[1], &sdNumberTypeDef));
	for (i = 1; i <= args->last; i++) {
		SDRL_TRY(num = sdrl_check_number(mach, args->items[i], &sdNumberTypeDef));
		if (comp != num) {
			result = 0;
			break;
		}
	}
	mach->ret = SDVALUE(sdrl_make_number(mach->heap, &sdNumberTypeDef, result));
	return(0);

    FAIL:
	return(mach->error->err);
}


