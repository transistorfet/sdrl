/*
 * Name:	bindings.h
 * Version:	0.1
 * Description:	Bindings Manager Header
 */


#ifndef _SDRL_BINDINGS_H
#define _SDRL_BINDINGS_H

#include "type.h"
#include "value.h"
#include "globals.h"

#define SDRL_BT_TYPE		0x01
#define SDRL_BT_VALUE		0x02

#define SDRL_EBF_PRIVATE	0x01

typedef union {
	struct sdrl_type *type;
	struct sdrl_value *value;
} sdrl_bind_t;

struct sdrl_binding {
	char *name;
	int type;
	sdrl_bind_t data;
	struct sdrl_binding *next;
};

struct sdrl_environment {
	int bitflags;
	struct sdrl_binding *head;
	struct sdrl_binding *tail;
	struct sdrl_environment *parent;
};

struct sdrl_environment *sdrl_create_environment(int, struct sdrl_environment *);
int sdrl_destroy_environment(struct sdrl_environment *);

int sdrl_bind_type(struct sdrl_environment *, char *, struct sdrl_type *);
int sdrl_bind_value(struct sdrl_environment *, char *, struct sdrl_value *);
int sdrl_rebind_value(struct sdrl_environment *, char *, struct sdrl_value *);
int sdrl_remove_binding(struct sdrl_environment *, char *);

struct sdrl_type *sdrl_find_type(struct sdrl_environment *, char *);
struct sdrl_value *sdrl_find_value(struct sdrl_environment *, char *);
struct sdrl_value *sdrl_find_local_value(struct sdrl_environment *, char *);

#endif

