/*
 * Name:	type.c
 * Version:	0.1
 * Description:	Type Manager
 */

#include <string.h>

#include "type.h"
#include "types.h"


/**
 * Make a new generic sdrl type.
 */
struct sdrl_type *sdrl_make_type(int size, int bitflags, sdrl_evaluate_t evaluate, sdrl_duplicate_t duplicate, sdrl_destroy_t destroy)
{
	struct sdrl_type *type;

	if (!(type = (struct sdrl_type *) malloc(sizeof(struct sdrl_type))))
		return(NULL);

	type->size = size;
	type->bitflags = bitflags;
	type->evaluate = evaluate;
	type->duplicate = duplicate;
	type->destroy = destroy;

	return(type);
}

/**
 * Frees resources used by type.
 */
int sdrl_destroy_type(struct sdrl_type *type)
{
	free(type);
	return(0);
}


