/*
 * Name:	bindings.h
 * Version:	0.1
 * Description:	Bindings Manager Header
 */


#ifndef BINDINGS_H
#define BINDINGS_H

#include "value.h"
#include "types.h"

#define SDRL_EBF_PRIVATE		0x01

struct sdrl_binding {
	char *name;
	struct sdrl_value *value;
	struct sdrl_binding *next;
};

struct sdrl_environment {
	u_char bitflags;
	struct sdrl_binding *head;
	struct sdrl_binding *tail;
	struct sdrl_environment *parent;
};

struct sdrl_environment *sdrl_create_environment(u_char, struct sdrl_environment *);
int sdrl_destroy_environment(struct sdrl_environment *);
int sdrl_add_binding(struct sdrl_environment *, char *, struct sdrl_value *);
int sdrl_remove_binding(struct sdrl_environment *, char *);
int sdrl_replace_binding(struct sdrl_environment *, char *, struct sdrl_value *);
struct sdrl_value *sdrl_find_binding(struct sdrl_environment *, char *);
struct sdrl_value *sdrl_find_local_binding(struct sdrl_environment *, char *);

#endif

