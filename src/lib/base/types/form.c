/*
 * Type Name:		form.c
 * Module Requirements:	(none)
 * Description:		Form (C Form) Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdFormTypeDef = {
	&sdValueTypeDef,
	sizeof(sdPointer),
	SDRL_TBF_PASS_EXPRS,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_duplicate_pointer,
	(sdrl_evaluate_t) sdrl_base_evaluate_form
};

int sdrl_base_evaluate_form(sdMachine *mach, sdArray *args)
{
	SDRL_DECREF(mach->ret);
	mach->ret = NULL;
	return(((sdrl_func_t) SDPOINTER(args->items[0])->ptr)(mach, args));
}

