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

typedef struct sdSource sdSource;

struct sdSource {
	int type;
	short line;
	short col;
	int i;
	union {
		char *str;
		FILE *fptr;
	} ptr;
	sdSource *next;
};

struct sdInput {
	char peek;
	sdSource *stack;	
};

sdInput *sdrl_make_input(void);
void sdrl_input_destroy(sdInput *);
int sdrl_add_file(sdInput *, const char *);
int sdrl_add_string(sdInput *, const char *, int);

char sdrl_get_char(sdInput *);
char sdrl_peek_char(sdInput *);
void sdrl_unget_char(sdInput *, char);

linenumber_t sdrl_get_linenumber(sdInput *);

#endif

