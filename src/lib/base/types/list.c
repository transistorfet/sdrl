/*
 * Type Name:		list.c
 * Module Requirements:	(none)
 * Description:		List Type
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

sdType sdListTypeDef = {
	&sdValueTypeDef,
	"list",
	sizeof(sdList),
	0,
	(sdrl_create_t) sdrl_base_list_create,
	(sdrl_destroy_t) sdrl_base_list_destroy,
	(sdrl_duplicate_t) sdrl_base_list_duplicate,
	NULL
};


sdList *sdrl_base_list_create(sdMachine *mach, sdType *type, sdArray *args)
{
	int i;
	sdList *list = NULL, *node;

	for (i = args->last; i >= 0; i--) {
		if (!(node = (sdList *) sdrl_heap_alloc(mach->heap, type->size)))
			goto FAIL;
		SDVALUE(node)->refs = 1;
		SDVALUE(node)->type = type;
		node->item = SDRL_INCREF(args->items[i]);
		node->next = list;
		list = node;
	}
	return(list);

    FAIL:
	sdrl_base_list_destroy(list);
	sdrl_set_memory_error(mach);
	return(NULL);
}

void sdrl_base_list_destroy(sdList *list)
{
	sdList *cur, *tmp;

	for (cur = list; cur; cur = tmp) {
		if (--SDVALUE(cur)->refs > 0)
			return;
		SDRL_DECREF(cur->item);
		tmp = cur->next;
		sdrl_heap_free(cur);
	}
}

sdList *sdrl_base_list_duplicate(sdMachine *mach, sdList *org)
{
	sdList *list, *node, *cur, *prev = NULL;

	for (cur = org; cur; cur = cur->next) {
		if (!(node = (sdList *) sdrl_heap_alloc(mach->heap, SDVALUE(org)->type->size)))
			goto FAIL;
		SDVALUE(node)->refs = 1;
		SDVALUE(node)->type = SDVALUE(cur)->type;
		node->item = SDRL_INCREF(cur->item);
		node->next = NULL;
		if (prev)
			prev->next = node;
		else
			list = node;
		prev = node;
	}
	return(list);

    FAIL:
	sdrl_base_list_destroy(list);
	sdrl_set_memory_error(mach);
	return(NULL);
}


