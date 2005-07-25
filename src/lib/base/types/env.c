/*
 * Builtin Name:	env.c
 * Version:		0.1
 * Module Requirements:	(none)
 * Description:		Environment Type
 */

#include <sdrl/sdrl.h>

static void *sdrl_create_env_type(struct sdrl_machine *, struct sdrl_value *);
static void *sdrl_duplicate_env_type(struct sdrl_heap *, void *);
static int sdrl_destroy_env_type(struct sdrl_heap *, void *);

int sdrl_base_load_env_type(struct sdrl_machine *mach, char *name)
{
	struct sdrl_type *env;

	env = sdrl_make_type(mach->heap, 0, SDRL_BT_POINTER, (sdrl_create_t) sdrl_create_env_type, NULL, (sdrl_duplicate_t) sdrl_duplicate_env_type, (sdrl_destroy_t) sdrl_destroy_env_type);
	return(sdrl_add_binding(mach->type_env, name, env));
}

/*** Local Functions ***/

/**
 * Create an environment.
 */
static void *sdrl_create_env_type(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	struct sdrl_type *env_type;
	struct sdrl_environment *env;

	if (value) {
		if (!(env_type = sdrl_find_binding(mach->type_env, "env")))
			ret = ERR_NOT_FOUND;
		else if (value->type != env_type)
			ret = ERR_INVALID_TYPE;
		else
			env = sdrl_extend_environment((struct sdrl_environment *) value->data.ptr);
	}
	else
		env = sdrl_create_environment(0, mach->heap, (sdrl_destroy_t) sdrl_destroy_value);
	sdrl_destroy_value(mach->heap, value);
	if (ret)
		return(NULL);
	return((void *) env);
}

/**
 * Make a reference to the environment and return the pointer.
 */
static void *sdrl_duplicate_env_type(struct sdrl_heap *heap, void *env)
{
	return((void *) sdrl_make_reference_m(((struct sdrl_environment *) env)));
}

/**
 * Destroy the environment.
 */
static int sdrl_destroy_env_type(struct sdrl_heap *heap, void *ptr)
{
	sdrl_retract_environment((struct sdrl_environment *) ptr);
	return(0);
}

