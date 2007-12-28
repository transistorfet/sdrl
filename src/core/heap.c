/*
 * Module Name:		heap.c
 * Description:		Heap Memory Manager
 */

#include <stdio.h>

#include <stdlib.h>
#include <sdrl/core/heap.h>

struct sdrl_heap {
	struct sdrl_heap_entry *head;
};

struct sdrl_heap_entry {
	struct sdrl_heap_entry *next;
	struct sdrl_heap_entry *prev;
};

static void sdrl_heap_report(struct sdrl_heap *heap);

/**
 * Create a new heap for objects of the given size using the given page size.
 */
struct sdrl_heap *sdrl_create_heap(void)
{
	struct sdrl_heap *heap;

	if (!(heap = (struct sdrl_heap *) malloc(sizeof(struct sdrl_heap))))
		return(NULL);
	heap->head = NULL;
	return(heap);
}

/**
 * Free all the memory allocated by the heap
 */
int sdrl_destroy_heap(struct sdrl_heap *heap)
{
	struct sdrl_heap_entry *cur, *next;

	if (!heap)
		return(-1);
	sdrl_heap_report(heap);

	cur = heap->head;
	while (cur) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	free(heap);
	return(0);
}

/**
 * Allocate a new heap object from the heap.
 */
void *sdrl_heap_alloc(struct sdrl_heap *heap, unsigned int size)
{
	struct sdrl_heap_entry *entry;

	if (!(entry = (struct sdrl_heap_entry *) malloc(sizeof(struct sdrl_heap_entry) + size)))
		return(NULL);
	entry->prev = NULL;
	if (heap->head)
		heap->head->prev = entry;
	entry->next = heap->head;
	heap->head = entry;
	return((void *) (entry + 1));
}

/**
 * Free a memory address allocated from heap.
 */

int sdrl_heap_free(struct sdrl_heap *heap, void *addr)
{
	struct sdrl_heap_entry *entry;

	entry = (struct sdrl_heap_entry *) (((struct sdrl_heap_entry *) addr) - 1);
	if (entry->prev)
		entry->prev->next = entry->next;
	else
		heap->head = entry->next;
	if (entry->next)
		entry->next->prev = entry->prev;
	free(entry);
	return(0);
}

/*** Local Functions ***/

static void sdrl_heap_report(struct sdrl_heap *heap)
{
	struct sdrl_heap_entry *cur;

	cur = heap->head;
	while (cur) {
		printf("Unfreed: 0x%x\n", (unsigned int) (cur + 1));
		cur = cur->next;
	}
}


