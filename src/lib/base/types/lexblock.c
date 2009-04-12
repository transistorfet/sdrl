/*
 * Type Name:		lexblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Lexical Scoping
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

struct sdrl_type *sdrl_base_make_lexblock_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		sizeof(struct sdrl_lexblock),
		0,
		SDRL_BT_DATA,
		(sdrl_create_t) sdrl_base_create_lexblock,
		(sdrl_destroy_t) sdrl_base_destroy_lexblock,
		(sdrl_duplicate_t) sdrl_base_duplicate_lexblock,
		(sdrl_evaluate_t) sdrl_base_evaluate_lexblock
	));
}

struct sdrl_value *sdrl_base_create_lexblock(struct sdrl_machine *mach, struct sdrl_type *type, struct sdrl_value *args)
{
	struct sdrl_lexblock *value;

	// TODO should you check that args is an expr reference type first?
	if (!(value = (struct sdrl_lexblock *) sdrl_heap_alloc(mach->heap, sizeof(struct sdrl_lexblock))))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = type;
	SDRL_VALUE(value)->next = NULL;
	value->code = SDRL_EXPR(SDRL_MAKE_REFERENCE(args));
	value->env = SDRL_MAKE_REFERENCE(mach->env);
	return(SDRL_VALUE(value));
}

int sdrl_base_destroy_lexblock(struct sdrl_lexblock *value)
{
	SDRL_DESTROY_REFERENCE(value->code);
	SDRL_DESTROY_REFERENCE(value->env);
	sdrl_heap_free(value);
	return(0);
}

struct sdrl_value *sdrl_base_duplicate_lexblock(struct sdrl_machine *mach, struct sdrl_lexblock *org)
{
	struct sdrl_lexblock *value;

	// TODO should you check the expr type first?
	if (!(value = (struct sdrl_lexblock *) sdrl_heap_alloc(mach->heap, sizeof(struct sdrl_lexblock))))
		return(NULL);
	SDRL_VALUE(value)->refs = 1;
	SDRL_VALUE(value)->type = SDRL_VALUE(org)->type;
	SDRL_VALUE(value)->next = NULL;
	value->code = SDRL_EXPR(SDRL_MAKE_REFERENCE(org->code));
	value->env = SDRL_MAKE_REFERENCE(org->env);
	return(SDRL_VALUE(value));
}

int sdrl_base_evaluate_lexblock(struct sdrl_machine *mach, struct sdrl_lexblock *func, struct sdrl_value *args)
{
	struct sdrl_environment *env;

	if (!(env = sdrl_extend_environment(func->env)))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, SDRL_VALUE(func->code), env));
	SDRL_DESTROY_REFERENCE(env);
	return(0);
}


