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
	int ret = 0;
	struct sdrl_value *block = NULL;

	if (sdrl_value_count(value) < 2)
		ret = SDRL_ERR_INVALID_PARAMS;
	else if (!SDRL_VALUE_IS_FALSE(value)) {
		block = value->next;
		value->next = value->next->next;
		block->next = NULL;
	}
	else if (value->next->next) {
		block = value->next->next;
		value->next->next = value->next->next->next;
		block->next = NULL;
	}

	if (block)
		sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_call_value, block, mach->env));
	sdrl_destroy_value(mach->heap, value);
	mach->ret = NULL;
	return(ret);
}



