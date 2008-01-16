/*
 * Function Name:	and.c
 * Module Requirements:	number type
 * Description:		And Boolean Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, ...
 * Description:	Returns 1 if all number values are 1, 0 otherwise.
 */
int sdrl_base_and(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 1;
	struct sdrl_value *cur;

	for (cur = args; cur; cur = cur->next) {
		if (cur->type->basetype != SDRL_BT_NUMBER)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (!SDRL_NUMBER(cur)->num) {
			result = 0;
			break;
		}
	}
	if (args)
		mach->ret = sdrl_make_number(mach->heap, args->type, result);
	return(0);
}


