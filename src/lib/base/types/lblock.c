/*
 * Type Name:		lblock.c
 * Module Requirements:	(none)
 * Description:		Code Block Type using Lexical Scoping
 */

#include <stdlib.h>

#include <sdrl/sdrl.h>

struct sdrl_lblock {
	struct sdrl_expr *code;
	struct sdrl_environment *env;
};

void *sdrl_create_lblock_type(struct sdrl_machine *, struct sdrl_value *);
static int sdrl_destroy_lblock_type(struct sdrl_heap *, void *);
static void *sdrl_duplicate_lblock_type(struct sdrl_heap *, void *);
static int sdrl_evaluate_lblock_type(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

struct sdrl_type *sdrl_base_make_lblock_type(struct sdrl_machine *mach)
{
	return(sdrl_make_type(
		mach->heap,
		0,
		SDRL_BT_POINTER,
		(sdrl_create_t) sdrl_create_lblock_type,
		(sdrl_evaluate_t) sdrl_evaluate_lblock_type,
		(sdrl_duplicate_t) sdrl_duplicate_lblock_type,
		(sdrl_destroy_t) sdrl_destroy_lblock_type
	));
}

/*** Local Functions ***/

/**
 * Create a new lblock given the AST code to store.
 */
void *sdrl_create_lblock_type(struct sdrl_machine *mach, struct sdrl_value *expr)
{
	struct sdrl_lblock *block;

	// TODO should you check the expr type first?
	if (!(block = (struct sdrl_lblock *) malloc(sizeof(struct sdrl_lblock))))
		return(NULL);
	block->code = expr->data.ptr;
	block->env = SDRL_MAKE_REFERENCE(mach->env);
	return((void *) block);
}

/**
 * Destroy the given lblock.
 */
static int sdrl_destroy_lblock_type(struct sdrl_heap *heap, void *ptr)
{
	struct sdrl_lblock *block;

	if (!(block = ptr))
		return(-1);
	SDRL_DESTROY_REFERENCE(block->env, sdrl_retract_environment);
	free(block);
	return(0);
}

/**
 * Duplicate the given lblock.
 */
static void *sdrl_duplicate_lblock_type(struct sdrl_heap *heap, void *ptr)
{
	struct sdrl_lblock *block, *newblock;

	if (!(block = ptr))
		return(NULL);
	if (!(newblock = (struct sdrl_lblock *) malloc(sizeof(struct sdrl_lblock))))
		return(NULL);
	newblock->code = block->code;
	newblock->env = SDRL_MAKE_REFERENCE(block->env);
	return((void *) newblock);
}

/**
 * Extend the stored environment (block->env), set the '_' binding in the new
 * environment to a reference to the given args, and push an event to evaluate
 * the given AST in the new environment.  If an error occurs then an error
 * code is returned; otherwise 0 is returned.
 */
static int sdrl_evaluate_lblock_type(struct sdrl_machine *mach, struct sdrl_value *value, struct sdrl_value *args)
{
	struct sdrl_lblock *block;
	struct sdrl_environment *env;

	block = (struct sdrl_lblock *) value->data.ptr;
	if (!(env = sdrl_extend_environment(block->env)))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_OUT_OF_MEMORY, NULL));
	sdrl_add_binding(env, "_", SDRL_MAKE_REFERENCE(args));
	sdrl_push_event(mach->cont, sdrl_make_event(0, (sdrl_event_t) sdrl_evaluate_expr_list, block->code, env));
	SDRL_DESTROY_REFERENCE(env, sdrl_retract_environment);
	return(0);
}


