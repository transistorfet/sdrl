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

struct environment *create_environment(struct environment *parent, uubyte bitflags)
{
	struct environment *env;

	if (!(env = (struct environment *) malloc(sizeof(struct environment))))
		return(NULL);

	env->bitflags = bitflags;
	env->head = NULL;
	env->tail = NULL;
	env->parent = parent;
	return(env);
}

int destroy_environment(struct environment *env)
{
	struct binding *cur, *next;

	cur = env->head;
	while (cur) {
		next = cur->next;
		destroy_value(cur->val);
		free(cur);
		cur = next;
	}
	free(env);
	return(0);
}

int add_binding(struct environment *env, char *name, struct value *val)
{
	struct binding *tmp;

	if ((!name))
		return(-1);

	if (tmp = find_local_binding(env, name))
		return(ERR_INUSE);

	if (!(tmp = (struct binding *) malloc(sizeof(struct binding) + strlen(name) + 1)))
		return(ERR_OUTOFMEMORY);

	tmp->name = (char *) ((usize_t) tmp + sizeof(struct binding));
	strcpy(tmp->name, name);
	if (val)
		val->binds++;
	tmp->val = val;


	tmp->next = NULL;
	if (env->tail)
		env->tail->next = tmp;
	else
		env->head = tmp;
	env->tail = tmp;

	return(0);
}

int remove_binding(struct environment *env, char *name)
{
	struct binding *cur, *prev;

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
			if (cur->val) {
				if (!(--cur->val->binds)) {
					if (cur->val->type == VT_ENVIRONMENT)
						destroy_environment((struct environment *) cur->val->data.val);
					destroy_value(cur->val);
				}
			}
			free(cur);
			return(0);
		}
		prev = cur;
		cur = cur->next;
	}
	return(ERR_NOTFOUND);
}

int replace_binding(struct environment *env, char *name, struct value *val)
{
	struct binding *tmp;

	if (tmp = find_binding(env, name)) {
		if (tmp->val) {
			if (!(--tmp->val->binds)) {
				if (tmp->val->type == VT_ENVIRONMENT)
					destroy_environment((struct environment *) tmp->val->data.val);
				destroy_value(tmp->val);
			}
		}
		tmp->val = val;
		return(0);
	}
	return(add_binding(env, name, val));
}

struct binding *find_binding(struct environment *env, char *name)
{
	struct binding *cur;
	struct environment *curenv;

	curenv = env;
	while (curenv) {
		cur = curenv->head;
		while (cur) {
			if (!strcmp(name, cur->name))
				return(cur);
			cur = cur->next;
		}
		do {
			curenv = curenv->parent;
		} while ((curenv) && (curenv->bitflags & EBF_PRIVATE));
	}
	return(NULL);
}

struct binding *find_local_binding(struct environment *env, char *name)
{
	struct binding *cur;

	cur = env->head;
	while (cur) {
		if (!strcmp(name, cur->name))
			return(cur);
		cur = cur->next;
	}
	return(NULL);
}

struct environment *find_environment(struct environment *env, char *name)
{
	struct binding *bind;

	if (!(bind = find_binding(env, name)) || (bind->val->type != VT_ENVIRONMENT))
		return(NULL);
	return((struct environment *) bind->val->data.val);	
}

