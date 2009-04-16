/*
 * Function Name:	modulo.c
 * Module Requirements:	number type
 * Description:		Modulo Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<number>, ...
 * Description:	Returns a number of the remainder of the list of number-values passes.
 */
int sdrl_base_modulo(sdMachine *mach, sdValue *args)
{
	number_t result;
	sdValue *cur;

	if (!args)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = SDNUMBER(args)->num;
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!SDNUMBER(cur)->num)
			return(sdrl_set_error(mach, SDRL_ES_LOW, SDRL_ERR_DIVIDE_BY_ZERO, NULL));
		result = (int) result % (int) SDNUMBER(cur)->num;
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


