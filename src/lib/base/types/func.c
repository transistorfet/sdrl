/*
 * Type Name:		func.c
 * Module Requirements:	(none)
 * Description:		C Function Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_type *sdrl_base_make_func_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_pointer),
		0,
		SDRL_BT_POINTER,
		NULL,
		(sdrl_destroy_t) sdrl_heap_free,
		(sdrl_duplicate_t) sdrl_duplicate_pointer,
		(sdrl_evaluate_t) sdrl_base_evaluate_func
	));
}

/*** Local Functions ***/

int sdrl_base_evaluate_func(struct sdrl_machine *mach, struct sdrl_pointer *func, struct sdrl_value *args)
{
	SDRL_DESTROY_REFERENCE(mach->ret);
	mach->ret = NULL;
	return(((sdrl_func_t) func->ptr)(mach, args));
}


