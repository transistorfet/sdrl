/*
 * Type Name:		dynblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Dynamic Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_type *sdrl_base_make_dynblock_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_reference),
		0,
		SDRL_BT_REFERENCE,
		NULL,
		(sdrl_destroy_t) sdrl_destroy_reference,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		(sdrl_evaluate_t) sdrl_base_evaluate_dynblock
	));
}

int sdrl_base_evaluate_dynblock(struct sdrl_machine *mach, struct sdrl_reference *expr, struct sdrl_value *args)
{
	struct sdrl_environment *env;

	if (!(env = sdrl_extend_environment(mach->env)))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->ref, env));
	SDRL_DESTROY_REFERENCE(env);
	return(0);
}


