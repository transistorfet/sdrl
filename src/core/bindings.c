/*
 * Name:	bindings.c
 * Version:	0.2
 * Description:	Bindings Manager
 */


#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "value.h"
#include "bindings.h"
#include "globals.h"

static int bindings_add(struct sdrl_environment *, char *, int, sdrl_bind_t);
static struct sdrl_binding *bindings_find(struct sdrl_environment *, char *, int, int);

/**
 * Allocate an environment for binding values to names.
 */
struct sdrl_environment *sdrl_create_environment(int bitflags, struct sdrl_environment *parent)
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
		if (cur->type == SDRL_BT_TYPE)
			sdrl_destroy_type(cur->data.type);
		else
			sdrl_destroy_value(cur->data.value);
		free(cur);
		cur = next;
	}
	free(env);
	return(0);
}

/**
 * Add a binding of name to type to environment.
 */
int sdrl_bind_type(struct sdrl_environment *env, char *name, struct sdrl_type *type)
{
	return(bindings_add(env, name, SDRL_BT_TYPE, (sdrl_bind_t) type));
}

/**
 * Add a binding of name to value to environment.
 */
int sdrl_bind_value(struct sdrl_environment *env, char *name, struct sdrl_value *value)
{
	return(bindings_add(env, name, SDRL_BT_VALUE, (sdrl_bind_t) value));
}

/**
 * Replace the binding's value with value or create the binding if it doesn't exist.
 * (Types cannot be replaced).
 */
int sdrl_rebind_value(struct sdrl_environment *env, char *name, struct sdrl_value *value)
{
	struct sdrl_binding *bind;

	if (!name || !value)
		return(-1);
	if (bind = bindings_find(env, name, SDRL_BT_VALUE, 1)) {
		sdrl_destroy_value(bind->data.value);
		bind->data.value = value;
		return(0);
	}
	return(ERR_NOT_FOUND);
}

/**
 * Remove the value binding, name.  (Types cannot be removed).
 */
int sdrl_remove_binding(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *cur, *prev;

	prev = NULL;
	cur = env->head;
	while (cur) {
		if ((cur->type == SDRL_BT_VALUE) && !strcmp(name, cur->name)) {
			if (prev)
				prev->next = cur->next;
			else
				env->head = cur->next;
			if (cur == env->tail)
				env->tail = prev;
			sdrl_destroy_value(cur->data.value);
			free(cur);
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}
	return(ERR_NOT_FOUND);
}

/**
 * Find the type bound to name in env or its parents.
 */
struct sdrl_type *sdrl_find_type(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = bindings_find(env, name, SDRL_BT_TYPE, 0))
		return(bind->data.type);
	return(NULL);
}


/**
 * Find the value bound to name in env or its parents.
 */
struct sdrl_value *sdrl_find_value(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = bindings_find(env, name, SDRL_BT_VALUE, 0))
		return(bind->data.value);
	return(NULL);
}

/**
 * Find the value bound to name in env only.
 */
struct sdrl_value *sdrl_find_local_value(struct sdrl_environment *env, char *name)
{
	struct sdrl_binding *bind;

	if (bind = bindings_find(env, name, SDRL_BT_VALUE, 1))
		return(bind->data.value);
	return(NULL);
}

/*** Local Functions ***/

/**
 * Add a binding of name to a type or value to environment.
 */
static int bindings_add(struct sdrl_environment *env, char *name, int type, sdrl_bind_t data)
{
	struct sdrl_binding *bind;

	if (!name || !data.value)
		return(-1);
	if (bindings_find(env, name, type, 1))
		return(ERR_IN_USE);
	if (!(bind = (struct sdrl_binding *) malloc(sizeof(struct sdrl_binding) + strlen(name) + 1)))
		return(ERR_OUT_OF_MEMORY);

	bind->name = (char *) ((size_t) bind + sizeof(struct sdrl_binding));
	strcpy(bind->name, name);
	bind->type = type;
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
 * Returns binding of name in environment or one of environments parents
 * searching up to levels (0 to search all).
 */
static struct sdrl_binding *bindings_find(struct sdrl_environment *env, char *name, int type, int levels)
{
	struct sdrl_binding *cur;
	struct sdrl_environment *curenv;

	curenv = env;
	while (curenv) {
		cur = curenv->head;
		while (cur) {
			if ((!type || (cur->type == type)) && !strcmp(name, cur->name))
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


