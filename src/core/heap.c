/*
 * Module Name:		heap.c
 * Module Version:	0.1
 * Description:		Heap Memory Manager
 */

#include <stdlib.h>
#include <sdrl/core/heap.h>


#define MAX_LIST	1000

struct entry {
	void *addr;
	int size;
	struct entry *next;
};

int next = 0;
struct entry *head = NULL;
struct entry list[MAX_LIST];

void heap_report(void);


//static struct sdrl_heap *heap_create_new_page(unsigned int, unsigned int);

/**
 * Create a new heap for objects of the given size using the given page size.
 */
struct sdrl_heap *sdrl_create_heap(unsigned int size, unsigned int page_size)
{
//	return(heap_create_new_page(size, page_size ? page_size : SDRL_HEAP_PAGE_SIZE));
	return((struct sdrl_heap *) 1);
}

/**
 * Free all the memory allocated by the heap
 */
int sdrl_destroy_heap(struct sdrl_heap *heap)
{
	struct sdrl_heap *tmp;

	if (!(heap))
		return(-1);
	heap_report();
/*
	while (heap) {
		tmp = heap->next;
		free(heap);
		heap = tmp;
	}
*/
	return(0);
}

/**
 * Allocate a new heap object from the heap.
 */
void *sdrl_heap_alloc(struct sdrl_heap *heap, unsigned int size)
{
	//printf("Allocate %d bytes\n", size);

	void *addr;

	addr = malloc(size);

	if (next >= MAX_LIST)
		return(NULL);
	list[next].addr = addr;
	list[next].size = size;
	list[next].next = head;
	head = &list[next++];

	return(addr);
//	return(malloc(size));
/*
	int i, j;
	unsigned char bits;
	struct sdrl_heap *next;

	if (!(heap))
		return(NULL);

	next = heap;
	while (next && (next->objs >= heap->allocs_size * 8))
		next = next->next;
	if (!(next)) {
		if (!(next = heap_create_new_page(heap->obj_size, heap->page_size)))
			return(NULL);
		next->next = heap->next;
		heap->next = next;
	}

	for (i = 0;i < next->allocs_size;i++) {
		if (~next->allocs[i]) {
			bits = 1;
			for (j = 0;j < 8;j++) {
				if (!(bits & next->allocs[i])) {
					next->allocs[i] |= bits;
					next->objs++;
					return((void *) (((unsigned int) next->page) + ((i * 8 + j) * next->obj_size)));
				}
				bits <<= 1;
			}
		}
	}

	return(NULL);
*/
}

/**
 * Free a memory address allocated from heap.
 */

int sdrl_heap_free(struct sdrl_heap *heap, void *addr)
{
	//printf("Free\n");

	struct entry *cur, *prev;
	prev = NULL;
	cur = head;
	while (cur) {
		if (cur->addr == addr) {
			if (prev)
				prev->next = cur->next;
			else
				head = cur->next;
			free(addr);
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}

	return(-1);

/*
	int object;
	struct sdrl_heap *next;

	if (!(heap))
		return(-1);

	next = heap;
	while (next && ((addr < next->page) || (addr > (void *) (((unsigned int) next->page) + (next->page_size - next->allocs_size)))))
		next = next->next;
	if (!(next))
		return(-1);

	object = (addr - next->page) / next->obj_size;
	next->allocs[object / 8] &= ~(0x01 << object - ((object / 8) * 8));
	next->objs--;

	if (next->objs == 0) {
		if (next != heap) {
			while (heap->next != next)
				heap = heap->next;
			heap->next = next->next;
			free(next);
		} 
	}

	return(0);
*/
}

void heap_report(void)
{
	struct entry *cur;
	cur = head;
	while (cur) {
		printf("Memory Leak: %d of size %d\n", cur->addr, cur->size);
		cur = cur->next;
	}
}



/*** Local Functions ***/

/**
 * Allocate and initialize a new heap page.
 */
/*
static struct sdrl_heap *heap_create_new_page(unsigned int size, unsigned int page_size)
{
	struct sdrl_heap *heap;

	if (!(heap = (struct sdrl_heap *) malloc(page_size)))
		return(NULL);

	heap->obj_size = size;
	heap->page_size = page_size;
	heap->objs = 0;
	heap->allocs = (unsigned char *) (((unsigned int) heap) + sizeof(struct sdrl_heap));
	heap->allocs_size = (page_size - sizeof(struct sdrl_heap)) / (size * 8 + 1);
	heap->page = (void *) (((unsigned int) heap->allocs) + heap->allocs_size);
	heap->next = NULL;

	return(heap);
}
*/

