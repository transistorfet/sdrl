/*
 * Function Name:	lethan.c
 * Module Requirements:	number type
 * Description:		Less Than Or Equal To Test Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if number value are less than or equal to eachother, 0 otherwise.
 */
int sdrl_base_less_than_equals(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 1;
	struct sdrl_value *cur, *prev;

	if (!args)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	prev = args;
	for (cur = args->next; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDRL_NUMBER(prev)->num > SDRL_NUMBER(cur)->num) {
			result = 0;
			break;
		}
		else
			prev = cur;
	}
	mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


