/*
 * Function Name:	gthan.c
 * Module Requirements:	number type
 * Description:		Greater Than Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are greater than eachother, 0 otherwise.
 */
int sdrl_base_greater_than(sdMachine *mach, sdValue *args)
{
	number_t result = 1;
	sdValue *cur, *prev;

	if (!args)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	prev = args;
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDNUMBER(prev)->num <= SDNUMBER(cur)->num) {
			result = 0;
			break;
		}
		else
			prev = cur;
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


