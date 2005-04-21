/*
 * Name:	input.c
 * Version:	0.2
 * Description:	Input Manager
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sdrl/core/input.h>
#include <sdrl/globals.h>

#define INPUT_MAX_BUFFER		1024

#define sdrl_input_is_number(ch) \
	((ch >= 0x30) && (ch <= 0x39) || (ch == '.') || (ch == '-'))

static char input_buffer[INPUT_MAX_BUFFER + 1];

static int input_free_source(struct sdrl_input *);
static char input_escape_char(char);

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
 * Returns the number that was read from input.
 */
number_t sdrl_read_number(struct sdrl_input *input)
{
	char ch;
	int i = 0;

	while ((i < INPUT_MAX_BUFFER) && (ch = sdrl_peek_input(input)) && sdrl_input_is_number(ch))
		input_buffer[i++] = sdrl_get_char(input);
	input_buffer[i] = '\0';
	return(atof(input_buffer));
}

/*
 * Returns the quoted string that was read from input.
 */
char *sdrl_read_string(struct sdrl_input *input, char term)
{
	char ch;
	int i = 0;

	while ((i < INPUT_MAX_BUFFER) && (ch = sdrl_get_char(input)) && (ch != term)) {
		if (ch == '\\')
			ch = input_escape_char(sdrl_get_char(input));
		input_buffer[i++] = ch;
	}
	input_buffer[i] = '\0';
	return(input_buffer);
}

/*
 * Returns the name that was read from input.
 */
char *sdrl_read_word(struct sdrl_input *input, sdrl_char_test_t is_word_char)
{
	char ch;
	int i = 0;

	while ((i < INPUT_MAX_BUFFER) && (ch = sdrl_peek_char(input)) && is_word_char(ch))
		input_buffer[i++] = sdrl_get_char(input);
	input_buffer[i] = '\0';
	return(input_buffer);
}


/**
 * Returns the next char read from the input stream ignoring comments and spaces
 */
char sdrl_get_input(struct sdrl_input *input)
{
	char ret;

	while (input->stack && (ret = sdrl_get_char(input))) {
		if (ret == '#') {
			while (ret = sdrl_get_char(input))
				if (ret == '\n')
					break;
		}
		else if (ret == ' ' || ret == '\n' || ret == '\t' || ret == '\r') {
			while (ret = sdrl_get_char(input)) {
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
char sdrl_get_char(struct sdrl_input *input)
{
	char ch = 0;

	if (input->peek) {
		ch = input->peek;
		input->peek = 0;
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
 * Returns the next character ignoring spaces and comments to be read from input (only one char ahead).
 */
char sdrl_peek_input(struct sdrl_input *input)
{
	if (!input->peek)
		input->peek = sdrl_get_input(input);
	return(input->peek);
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
	if (input && input->stack)
		return(sdrl_make_linenumber_m(input->stack->line, input->stack->col));
	return(0);
}


/**
 * Returns 1 if ch is a valid digit character, 0 otherwise.
 */
int sdrl_is_digit(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid space character, 0 otherwise.
 */
int sdrl_is_space(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r'))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
int sdrl_is_word(char ch)
{
	if ((ch >= 'A') && (ch <= 'Z') || (ch >= 'a') && (ch <= 'z') || (ch == '_'))
		return(1);
	return(0);
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

/**
 * Returns the character that corresponds to the escape code ch.
 */
static char input_escape_char(char ch)
{
	switch (ch) {
		case 't':
			return('\x09');
		case 'n':
			return('\x0a');
		default:
			return(ch);
	}
}



