/* 
 * Include Name:	heap.h
 * Include Version:	0.1
 * Description:		Heap Manager Interface
 */

#ifndef _SDRL_CORE_HEAP_H
#define _SDRL_CORE_HEAP_H

#define SDRL_HEAP_PAGE_SIZE		0x400

struct sdrl_heap {
	unsigned int obj_size;
	unsigned int page_size;
	int objs;
	unsigned char *allocs;
	int allocs_size;
	void *page;
	struct sdrl_heap *next;
};

struct sdrl_heap *sdrl_create_heap(unsigned int, unsigned int);
int sdrl_destroy_heap(struct sdrl_heap *);
void *sdrl_heap_alloc(struct sdrl_heap *, unsigned int);
int sdrl_heap_free(struct sdrl_heap *, void *);

#endif

