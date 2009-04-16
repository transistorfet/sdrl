/*
 * Function Name:	if.c
 * Module Requirements:	(none)
 * Description:		If Expression
 */

#include <sdrl/sdrl.h>

/**
 * Args:	<value>, <expr-value> [, <expr-value>]
 * Description:	Evaluates the first expr-value if value is not 0, the second otherwise.
 */
int sdrl_base_if(sdMachine *mach, sdValue *args)
{
	sdValue *block = NULL;

	if (sdrl_value_count(args) < 2)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (args->type->basetype != SDRL_BT_NUMBER)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	// TODO convert this to to more than just number comparison
	else if (SDNUMBER(args)->num)
		block = sdrl_duplicate_single_value(mach, args->next);
	else if (args->next->next)
		block = sdrl_duplicate_single_value(mach, args->next->next);

	if (block)
		sdrl_push_event(mach->cont, sdrl_make_event(SDRL_EBF_USE_RET, (sdrl_event_t) sdrl_evaluate_value, block, mach->env));
	mach->ret = NULL;
	return(0);
}


