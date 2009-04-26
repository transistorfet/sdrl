/*
 * Name:	input.c
 * Description:	Input Manager
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sdrl/globals.h>
#include <sdrl/lib/base/input.h>

static int input_free_source(sdInput *);

/**
 * Create an initially empty input stack.
 */
sdInput *sdrl_make_input(void)
{
	sdInput *input;

	if (!(input = (sdInput *) malloc(sizeof(sdInput))))
		return(NULL);

	input->peek = 0;
	input->stack = NULL;

	return(input);
}

/**
 * Free the resources used by the input stack.
 */
void sdrl_input_destroy(sdInput *input)
{
	while (input->stack)
		input_free_source(input);
	free(input);
}

/**
 * Add a file to the top of the input stack.
 */
int sdrl_add_file(sdInput *input, const char *filename)
{
	FILE *fptr;
	sdSource *source;

	if (!(fptr = fopen(filename, "rb")))
		return(SDRL_ERR_NOT_FOUND);
	if (!(source = (sdSource *) malloc(sizeof(sdSource)))) {
		fclose(fptr);
		return(SDRL_ERR_OUT_OF_MEMORY);
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
int sdrl_add_string(sdInput *input, const char *str, int size)
{
	char *cpy_str;
	sdSource *source;

	if (!size)
		size = strlen(str);
	if (!(cpy_str = (char *) malloc(size + 1)))
		return(SDRL_ERR_OUT_OF_MEMORY);
	if (!(source = (sdSource *) malloc(sizeof(sdSource)))) {
		free(cpy_str);
		return(SDRL_ERR_OUT_OF_MEMORY);
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
 * Get the next char from the input stream.
 */
char sdrl_get_char(sdInput *input)
{
	char ch = 0;

	if (input->peek) {
		ch = input->peek;
		input->peek = 0;
		return(ch);
	}
	else if (!input->stack)
		return(0);
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
char sdrl_peek_char(sdInput *input)
{
	if (!input->peek)
		input->peek = sdrl_get_char(input);
	return(input->peek);
}

/**
 * Re-inserts the given character into the input stream such that the next call
 * to sdrl_get_char will return the given character.  Note: this can only be
 * done with one character.  Ungetting a second character will erase the
 * previously unget character.
 */
void sdrl_unget_char(sdInput *input, char ch)
{
	input->peek = ch;
}

/**
 * Return the current line/column number of the input stream.
 */
linenumber_t sdrl_get_linenumber(sdInput *input)
{
	if (input && input->stack)
		return(SDRL_MAKE_LINENUMBER(input->stack->line, input->stack->col));
	return(0);
}

/*** Local Function ***/

/**
 * Free the current input source on the input stack
 */
static int input_free_source(sdInput *input)
{
	sdSource *tmp;

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




