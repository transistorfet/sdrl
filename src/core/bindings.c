/*
 * Name:	bindings.c
 * Description:	Bindings Manager
 */


#include <stdlib.h>
#include <string.h>

#include <sdrl/core/bindings.h>
#include <sdrl/core/heap.h>
#include <sdrl/globals.h>

#define BINDINGS_DESTROY_DATA(env, data) {				\
	if ((env)->destroy) {						\
		if ((env)->heap)					\
			(env)->destroy((env)->heap, (data));		\
		else							\
			((int (*)(void *)) (env)->destroy)((data));	\
	}								\
}

#define BINDINGS_COMPARE(env, str1, str2) \
	( ((env)->bitflags & SDRL_BBF_CASE_INSENSITIVE) ? !sdrl_stricmp((str1), (str2)) : !strcmp((str1), (str2)) )

#define IS_LOWERCASE(ch) \
	( (ch >= 0x41 && ch <= 0x5a) ? ch + 0x20 : ch )

static struct sdrl_binding *sdrl_get_bindings(struct sdrl_environment *, char *, int);
static int sdrl_stricmp(char *, char *);

/**
 * Allocate an environment for binding values to names.
 */
struct sdrl_environment *sdrl_create_environment(short bitflags, struct sdrl_heap *heap, sdrl_destroy_t destroy)
{
	struct sdrl_environment *env;

	if (!(env = (struct sdrl_environment *) sdrl_heap_alloc(heap, sizeof(struct sdrl_environment))))
		return(NULL);
	env->bitflags = bitflags;
	env->refs = 1;
	env->heap = heap;
	env->destroy = destroy;
	env->head = NULL;
	env->tail = NULL;
	env->parent = NULL;
	return(env);
}

/**
 * Allocate an environment for binding values to names.
 */
struct sdrl_environment *sdrl_extend_environment(struct sdrl_environment *parent)
{
	struct sdrl_environment *env;

	if (!(env = (struct sdrl_environment *) sdrl_heap_alloc(parent->heap, sizeof(struct sdrl_environment))))
		return(NULL);
	env->bitflags = parent->bitflags;
	env->refs = 1;
	env->heap = parent->heap;
	env->destroy = parent->destroy;
	env->head = NULL;
	env->tail = NULL;
	env->parent = SDRL_MAKE_REFERENCE(parent);
	return(env);
}

/**
 * Free resources allocated by the top most environment including all bindings and
 * return a pointer to the parent environment or NULL if env does not have a parent.
 */
struct sdrl_environment *sdrl_retract_environment(struct sdrl_environment *env)
{
	struct sdrl_binding *cur, *next;
	struct sdrl_environment *parent;

	if (!env)
		return(NULL);

	parent = env->parent;
	if (--env->refs)
		return(parent);

	cur = env->head;
	while (cur) {
		next = cur->next;
		BINDINGS_DESTROY_DATA(env, cur->data);
		free(cur);
		cur = next;
	}
	if (env->parent)
		parent = SDRL_DESTROY_REFERENCE(env->parent, sdrl_retract_environment) ? NULL : env->parent;
	sdrl_heap_free(env->heap, env);
	return(parent);
}

/**
 * Free resources allocated by the environment and all of its parents.
 */
int sdrl_destroy_environment(struct sdrl_environment *env)
{
	while (env)
		env = sdrl_retract_environment(env);
	return(0);
}


/**
 * Add a binding of name to environment.
 */
int sdrl_add_binding(struct sdrl_environment *env, char *name, void *data)
{
	struct sdrl_binding *bind;

	if (!name || !data || (env->bitflags & SDRL_BBF_NO_ADD))
		return(-1);
	if (sdrl_get_bindings(env, name, 1))
		return(SDRL_ERR_IN_USE);
	if (!(bind = (struct sdrl_binding *) malloc(sizeof(struct sdrl_binding) + strlen(name) + 1)))
		return(SDRL_ERR_OUT_OF_MEMORY);

	bind->name = (char *) ((size_t) bind + sizeof(struct sdrl_binding));
	strcpy(bind->name, name);
	bind->data = data;
	bind->next = NULL;
	if (env->tail)
		env->tail->next = bind;
	else
		env->head = bind;
	env->tail = bind;

	return(0);
}

/**
 * Replace the binding's data with data.
 */
int sdrl_replace_binding(struct sdrl_environment *env, char *name, void *data)
{
	struct sdrl_binding *bind;

	if (!name || !data || (env->bitflags & SDRL_BBF_NO_REPLACE))
		return(-1);
	if ((bind = sdrl_get_bindings(env, name, 1))) {
		BINDINGS_DESTROY_DATA(env, bind->data);
		bind->data = data;
		return(0);
	}
	return(SDRL_ERR_NOT_FOUND);
}

/**
 * Remove a bindings of name from environment
 */
int sdrl_remove_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *cur, *prev;

	if (!name || (env->bitflags & SDRL_BBF_NO_REMOVE))
		return(-1);
	prev = NULL;
	cur = env->head;
	while (cur) {
		if (BINDINGS_COMPARE(env, name, cur->name)) {
			if (prev)
				prev->next = cur->next;
			else
				env->head = cur->next;
			if (cur == env->tail)
				env->tail = prev;
			BINDINGS_DESTROY_DATA(env, cur->data);
			free(cur);
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
void *sdrl_find_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if ((bind = sdrl_get_bindings(env, name, 0)))
		return(bind->data);
	return(NULL);
}

/*** Local Functions ***/

/**
 * Returns binding of name in environment or one of environments parents
 * searching up to levels (0 to search all).
 */
static struct sdrl_binding *sdrl_get_bindings(struct sdrl_environment *env, char *name, int levels)
{
	struct sdrl_binding *cur;
	struct sdrl_environment *curenv;

	curenv = env;
	while (curenv) {
		cur = curenv->head;
		while (cur) {
			if (BINDINGS_COMPARE(curenv, name, cur->name))
				return(cur);
			cur = cur->next;
		}
		if (--levels == 0)
			return(NULL);
		curenv = curenv->parent;
	}
	return(NULL);
}

static int sdrl_stricmp(char *str1, char *str2)
{
	int i = 0;

	while ((str1[i] != '\0') && (str2[i] != '\0')) {
		if (IS_LOWERCASE(str1[i]) != IS_LOWERCASE(str2[i]))
			return(1);
		i++;
	}
	return(0);
}



