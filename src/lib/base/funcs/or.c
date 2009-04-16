/*
 * Function Name:	or.c
 * Module Requirements:	number type
 * Description:		Or Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if any of the number values are 1, 0 otherwise.
 */
int sdrl_base_or(sdMachine *mach, sdValue *args)
{
	number_t result = 0;
	sdValue *cur;

	for (cur = args; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDNUMBER(cur)->num) {
			result = 1;
			break;
		}
	}
	if (args)
		mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


