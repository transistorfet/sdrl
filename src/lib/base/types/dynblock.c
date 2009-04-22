/*
 * Type Name:		dynblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Dynamic Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType *sdrl_base_make_dynblock_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdReference),
		0,
		SDRL_BT_REFERENCE,
		NULL,
		(sdrl_destroy_t) sdrl_destroy_reference,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		(sdrl_evaluate_t) sdrl_base_evaluate_dynblock
	));
}

int sdrl_base_evaluate_dynblock(sdMachine *mach, sdArray *args)
{
	sdEnv *env;

	if (!(env = sdrl_extend_environment(mach->env)))
		return(sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_INCREF(args));
	sdrl_push_new_event(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDREFERENCE(args->items[0])->ref, env);
	SDRL_DECREF(env);
	return(0);
}


