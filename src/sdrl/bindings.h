/*
 * Name:	bindings.h
 * Version:	0.1
 * Description:	Bindings Manager Header
 */


#ifndef BINDINGS_H
#define BINDINGS_H

#include "value.h"
#include "types.h"

#define EBF_PRIVATE		0x01

struct binding {
	char *name;
	struct value *val;
	struct binding *next;
};

struct environment {
	uubyte bitflags;
	struct binding *head;
	struct binding *tail;
	struct environment *parent;
};

struct environment *create_environment(struct environment *, uubyte);
int destroy_environment(struct environment *);
int add_binding(struct environment *, char *, struct value *);
int remove_binding(struct environment *, char *);
int replace_binding(struct environment *, char *, struct value *);
struct binding *find_binding(struct environment *, char *);
struct binding *find_local_binding(struct environment *, char *);
struct environment *find_environment(struct environment *, char *);

#endif

