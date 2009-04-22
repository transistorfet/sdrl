/*
 * Type Name:		env.c
 * Module Requirements:	(none)
 * Description:		Environment Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType *sdrl_base_make_env_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdReference),
		0,
		SDRL_BT_REFERENCE,
		(sdrl_create_t) sdrl_base_create_env,
		(sdrl_destroy_t) sdrl_destroy_reference,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		NULL
	));
}

sdValue *sdrl_base_create_env(sdMachine *mach, sdType *type, sdArray *args)
{
	sdEnv *env;
	sdType *env_type;

	if (args->last < 0) {
		if (!(env_type = sdrl_find_binding(mach->type_env, "*env*")))
			sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL);
		env = sdrl_create_environment(mach->heap, env_type, 0, (sdrl_destroy_t) sdrl_destroy_value);
	}
	else if (args->items[0]->type != type) {
		sdrl_set_error(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	else
		env = sdrl_extend_environment(SDENV(SDREFERENCE(args->items[0])->ref));
	return(sdrl_make_reference(mach->heap, type, SDVALUE(env)));
}


