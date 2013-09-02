/* 
 * Name:	memory.h
 * Description:	Memory Functions
 */

#ifndef _SDRL_CORE_MEMORY_H
#define _SDRL_CORE_MEMORY_H

#include <stddef.h>
#include <sdrl/globals.h>

#ifndef sdrl_alloc
#define sdrl_alloc(mach, size)	\
	( sdrl_heap_alloc((mach)->heap, (size)) )
#endif

#ifndef sdrl_free
#define sdrl_free(mach, ptr)	\
	( sdrl_heap_free((ptr)) )
#endif

#endif

