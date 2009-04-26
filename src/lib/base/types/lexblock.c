/*
 * Type Name:		lexblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Lexical Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdLexBlockTypeDef = {
	&sdValueTypeDef,
	sizeof(sdLexBlock),
	0,
	(sdrl_create_t) sdrl_base_lexblock_create,
	(sdrl_destroy_t) sdrl_base_lexblock_destroy,
	(sdrl_duplicate_t) sdrl_base_lexblock_duplicate,
	(sdrl_evaluate_t) sdrl_base_lexblock_evaluate
};


sdValue *sdrl_base_lexblock_create(sdMachine *mach, sdType *type, sdArray *args)
{
	sdLexBlock *value;

	// TODO should you check that args is an expr reference type first?
	if (!(value = (sdLexBlock *) sdrl_heap_alloc(mach->heap, type->size)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = type;
	value->code = SDEXPR(SDRL_INCREF(args->items[0]));
	value->env = SDRL_INCREF(mach->env);
	return(SDVALUE(value));
}

void sdrl_base_lexblock_destroy(sdLexBlock *value)
{
	SDRL_DECREF(value->code);
	SDRL_DECREF(value->env);
	sdrl_heap_free(value);
}

sdValue *sdrl_base_lexblock_duplicate(sdMachine *mach, sdLexBlock *org)
{
	sdLexBlock *value;

	// TODO should you check the expr type first?
	if (!(value = (sdLexBlock *) sdrl_heap_alloc(mach->heap, SDVALUE(org)->type->size)))
		return(NULL);
	SDVALUE(value)->refs = 1;
	SDVALUE(value)->type = SDVALUE(org)->type;
	value->code = SDEXPR(SDRL_INCREF(org->code));
	value->env = SDRL_INCREF(org->env);
	return(SDVALUE(value));
}

int sdrl_base_lexblock_evaluate(sdMachine *mach, sdArray *args)
{
	sdEnv *env;
	sdLexBlock *func;

	func = SDLEXBLOCK(args->items[0]);
	if (!(env = sdrl_env_extend(func->env)))
		return(sdrl_set_memory_error(mach));
	sdrl_env_add(env, "_", SDRL_INCREF(args));
	sdrl_event_push_new(mach->cont, (sdrl_event_t) sdrl_evaluate_expr_list, SDVALUE(func->code), env);
	SDRL_DECREF(env);
	return(0);
}


