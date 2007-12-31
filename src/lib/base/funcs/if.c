/*
 * Builtin Name:	if.c
 * Module Requirements:	(none)
 * Description:		If Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, <expr-value> [, <expr-value>]
 * Description:	Evaluates the first expr-value if value is not 0, the second otherwise.
 */
int sdrl_base_if(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *block = NULL;

	if (sdrl_value_count(value) < 2)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (!SDRL_VALUE_IS_FALSE(value))
		block = sdrl_duplicate_single_value(mach->heap, value->next);
	else if (value->next->next)
		block = sdrl_duplicate_single_value(mach->heap, value->next->next);

	if (block) {
		sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_destroy_reference, block, mach->env));
		sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_call_value, block, mach->env));
	}
	mach->ret = NULL;
	return(0);
}



