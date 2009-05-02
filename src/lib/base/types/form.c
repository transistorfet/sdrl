/*
 * Type Name:		form.c
 * Module Requirements:	(none)
 * Description:		Form (C Form) Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdFormTypeDef = {
	&sdValueTypeDef,
	"form",
	sizeof(sdPointer),
	SDRL_TBF_PASS_EXPRS,
	NULL,
	(sdrl_destroy_t) sdrl_heap_free,
	(sdrl_duplicate_t) sdrl_pointer_duplicate,
	(sdrl_evaluate_t) sdrl_base_form_evaluate
};

int sdrl_base_form_evaluate(sdMachine *mach, sdArray *args)
{
	SDRL_DECREF(mach->ret);
	mach->ret = NULL;
	return(((sdrl_func_t) SDPOINTER(args->items[0])->ptr)(mach, args));
}

