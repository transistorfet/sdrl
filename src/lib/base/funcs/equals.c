/*
 * Function Name:	equals.c
 * Module Requirements:	number type
 * Description:		Equals Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are equal to eachother, 0 otherwise.
 */
int sdrl_base_equals(sdMachine *mach, sdValue *args)
{
	number_t comp, num, result = 1;
	sdValue *cur;

/*
	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type != args->type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDRL_NUMBER(cur)->num != SDNUMBER(args)->num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
*/

	SDRL_TRY(comp = sdrl_check_number(mach, args, SDRL_BT_NUMBER, NULL));
	for (cur = args->next; cur; cur = cur->next) {
		SDRL_TRY(num = sdrl_check_number(mach, cur, SDRL_BT_NUMBER, NULL));
		if (comp != num) {
			result = 0;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);

    FAIL:
	return(mach->error->err);
}


