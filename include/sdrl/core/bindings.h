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

typedef struct sdBinding sdBinding;

struct sdBinding {
	char *name;
	void *data;
	sdBinding *next;
};

struct sdEnv {
	sdValue value;
	int bitflags;
	sdHeap *heap;
	sdrl_destroy_t destroy;
	int size;
	int entries;
	sdBinding **table;
	sdEnv *parent;
};

#define SDENV(ptr)		( (sdEnv *) (ptr) )

sdType *sdrl_make_environment_type(void);

sdEnv *sdrl_make_environment(sdHeap *, sdType *, short, sdrl_destroy_t);
sdEnv *sdrl_environment_create(sdMachine *, sdType *, sdArray *);
sdEnv *sdrl_extend_environment(sdEnv *);
sdEnv *sdrl_retract_environment(sdEnv *);
int sdrl_destroy_environment(sdEnv *);

int sdrl_add_binding(sdEnv *, const char *, void *);
int sdrl_replace_binding(sdEnv *, const char *, void *);
int sdrl_remove_binding(sdEnv *, const char *);

void *sdrl_find_binding(sdEnv *, const char *);

#endif

