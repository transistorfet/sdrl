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
int sdrl_base_or(struct sdrl_machine *mach, struct sdrl_value *args)
{
	number_t result = 0;
	struct sdrl_type *type;
	struct sdrl_value *cur;

	if (!(type = sdrl_find_binding(mach->type_env, "number")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	for (cur = args; cur; cur = cur->next) {
		if (cur->type != type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (SDRL_NUMBER(cur)->num) {
			result = 1;
			break;
		}
	}
	mach->ret = sdrl_make_number(mach->heap, type, result);
	return(0);
}


