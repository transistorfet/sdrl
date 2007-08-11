/*
 * Name:	type.c
 * Description:	Type Manager
 */

#include <string.h>

#include <sdrl/core/type.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>


/**
 * Make a new generic sdrl type.
 */
struct sdrl_type *sdrl_make_type(struct sdrl_heap *heap, int size, int bitflags, sdrl_create_t create, sdrl_evaluate_t evaluate, sdrl_duplicate_t duplicate, sdrl_destroy_t destroy)
{
	struct sdrl_type *type;

	if (!(type = (struct sdrl_type *) sdrl_heap_alloc(heap, sizeof(struct sdrl_type))))
		return(NULL);

	type->size = size;
	type->bitflags = bitflags;
	type->create = create;
	type->evaluate = evaluate;
	type->duplicate = duplicate;
	type->destroy = destroy;

	return(type);
}

/**
 * Frees resources used by type.
 */
int sdrl_destroy_type(struct sdrl_heap *heap, struct sdrl_type *type)
{
	sdrl_heap_free(heap, type);
	return(0);
}


