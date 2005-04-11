/*
 * Name:	input.c
 * Version:	0.2
 * Description:	Input Manager
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "globals.h"

static int input_free_source(struct sdrl_input *);

/**
 * Create an initially empty input stack.
 */
struct sdrl_input *sdrl_create_input(void)
{
	struct sdrl_input *input;

	if (!(input = (struct sdrl_input *) malloc(sizeof(struct sdrl_input))))
		return(NULL);

	input->peek = 0;
	input->stack = NULL;

	return(input);
}

/**
 * Free the resources used by the input stack.
 */
int sdrl_destroy_input(struct sdrl_input *input)
{
	while (input->stack)
		input_free_source(input);
	free(input);
	return(0);
}

/**
 * Add a file to the top of the input stack.
 */
int sdrl_add_file(struct sdrl_input *input, char *filename)
{
	FILE *fptr;
	struct sdrl_source *source;

	if (!(fptr = fopen(filename, "rb")))
		return(ERR_NOT_FOUND);
	if (!(source = (struct sdrl_source *) malloc(sizeof(struct sdrl_source)))) {
		fclose(fptr);
		return(ERR_OUT_OF_MEMORY);
	}
	source->type = SDRL_IT_FILE;
	source->line = 1;
	source->col = 1;
	source->i = 0;
	source->ptr.fptr = fptr;
	source->next = input->stack;
	input->stack = source;
	return(0);
}

/**
 * Add a copy of the string to the top of the input stack.
 */
int sdrl_add_string(struct sdrl_input *input, char *str, int size)
{
	char *cpy_str;
	struct sdrl_source *source;

	if (!size)
		size = strlen(str);
	if (!(cpy_str = (char *) malloc(size + 1)))
		return(ERR_OUT_OF_MEMORY);
	if (!(source = (struct sdrl_source *) malloc(sizeof(struct sdrl_source)))) {
		free(cpy_str);
		return(ERR_OUT_OF_MEMORY);
	}
	strncpy(cpy_str, str, size);
	cpy_str[size] = '\0';
	source->type = SDRL_IT_STRING;
	source->line = 1;
	source->col = 1;
	source->i = 0;
	source->ptr.str = cpy_str;
	source->next = input->stack;
	input->stack = source;
	return(0);
}

/**
 * Returns the next char read from the input stream ignoring comments and spaces
 */
char sdrl_get_char(struct sdrl_input *input)
{
	char ret;

	while (input->stack && (ret = sdrl_get_raw_char(input))) {
		if (ret == '#') {
			while (ret = sdrl_get_raw_char(input))
				if (ret == '\n')
					break;
		}
		else if (ret == ' ' || ret == '\n' || ret == '\t' || ret == '\r') {
			while (ret = sdrl_get_raw_char(input)) {
				if (ret != ' ' && ret != '\n' && ret != '\t' && ret != '\r') {
					input->peek = ret;
					break;
				}
			}
		}
		else
			return(ret);
		if (!ret)
			input_free_source(input);
	}

	return(0);
}

/**
 * Get the next char from the input stream.
 */
char sdrl_get_raw_char(struct sdrl_input *input)
{
	char ch = 0;

	if (input->peek) {
		ch = input->peek;
		input->peek = 0;
	}
	else if (input->stack->type == SDRL_IT_STRING)
		ch = input->stack->ptr.str[input->stack->i++];
	else if ((input->stack->type == SDRL_IT_FILE) && (!feof(input->stack->ptr.fptr))) {
		ch = fgetc(input->stack->ptr.fptr);
		if (ch == -1)
			ch = '\n';
	}

	input->stack->col++;
	if (ch == '\n') {
		input->stack->col = 1;
		input->stack->line++;
	}
	return(ch);
}

/**
 * Returns the next character to be read from input (only one char ahead).
 */
char sdrl_peek_char(struct sdrl_input *input)
{
	if (!input->peek)
		input->peek = sdrl_get_char(input);
	return(input->peek);
}

/**
 * Return the current line/column number of the input stream.
 */
linenumber_t sdrl_get_linenumber(struct sdrl_input *input)
{
	return(sdrl_make_linenumber_m(input->stack->line, input->stack->col));
}

/*** Local Function ***/

/**
 * Free the current input source on the input stack
 */
static int input_free_source(struct sdrl_input *input)
{
	struct sdrl_source *tmp;

	if (!input->stack)
		return(-1);
	if (input->stack->type == SDRL_IT_FILE)
		fclose(input->stack->ptr.fptr);
	else if (input->stack->type == SDRL_IT_STRING)
		free(input->stack->ptr.str);
	tmp = input->stack->next;
	free(input->stack);
	input->stack = tmp;
	return(0);
}


