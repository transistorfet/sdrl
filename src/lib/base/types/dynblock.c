/*
 * Type Name:		dynblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Dynamic Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdDynBlockTypeDef = {
	&sdValueTypeDef,
	"dynblock",
	sizeof(sdReference),
	0,
	NULL,
	(sdrl_destroy_t) sdrl_reference_destroy,
	(sdrl_duplicate_t) sdrl_reference_duplicate,
	(sdrl_evaluate_t) sdrl_base_dynblock_evaluate
};


int sdrl_base_dynblock_evaluate(sdMachine *mach, sdArray *args)
{
	sdEnv *env;

	if (!(env = sdrl_env_extend(mach->env)))
		return(sdrl_set_memory_error(mach));
	sdrl_env_add(env, "_", SDRL_INCREF(args));
	sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDREFERENCE(args->items[0])->ref, env);
	SDRL_DECREF(env);
	return(0);
}


