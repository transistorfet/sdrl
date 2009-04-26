/*
 * Type Name:		func.c
 * Module Requirements:	(none)
 * Description:		C Function Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdFuncTypeDef = {
	&sdValueTypeDef,
	sizeof(sdPointer),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_pointer_duplicate,
	(sdrl_evaluate_t) sdrl_base_func_evaluate
};


int sdrl_base_func_evaluate(sdMachine *mach, sdArray *args)
{
	SDRL_DECREF(mach->ret);
	mach->ret = NULL;
	return(((sdrl_func_t) SDPOINTER(args->items[0])->ptr)(mach, args));
}


