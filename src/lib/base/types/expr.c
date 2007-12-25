/*
 * Builtin Name:	expr.c
 * Module Requirements:	(none)
 * Description:		Expression (Code Function) Type
 */

#include <sdrl/sdrl.h>

static int sdrl_evaluate_expr_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

int sdrl_base_load_expr_type(struct sdrl_machine *mach, const char *name)
{
	struct sdrl_type *expr;

	expr = sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, NULL, (sdrl_evaluate_t) sdrl_evaluate_expr_type, NULL, NULL);
	return(sdrl_add_binding(mach->type_env, name, expr));
}

/*** Local Functions ***/

/**
 * Evaluate function for the cmdptr type.
 */
static int sdrl_evaluate_expr_type(struct sdrl_machine *mach, struct sdrl_value *expr, struct sdrl_value *params)
{
	struct sdrl_environment *env;

	if (!(env = sdrl_extend_environment(mach->env)))
		return(SDRL_ERR_OUT_OF_MEMORY);
	sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(params));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->data.ptr, env));
	SDRL_DESTROY_REFERENCE(env, sdrl_retract_environment);
	return(0);
}


