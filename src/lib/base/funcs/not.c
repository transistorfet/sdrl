/*
 * Function Name:	not.c
 * Module Requirements:	number type
 * Description:		Not Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>
 * Description:	Returns 1 if all number value is 0, 0 otherwise.
 */
int sdrl_base_not(sdMachine *mach, sdValue *args)
{
	number_t result = 1;

	if (!args || args->next)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	result = (SDNUMBER(args)->num == 0);
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


