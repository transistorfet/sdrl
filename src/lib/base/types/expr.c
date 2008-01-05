/*
 * Type Name:		expr.c
 * Module Requirements:	(none)
 * Description:		AST Expression Type
 */

#include <sdrl/sdrl.h>

static int sdrl_evaluate_expr_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

struct sdrl_type *sdrl_base_make_expr_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		mach->heap,
		0,
		SDRL_BT_POINTER,
		NULL,
		(sdrl_evaluate_t) sdrl_evaluate_expr_type,
		NULL,
		NULL
	));
}

/*** Local Functions ***/

/**
 * Evaluate the given expr
 */
static int sdrl_evaluate_expr_type(struct sdrl_machine *mach, struct sdrl_value *expr, struct sdrl_value *args)
{
	// TODO what do you do (if anything) with the arguments?
	//sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->data.ptr, mach->env));
	return(0);
}


