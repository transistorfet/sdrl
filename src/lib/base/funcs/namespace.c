/*
 * Function Name:	namespace.c
 * Module Requirements:	(none)
 * Description:		Namespace Expression
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

/**
 * Args:	<string>|<env>, <expr-value>
 * Description:	Evaluate the given expression under the appropriate environment.
 */
int sdrl_base_namespace(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdArray *block;

	SDRL_TRY(sdrl_check_args(mach, args, 2, 2));
	if (sdrl_value_isa(args->items[1], &sdStringTypeDef)) {
		if (!(env = sdrl_env_find(mach->env, SDSTRING(args->items[1])->str))
		    && (!(env = sdrl_env_extend(mach->env, &sdEnvTypeDef))
		    || sdrl_env_add(mach->env, SDSTRING(args->items[1])->str, env)))
			return(sdrl_set_memory_error(mach));
		SDRL_TRY(sdrl_check_type(mach, SDVALUE(env), &sdEnvTypeDef));
	}
	else if (sdrl_value_isa(args->items[1], &sdEnvTypeDef))
		env = SDENV(args->items[1]);
	else
		return(sdrl_set_type_error(mach, &sdStringTypeDef, args->items[1]->type));

	if (!(block = sdrl_make_array(mach, &sdArrayTypeDef, 1)))
		return(sdrl_set_memory_error(mach));
	sdrl_array_push(block, sdrl_duplicate_value(mach, args->items[2]));
	sdrl_event_push(mach->cont, (sdrl_event_t) sdrl_evaluate_value, SDVALUE(block), env);
	return(0);

    FAIL:
	return(mach->error->err);
}


