/* 
 * Name:	heap.h
 * Description:	Heap Manager Interface
 */

#ifndef _SDRL_CORE_HEAP_H
#define _SDRL_CORE_HEAP_H

#include <stddef.h>
#include <sdrl/globals.h>

sdHeap *sdrl_create_heap(void);
int sdrl_destroy_heap(sdHeap *);
void *sdrl_heap_alloc(sdHeap *, unsigned int);
int sdrl_heap_free(void *);

#endif

