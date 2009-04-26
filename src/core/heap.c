/*
 * Module Name:		heap.c
 * Description:		Heap Memory Manager
 */

#include <stdio.h>

#include <stdlib.h>
#include <sdrl/core/heap.h>

typedef struct sdHeapEntry sdHeapEntry;

struct sdHeapEntry {
	sdHeapEntry *next;
	sdHeapEntry *prev;
};

struct sdHeap {
	sdHeapEntry node;		/** This is an actual node */
};

static void sdrl_heap_report(sdHeap *heap);

/**
 * Create a new heap for objects of the given size using the given page size.
 */
sdHeap *sdrl_make_heap(void)
{
	sdHeap *heap;

	if (!(heap = (sdHeap *) malloc(sizeof(sdHeap))))
		return(NULL);
	heap->node.prev = NULL;
	heap->node.next = NULL;
	return(heap);
}

/**
 * Free all the memory allocated by the heap
 */
int sdrl_heap_destroy(sdHeap *heap)
{
	sdHeapEntry *cur, *next;

	if (!heap)
		return(-1);
	//sdrl_heap_report(heap);

	cur = heap->node.next;
	while (cur) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	free(heap);
	return(0);
}

/**
 * Allocate a new object from the heap of the given size and return a pointer
 * to it or NULL if out of memory.
 */
void *sdrl_heap_alloc(sdHeap *heap, unsigned int size)
{
	sdHeapEntry *entry;

	if (!(entry = (sdHeapEntry *) malloc(sizeof(sdHeapEntry) + size)))
		return(NULL);
	/** The heap itself is a node in the list which allows us to free a node
	    without a pointer to the heap itself. */
	entry->prev = &heap->node;
	if (heap->node.next)
		heap->node.next->prev = entry;
	entry->next = heap->node.next;
	heap->node.next = entry;
	return((void *) (entry + 1));
}

/**
 * Free a memory address allocated from heap and return 0 if successful or
 * -1 if an error occurs.
 */
int sdrl_heap_free(void *addr)
{
	sdHeapEntry *entry;

	entry = (sdHeapEntry *) (((sdHeapEntry *) addr) - 1);
	/** Since the heap struct itself is a node and we will obviously never
	    remove that node from the list, we don't need a pointer to the heap
	    in order to remove a node.  Therefore, if we encounter a NULL prev
	    here, it must be a corrupt or invalid node */
	if (!entry->prev)
		return(-1);
	entry->prev->next = entry->next;
	if (entry->next)
		entry->next->prev = entry->prev;
	free(entry);
	return(0);
}

/*** Local Functions ***/

static void sdrl_heap_report(sdHeap *heap)
{
	sdHeapEntry *cur;

	cur = heap->node.next;
	while (cur) {
		printf("Unfreed: 0x%x\n", (unsigned int) (cur + 1));
		cur = cur->next;
	}
}


