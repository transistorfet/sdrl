/*
 * Type Name:		env.c
 * Module Requirements:	(none)
 * Description:		Environment Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_type *sdrl_base_make_env_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_reference),
		0,
		SDRL_BT_POINTER,
		(sdrl_create_t) sdrl_base_create_env,
		(sdrl_destroy_t) sdrl_destroy_reference,
		(sdrl_duplicate_t) sdrl_duplicate_reference,
		NULL
	));
}

struct sdrl_value *sdrl_base_create_env(struct sdrl_machine *mach, struct sdrl_type *type, struct sdrl_value *args)
{
	struct sdrl_type *env_type;
	struct sdrl_environment *env;

	// TODO find the *env* type and pass it
	if (!args) {
		if (!(env_type = sdrl_find_binding(mach->type_env, "*env*")))
			SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL);
		env = sdrl_create_environment(mach->heap, env_type, 0, (sdrl_destroy_t) sdrl_destroy_value);
	}
	else if (args->type != type) {
		SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL);
		return(NULL);
	}
	else
		env = sdrl_extend_environment((struct sdrl_environment *) SDRL_REFERENCE(args)->ref);
	return(sdrl_make_reference(mach->heap, type, SDRL_VALUE(env)));
}


