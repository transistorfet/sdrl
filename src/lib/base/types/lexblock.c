/*
 * Type Name:		lexblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Lexical Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType *sdrl_base_make_lexblock_type(sdMachine *mach)
{
	return(sdrl_make_type(
		sizeof(sdLexBlock),
		0,
		SDRL_BT_DATA,
		(sdrl_create_t) sdrl_base_create_lexblock,
		(sdrl_destroy_t) sdrl_base_destroy_lexblock,
		(sdrl_duplicate_t) sdrl_base_duplicate_lexblock,
		(sdrl_evaluate_t) sdrl_base_evaluate_lexblock
	));
}

sdValue *sdrl_base_create_lexblock(sdMachine *mach, sdType *type, sdValue *args)
{
	sdLexBlock *value;

	// TODO should you check that args is an expr reference type first?
	if (!(value = (sdLexBlock *) sdrl_heap_alloc(mach->heap, sizeof(sdLexBlock))))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	SDVALUE(value)->next = NULL;
	value->code = SDEXPR(SDRL_INCREF(args));
	value->env = SDRL_INCREF(mach->env);
	return(SDVALUE(value));
}

int sdrl_base_destroy_lexblock(sdLexBlock *value)
{
	SDRL_DECREF(value->code);
	SDRL_DECREF(value->env);
	sdrl_heap_free(value);
	return(0);
}

sdValue *sdrl_base_duplicate_lexblock(sdMachine *mach, sdLexBlock *org)
{
	sdLexBlock *value;

	// TODO should you check the expr type first?
	if (!(value = (sdLexBlock *) sdrl_heap_alloc(mach->heap, sizeof(sdLexBlock))))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = SDVALUE(org)->type;
	SDVALUE(value)->next = NULL;
	value->code = SDEXPR(SDRL_INCREF(org->code));
	value->env = SDRL_INCREF(org->env);
	return(SDVALUE(value));
}

int sdrl_base_evaluate_lexblock(sdMachine *mach, sdLexBlock *func, sdValue *args)
{
	sdEnv *env;

	if (!(env = sdrl_extend_environment(func->env)))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_INCREF(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(func->code), env));
	SDRL_DECREF(env);
	return(0);
}


