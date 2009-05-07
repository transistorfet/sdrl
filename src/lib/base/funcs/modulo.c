/*
 * Function Name:	modulo.c
 * Module Requirements:	number type
 * Description:		Modulo Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the remainder of the list of number-values passes.
 */
int sdrl_base_modulo(sdMachine *mach, sdArray *args)
{
	int i;
	number_t result;

	SDRL_TRY(sdrl_check_args(mach, args, 1, -1));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	result = SDNUMBER(args->items[1])->num;
	for (i = 2; i <= args->last; i++) {
		SDRL_TRY(sdrl_check_type(mach, args->items[i], &sdNumberTypeDef));
		if (!SDNUMBER(args->items[i])->num)
			return(sdrl_set_error(mach, SDRL_ES_LOW, SDRL_ERR_DIVIDE_BY_ZERO, NULL));
		result = (int) result % (int) SDNUMBER(args->items[i])->num;
	}
	mach->ret = SDVALUE(sdrl_make_number(mach, &sdNumberTypeDef, result));
	return(0);

    FAIL:
	return(mach->error->err);
}


