/*
 * Name:	bindings.c
 * Description:	Bindings Manager
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/bindings.h>
#include <sdrl/core/heap.h>
#include <sdrl/core/value.h>
#include <sdrl/globals.h>

#define SDRL_ENV_INIT_SIZE		32
#define SDRL_ENV_LOAD_FACTOR		0.75
#define SDRL_ENV_GROWTH_FACTOR		1.75

#define BINDINGS_COMPARE(env, str1, str2) \
	( SDRL_BF_IS_SET((env), SDRL_BBF_CASE_INSENSITIVE) ? !sdrl_stricmp((str1), (str2)) : !strcmp((str1), (str2)) )

#define LOWERCASE(ch) \
	( (ch >= 0x41 && ch <= 0x5a) ? ch + 0x20 : ch )

static sdBinding *sdrl_get_bindings(sdEnv *, const char *, int);
static inline int sdrl_bindings_rehash(sdEnv *env, int newsize);
static int sdrl_stricmp(const char *, const char *);
static inline unsigned int sdrl_hash(const char *);

/**
 * Allocate an environment for binding values to names.
 */
sdEnv *sdrl_create_environment(sdHeap *heap, sdType *type, short bitflags, sdrl_destroy_t destroy)
{
	sdBinding **table;
	sdEnv *env;

	if (!(table = (sdBinding **) malloc(SDRL_ENV_INIT_SIZE * sizeof(sdBinding *))))
		return(NULL);
	if (!(env = (sdEnv *) sdrl_heap_alloc(heap, type->size))) {
		free(table);
		return(NULL);
	}
	SDVALUE(env)->refs = 1;
	SDVALUE(env)->type = type;
	SDVALUE(env)->next = NULL;
	env->bitflags = bitflags;
	env->heap = heap;
	env->destroy = destroy;
	env->size = SDRL_ENV_INIT_SIZE;
	env->entries = 0;
	env->table = table;
	memset(env->table, '\0', SDRL_ENV_INIT_SIZE * sizeof(sdBinding *));
	env->parent = NULL;
	return(env);
}

/**
 * Allocate an environment for binding values to names.
 */
sdEnv *sdrl_extend_environment(sdEnv *parent)
{
	sdEnv *env;

	if (!(env = sdrl_create_environment(parent->heap, SDVALUE(parent)->type, parent->bitflags, parent->destroy)))
		return(NULL);
	env->parent = SDRL_MAKE_REFERENCE(parent);
	return(env);
}

/**
 * Free resources allocated by the top most environment including all bindings and
 * return a pointer to the parent environment or NULL if env does not have a parent.
 */
sdEnv *sdrl_retract_environment(sdEnv *env)
{
	unsigned int i;
	sdBinding *cur, *next;
	sdEnv *parent;

	if (!env)
		return(NULL);

	parent = env->parent;
	if (--SDVALUE(env)->refs)
		return(parent);

	for (i = 0;i < env->size;i++) {
		cur = env->table[i];
		while (cur) {
			next = cur->next;
			if (env->destroy)
				env->destroy(cur->data);
			free(cur);
			cur = next;
		}
	}
	free(env->table);
	if (env->parent)
		parent = SDRL_DESTROY_REFERENCE(env->parent) ? NULL : env->parent;
	sdrl_heap_free(env);
	return(parent);
}

/**
 * Free resources allocated by the environment and all of its parents.
 */
int sdrl_destroy_environment(sdEnv *env)
{
	while (env)
		env = sdrl_retract_environment(env);
	return(0);
}


/**
 * Add a binding of name to environment.
 */
int sdrl_add_binding(sdEnv *env, const char *name, void *data)
{
	unsigned int hash;
	sdBinding *bind;

	if (!name || !data || SDRL_BF_IS_SET(env, SDRL_BBF_NO_ADD))
		return(-1);
	/** Search for an existing entry */
	hash = sdrl_hash(name) % env->size;
	for (bind = env->table[hash]; bind; bind = bind->next) {
		if (BINDINGS_COMPARE(env, name, bind->name))
			return(SDRL_ERR_IN_USE);
	}

	if (!(bind = (sdBinding *) malloc(sizeof(sdBinding) + strlen(name) + 1)))
		return(SDRL_ERR_OUT_OF_MEMORY);
	bind->name = (char *) (bind + 1);
	strcpy(bind->name, name);
	bind->data = data;

	bind->next = env->table[hash];
	env->table[hash] = bind;
	env->entries++;
	if ((env->entries / env->size) > SDRL_ENV_LOAD_FACTOR)
		sdrl_bindings_rehash(env, env->size * SDRL_ENV_GROWTH_FACTOR);
	return(0);
}

/**
 * Replace the binding's data with data.
 */
int sdrl_replace_binding(sdEnv *env, const char *name, void *data)
{
	sdBinding *bind;

	if (!name || !data || SDRL_BF_IS_SET(env, SDRL_BBF_NO_REPLACE))
		return(-1);
	if ((bind = sdrl_get_bindings(env, name, 1))) {
		if (env->destroy)
			env->destroy(bind->data);
		bind->data = data;
		return(0);
	}
	return(SDRL_ERR_NOT_FOUND);
}

/**
 * Remove a bindings of name from environment
 */
int sdrl_remove_binding(sdEnv *env, const char *name)
{
	unsigned int hash;
	sdBinding *cur, *prev;

	if (!name || SDRL_BF_IS_SET(env, SDRL_BBF_NO_REMOVE))
		return(-1);
	hash = sdrl_hash(name) % env->size;
	prev = NULL;
	cur = env->table[hash];
	while (cur) {
		if (BINDINGS_COMPARE(env, name, cur->name)) {
			if (prev)
				prev = cur->next;
			else
				env->table[hash] = cur->next;
			if (env->destroy)
				env->destroy(cur->data);
			free(cur);
			env->entries--;
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}
	return(SDRL_ERR_NOT_FOUND);
}

/**
 * Find the value bound to name in env or its parents.
 */
void *sdrl_find_binding(sdEnv *env, const char *name)
{
	sdBinding *bind;

	if ((bind = sdrl_get_bindings(env, name, 0)))
		return(bind->data);
	return(NULL);
}

/*** Local Functions ***/

/**
 * Returns binding of name in environment or one of environments parents
 * searching up to levels (0 to search all).
 */
static sdBinding *sdrl_get_bindings(sdEnv *env, const char *name, int levels)
{
	unsigned int hash;
	sdBinding *cur;
	sdEnv *curenv;

	hash = sdrl_hash(name);
	curenv = env;
	for (curenv = env; curenv; curenv = curenv->parent) {
		for (cur = curenv->table[hash % curenv->size]; cur; cur = cur->next) {
			if (BINDINGS_COMPARE(curenv, name, cur->name))
				return(cur);
		}
		if (--levels == 0)
			return(NULL);
	}
	return(NULL);
}

/**
 * Increases the size of the hashtable and reinserts all of the elements.
 */
static inline int sdrl_bindings_rehash(sdEnv *env, int newsize)
{
	unsigned int i, hash, oldsize;
	sdBinding **newtable;
	sdBinding *cur, *next;

	if (!(newtable = (sdBinding **) malloc(newsize * sizeof(sdBinding *))))
		return(-1);
	memset(newtable, '\0', newsize * sizeof(sdBinding *));
	oldsize = env->size;
	env->size = newsize;
	for (i = 0;i < oldsize;i++) {
		cur = env->table[i];
		while (cur) {
			next = cur->next;
			hash = sdrl_hash(cur->name) % env->size;
			cur->next = newtable[hash];
			newtable[hash] = cur;
			cur = next;
		}
	}
	free(env->table);
	env->table = newtable;
	return(0);
}

static int sdrl_stricmp(const char *str1, const char *str2)
{
	int i = 0;

	while ((str1[i] != '\0') && (str2[i] != '\0')) {
		if (LOWERCASE(str1[i]) != LOWERCASE(str2[i]))
			return(1);
		i++;
	}
	return(0);
}

static inline unsigned int sdrl_hash(const char *str)
{
	int i;
	unsigned int hash = 0;

	for (i = 0;str[i] != '\0';i++)
		hash = LOWERCASE(str[i]) + (hash << 6) + (hash << 16) - hash;
	return(hash);
}

