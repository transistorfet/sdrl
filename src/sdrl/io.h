/*
 * Name:	io.h
 * Version:	0.1
 * Description:	Input/Output Manager Header
 */


#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

#define IBF_ALLOCATED		0x01
#define IBF_ENDOFINPUT		0x02
#define IBF_REPEAT		0x10

struct source {
	uubyte bitflags;
	int i;
	char *str;
};

struct input_stack {
	int isp;
	int size;
	struct source *instack;	
};


struct input_stack *create_input_stack(int);
int destroy_input_stack(struct input_stack *);

int io_loadfile(struct input_stack *, char *);
int io_stackstring(struct input_stack *, char *, uubyte);
int io_changeflags(struct input_stack *, int, uubyte, uubyte);

char io_getchar(struct input_stack *);
char io_peekchar(struct input_stack *);
char io_getrawchar(struct input_stack *);
int io_ungetchar(struct input_stack *, int);
int io_pushnull(struct input_stack *);
char *io_getprevchars(struct input_stack *, int);
int io_parse_preop(struct input_stack *);
char *io_convertslashes(char *);

#endif

