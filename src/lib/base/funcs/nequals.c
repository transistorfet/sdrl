/*
 * Function Name:	nequals.c
 * Module Requirements:	number type
 * Description:		Not Equals Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value is not equal to eachother, 0 otherwise.
 */
int sdrl_base_not_equals(sdMachine *mach, sdValue *args)
{
	number_t result = 1;
	sdValue *cur;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDNUMBER(cur)->num == SDNUMBER(args)->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


