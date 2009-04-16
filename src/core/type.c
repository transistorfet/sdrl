/*
 * Name:	type.c
 * Description:	Type Manager
 */

#include <stdlib.h>

#include <sdrl/core/type.h>
#include <sdrl/globals.h>


/**
 * Make a new generic sdrl type.
 */
sdType *sdrl_make_type(int size, short bitflags, short basetype, sdrl_create_t create, sdrl_destroy_t destroy, sdrl_duplicate_t duplicate, sdrl_evaluate_t evaluate)
{
	sdType *type;

	if (!(type = (sdType *) malloc(sizeof(sdType))))
		return(NULL);
	type->size = size;
	type->bitflags = bitflags;
	type->basetype = basetype;
	type->create = create;
	type->destroy = destroy;
	type->duplicate = duplicate;
	type->evaluate = evaluate;
	return(type);
}

/**
 * Frees resources used by type.
 */
int sdrl_destroy_type(sdType *type)
{
	free(type);
	return(0);
}


