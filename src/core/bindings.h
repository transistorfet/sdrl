/*
 * Name:	bindings.h
 * Version:	0.2
 * Description:	Bindings Manager Header
 */


#ifndef _SDRL_CORE_BINDINGS_H
#define _SDRL_CORE_BINDINGS_H

#include "globals.h"

#define SDRL_BBF_NO_ADD			0x0001			/** No bindings can be added */
#define SDRL_BBF_NO_REPLACE		0x0002			/** No bindings can be replaced */
#define SDRL_BBF_NO_REMOVE		0x0004			/** No bindings can be removed */
#define SDRL_BBF_CONSTANT		0x0006			/** No bindings can be replaced or removed */

struct sdrl_binding {
	char *name;
	void *data;
	struct sdrl_binding *next;
};

struct sdrl_environment {
	int bitflags;
	int (*destroy)(void *);
	struct sdrl_binding *head;
	struct sdrl_binding *tail;
	struct sdrl_environment *parent;
};

struct sdrl_environment *sdrl_create_environment(int, int (*)(void *));
struct sdrl_environment *sdrl_extend_environment(struct sdrl_environment *);
int sdrl_destroy_environment(struct sdrl_environment *);

int sdrl_add_binding(struct sdrl_environment *, char *, void *);
int sdrl_replace_binding(struct sdrl_environment *, char *, void *);
int sdrl_remove_binding(struct sdrl_environment *, char *);

void *sdrl_find_binding(struct sdrl_environment *, char *);

#endif

