/*
 * Name:	bindings.h
 * Description:	Bindings Manager Header
 */


#ifndef _SDRL_CORE_BINDINGS_H
#define _SDRL_CORE_BINDINGS_H

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_BBF_NO_ADD			0x0001			/** No bindings can be added */
#define SDRL_BBF_NO_REPLACE		0x0002			/** No bindings can be replaced */
#define SDRL_BBF_NO_REMOVE		0x0004			/** No bindings can be removed */
#define SDRL_BBF_CONSTANT		0x0006			/** No bindings can be replaced or removed */
#define SDRL_BBF_CASE_INSENSITIVE	0x0008			/** Ignore case when looking up bindings */

struct sdrl_binding {
	char *name;
	void *data;
	struct sdrl_binding *next;
};

struct sdrl_environment {
	struct sdrl_value value;
	int bitflags;
	struct sdrl_heap *heap;
	sdrl_destroy_t destroy;
	int size;
	int entries;
	struct sdrl_binding **table;
	struct sdrl_environment *parent;
};

#define SDRL_ENVIRONMENT(ptr)	( (struct sdrl_environment *) (ptr) )

struct sdrl_environment *sdrl_create_environment(struct sdrl_heap *, struct sdrl_type *, short, sdrl_destroy_t);
struct sdrl_environment *sdrl_extend_environment(struct sdrl_environment *);
struct sdrl_environment *sdrl_retract_environment(struct sdrl_environment *);
int sdrl_destroy_environment(struct sdrl_environment *);

int sdrl_add_binding(struct sdrl_environment *, const char *, void *);
int sdrl_replace_binding(struct sdrl_environment *, const char *, void *);
int sdrl_remove_binding(struct sdrl_environment *, const char *);

void *sdrl_find_binding(struct sdrl_environment *, const char *);

#endif

