/* 
 * Name:	heap.h
 * Description:	Heap Manager Interface
 */

#ifndef _SDRL_CORE_HEAP_H
#define _SDRL_CORE_HEAP_H

#include <stdlib.h>

struct sdrl_heap;

struct sdrl_heap *sdrl_create_heap(unsigned int, unsigned int);
int sdrl_destroy_heap(struct sdrl_heap *);
void *sdrl_heap_alloc(struct sdrl_heap *, unsigned int);
int sdrl_heap_free(struct sdrl_heap *, void *);

#endif

