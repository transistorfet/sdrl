/*
 * Builtin Name:	func.c
 * Module Requirements:	(none)
 * Description:		C Function Type
 */

#include <sdrl/sdrl.h>

static int sdrl_evaluate_func_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

struct sdrl_type *sdrl_base_make_func_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		mach->heap,
		0,
		SDRL_BT_POINTER,
		NULL,
		(sdrl_evaluate_t) sdrl_evaluate_func_type,
		NULL,
		NULL
	));
}

/*** Local Functions ***/

/**
 * Evaluate function for the func type.
 */
static int sdrl_evaluate_func_type(struct sdrl_machine *mach, struct sdrl_value *func, struct sdrl_value *args)
{
	sdrl_destroy_value(mach->heap, mach->ret);
	mach->ret = NULL;
	return(((int (*)(struct sdrl_machine *, struct sdrl_value *)) func->data.ptr)(mach, args));
}


