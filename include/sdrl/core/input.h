/*
 * Name:	input.h
 * Description:	Input/Output Manager Header
 */


#ifndef _SDRL_CORE_INPUT_H
#define _SDRL_CORE_INPUT_H

#include <stdio.h>
#include <sdrl/core/input.h>
#include <sdrl/globals.h>

#define SDRL_IT_FILE		0x01
#define SDRL_IT_STRING		0x02

typedef int (*sdrl_char_test_t)(char);

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
int sdrl_add_file(struct sdrl_input *, char *);
int sdrl_add_string(struct sdrl_input *, char *, int);

number_t sdrl_read_number(struct sdrl_input *);
char *sdrl_read_string(struct sdrl_input *, char);
char *sdrl_read_word(struct sdrl_input *, sdrl_char_test_t);

char sdrl_get_input(struct sdrl_input *);
char sdrl_get_char(struct sdrl_input *);

char sdrl_peek_input(struct sdrl_input *);
char sdrl_peek_char(struct sdrl_input *);

linenumber_t sdrl_get_linenumber(struct sdrl_input *);

int sdrl_is_digit(char);
int sdrl_is_space(char);
int sdrl_is_word(char);

#endif

