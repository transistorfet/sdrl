/*
 * Function Name:	if.c
 * Module Requirements:	(none)
 * Description:		If Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<value>, <expr-value> [, <expr-value>]
 * Description:	Evaluates the first expr-value if value is not 0, the second otherwise.
 */
int sdrl_base_if(sdMachine *mach, sdArray *args)
{
	sdArray *block;

	SDRL_TRY(sdrl_check_args(mach, args, 2, 3));
	SDRL_TRY(sdrl_check_type(mach, args->items[1], &sdNumberTypeDef));
	if (!(block = sdrl_make_array(mach, &sdArrayTypeDef, 1)))
		return(sdrl_set_memory_error(mach));
	// TODO convert this to to more than just number comparison
	else if (SDNUMBER(args->items[1])->num)
		sdrl_array_push(block, sdrl_duplicate_value(mach, args->items[2]));
	else if (args->items[3])
		sdrl_array_push(block, sdrl_duplicate_value(mach, args->items[3]));

	if (block->last >= 0)
		sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(block), mach->env);
	mach->ret = NULL;
	return(0);

    FAIL:
	return(mach->error->err);
}


