/*
 * Name:	bindings.c
 * Version:	0.1
 * Description:	Bindings Manager
 */


#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "bindings.h"
#include "types.h"

static struct sdrl_binding *bindings_find(struct sdrl_environment *, char *, int);

/**
 * Allocate an environment for binding values to names.
 */
struct sdrl_environment *sdrl_create_environment(u_char bitflags, struct sdrl_environment *parent)
{
	struct sdrl_environment *env;

	if (!(env = (struct sdrl_environment *) malloc(sizeof(struct sdrl_environment))))
		return(NULL);

	env->bitflags = bitflags;
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

	cur = env->head;
	while (cur) {
		next = cur->next;
		sdrl_destroy_value(cur->value);
		free(cur);
		cur = next;
	}
	free(env);
	return(0);
}

/**
 * Add a binding of name to value to environment.
 */
int sdrl_add_binding(struct sdrl_environment *env, char *name, struct sdrl_value *value)
{
	struct sdrl_binding *bind;

	if (!name || !value)
		return(-1);

	if (sdrl_find_local_binding(env, name))
		return(ERR_IN_USE);

	if (!(bind = (struct sdrl_binding *) malloc(sizeof(struct sdrl_binding) + strlen(name) + 1)))
		return(ERR_OUT_OF_MEMORY);

	bind->name = (char *) ((size_t) bind + sizeof(struct sdrl_binding));
	strcpy(bind->name, name);
	bind->value = value;

	bind->next = NULL;
	if (env->tail)
		env->tail->next = bind;
	else
		env->head = bind;
	env->tail = bind;

	return(0);
}

/**
 * Remove the binding, name.
 */
int sdrl_remove_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *cur, *prev;

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
			sdrl_destroy_value(cur->value);
			free(cur);
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}
	return(ERR_NOT_FOUND);
}

/**
 * Replace the binding's value with value or create the binding if it doesn't exist.
 */
int sdrl_replace_binding(struct sdrl_environment *env, char *name, struct sdrl_value *value)
{
	struct sdrl_binding *bind;

	if (!value)
		return(-1);
	if (bind = bindings_find(env, name, 0)) {
		sdrl_destroy_value(bind->value);
		bind->value = value;
		return(0);
	}
	return(sdrl_add_binding(env, name, value));
}

/**
 * Find the value bound to name in env or its parents.
 */
struct sdrl_value *sdrl_find_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = bindings_find(env, name, 0))
		return(bind->value);
	return(NULL);
}

/**
 * Find the value bound to name in env only.
 */
struct sdrl_value *sdrl_find_local_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = bindings_find(env, name, 1))
		return(bind->value);
	return(NULL);
}

/*** Local Functions ***/

/**
 * Returns binding of name in environment or one of environments parents searching up to levels (0 to search all)
 */
static struct sdrl_binding *bindings_find(struct sdrl_environment *env, char *name, int levels)
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
		do {
			curenv = curenv->parent;
		} while ((curenv) && (curenv->bitflags & SDRL_EBF_PRIVATE));
	}
	return(NULL);
}


