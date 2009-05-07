/*
 * Name:	env.h
 * Description:	Environment Type Header
 */


#ifndef _SDRL_CORE_ENV_H
#define _SDRL_CORE_ENV_H

#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_BBF_NO_ADD			0x0001			/** No bindings can be added */
#define SDRL_BBF_NO_REPLACE		0x0002			/** No bindings can be replaced */
#define SDRL_BBF_NO_REMOVE		0x0004			/** No bindings can be removed */
#define SDRL_BBF_CONSTANT		0x0006			/** No bindings can be replaced or removed */
#define SDRL_BBF_CASE_INSENSITIVE	0x0008			/** Ignore case when looking up bindings */

#define SDENV(ptr)		( (sdEnv *) (ptr) )

extern sdType sdEnvTypeDef;

typedef struct sdBinding sdBinding;

struct sdBinding {
	char *name;
	void *data;
	sdBinding *next;
};

struct sdEnv {
	sdValue value;
	int bitflags;
	sdMachine *mach;
	sdrl_destroy_t destroy;
	int size;
	int entries;
	sdBinding **table;
	sdEnv *parent;
};

sdEnv *sdrl_make_env(sdMachine *, sdType *, short, sdrl_destroy_t);
sdEnv *sdrl_env_create(sdMachine *, sdType *, sdArray *);
sdEnv *sdrl_env_extend(sdEnv *, sdType *);
sdEnv *sdrl_env_retract(sdEnv *);
void sdrl_env_destroy(sdEnv *);

int sdrl_env_add(sdEnv *, const char *, void *);
int sdrl_env_replace(sdEnv *, const char *, void *);
int sdrl_env_remove(sdEnv *, const char *);

void *sdrl_env_find(sdEnv *, const char *);

#endif

