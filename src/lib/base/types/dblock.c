/*
 * Type Name:		dblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Dynamic Scoping
 */

#include <sdrl/sdrl.h>

static int sdrl_evaluate_dblock_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

struct sdrl_type *sdrl_base_make_dblock_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		mach->heap,
		0,
		SDRL_BT_POINTER,
		NULL,
		(sdrl_evaluate_t) sdrl_evaluate_dblock_type,
		NULL,
		NULL
	));
}

/*** Local Functions ***/

/**
 * Extend the current environment (mach->env), set the '_' binding in the new
 * environment to a reference to the given args, and push an event to evaluate
 * the given AST in the new environment.  If an error occurs then an error
 * code is returned; otherwise 0 is returned.
 */
static int sdrl_evaluate_dblock_type(struct sdrl_machine *mach, struct sdrl_value *expr, struct sdrl_value *args)
{
	struct sdrl_environment *env;

	if (!(env = sdrl_extend_environment(mach->env)))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->data.ptr, env));
	SDRL_DESTROY_REFERENCE(env, sdrl_retract_environment);
	return(0);
}


