/*
 * Name:	bindings.c
 * Version:	0.2
 * Description:	Bindings Manager
 */


#include <stdlib.h>
#include <string.h>

#include "bindings.h"
#include "globals.h"

static struct sdrl_binding *sdrl_get_bindings(struct sdrl_environment *, char *, int);

/**
 * Allocate an environment for binding values to names.
 */
struct sdrl_environment *sdrl_create_environment(int bitflags, int (*destroy)(void *))
{
	struct sdrl_environment *env;

	if (!(env = (struct sdrl_environment *) malloc(sizeof(struct sdrl_environment))))
		return(NULL);

	env->bitflags = bitflags;
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

	if (!(env = (struct sdrl_environment *) malloc(sizeof(struct sdrl_environment))))
		return(NULL);

	env->bitflags = parent->bitflags;
	env->destroy = parent->destroy;
	env->head = NULL;
	env->tail = NULL;
	env->parent = parent;
	return(env);
}

/**
 * Free resources allocated by the environment including all bindings.
 */
int sdrl_destroy_environment(struct sdrl_environment *env)
{
	struct sdrl_binding *cur, *next;

	if (!env)
		return(-1);

	cur = env->head;
	while (cur) {
		next = cur->next;
		env->destroy(cur->data);
		free(cur);
		cur = next;
	}
	free(env);
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
		return(ERR_IN_USE);
	if (!(bind = (struct sdrl_binding *) malloc(sizeof(struct sdrl_binding) + strlen(name) + 1)))
		return(ERR_OUT_OF_MEMORY);

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
	if (bind = sdrl_get_bindings(env, name, 1)) {
		env->destroy(bind->data);
		bind->data = data;
		return(0);
	}
	return(ERR_NOT_FOUND);
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
		if (!strcmp(name, cur->name)) {
			if (prev)
				prev->next = cur->next;
			else
				env->head = cur->next;
			if (cur == env->tail)
				env->tail = prev;
			env->destroy(cur->data);
			free(cur);
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}
	return(ERR_NOT_FOUND);
}

/**
 * Find the value bound to name in env or its parents.
 */
void *sdrl_find_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = sdrl_get_bindings(env, name, 0))
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
			if (!strcmp(name, cur->name))
				return(cur);
			cur = cur->next;
		}
		if (--levels == 0)
			return(NULL);
		curenv = curenv->parent;
	}
	return(NULL);
}


