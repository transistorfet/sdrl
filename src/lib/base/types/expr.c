/*
 * Type Name:		expr.c
 * Module Requirements:	(none)
 * Description:		AST Expression Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType *sdrl_base_make_expr_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdReference),
		0,
		SDRL_BT_REFERENCE,
		(sdrl_create_t) sdrl_base_create_expr,
		(sdrl_destroy_t) sdrl_destroy_reference,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		(sdrl_evaluate_t) sdrl_base_evaluate_expr
	));
}


sdValue *sdrl_base_create_expr(sdMachine *mach, sdType *type, sdValue *args)
{
	if (!args) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	// TODO check that args is an expr type
	return(sdrl_make_reference(mach->heap, type, args));
}

int sdrl_base_evaluate_expr(sdMachine *mach, sdReference *expr, sdValue *args)
{
	// TODO what do you do (if anything) with the arguments?
	SDRL_INCREF(args);
	if (sdrl_add_binding(mach->env, "_", args))
		sdrl_replace_binding(mach->env, "_", args);
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, expr->ref, mach->env));
	return(0);
}


