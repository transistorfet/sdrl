/*
 * Name:	input.h
 * Description:	Input/Output Manager Header
 */


#ifndef _SDRL_LIB_BASE_INPUT_H
#define _SDRL_LIB_BASE_INPUT_H

#include <stdio.h>

#include <sdrl/globals.h>

#define SDRL_IT_FILE		0x01
#define SDRL_IT_STRING		0x02

struct sdrl_source {
	int type;
	short line;
	short col;
	int i;
	union {
		char *str;
		FILE *fptr;
	} ptr;
	struct sdrl_source *next;
};

struct sdrl_input {
	char peek;
	struct sdrl_source *stack;	
};

struct sdrl_input *sdrl_create_input(void);
int sdrl_destroy_input(struct sdrl_input *);
int sdrl_add_file(struct sdrl_input *, const char *);
int sdrl_add_string(struct sdrl_input *, const char *, int);

char sdrl_get_char(struct sdrl_input *);
char sdrl_peek_char(struct sdrl_input *);
void sdrl_unget_char(struct sdrl_input *, char);

linenumber_t sdrl_get_linenumber(struct sdrl_input *);

#endif

