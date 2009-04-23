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
int sdrl_base_if(sdMachine *mach, sdArray *args)
{
	sdArray *block;

	if (args->last < 2 || args->last > 3)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (args->items[1]->type->basetype != SDRL_BT_NUMBER)
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
	else if (!(block = sdrl_make_array(mach->heap, sdrl_env_find(mach->type_env, "array"), 1)))
		return(sdrl_set_error(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
	// TODO convert this to to more than just number comparison
	else if (SDNUMBER(args->items[1])->num)
		sdrl_array_push(block, sdrl_duplicate_value(mach, args->items[2]));
	else if (args->items[3])
		sdrl_array_push(block, sdrl_duplicate_value(mach, args->items[3]));

	if (block->last >= 0)
		sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(block), mach->env);
	mach->ret = NULL;
	return(0);
}


