/*
 * Name:	parse.c
 * Version:	0.1
 * Description:	Parser
 */

#include <stdlib.h>

#include "expr.h"
#include "input.h"
#include "parse.h"
#include "globals.h"

#define PARSE_MAX_BUFFER		1024


static number_t parse_get_number(struct sdrl_input *);
static char *parse_get_string(struct sdrl_input *);
static char *parse_get_name(struct sdrl_input *);
static char parse_escape_char(char);
static int parse_is_digit(char);
static int parse_is_number(char);
static int parse_is_name(char);

static char parse_buffer[PARSE_MAX_BUFFER + 1];

/**
 * Parse the input file and return the expression tree
 */
struct sdrl_expr *sdrl_parse_file(char *filename)
{
	struct sdrl_expr *expr;
	struct sdrl_input *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_file(input, filename)) {
		sdrl_destroy_input(input);
		return(NULL);
	}

	expr = sdrl_parse_input(input);

	sdrl_destroy_input(input);
	return(expr);
}

/**
 * Parse the input string and return the expression tree
 */
struct sdrl_expr *sdrl_parse_string(char *str, int size)
{
	struct sdrl_expr *expr;
	struct sdrl_input *input;

	if (!(input = sdrl_create_input()))
		return(NULL);
	if (sdrl_add_string(input, str, size)) {
		sdrl_destroy_input(input);
		return(NULL);
	}

	expr = sdrl_parse_input(input);

	sdrl_destroy_input(input);
	return(expr);
}

/**
 * Parse the input stream until EOF and return the expression tree
 */
struct sdrl_expr *sdrl_parse_input(struct sdrl_input *input)
{
	struct sdrl_expr *head, *cur;

	head = cur = sdrl_parse_expr(input);
	while (cur) {
		cur->next = sdrl_parse_expr(input);
		cur = cur->next;
	}
	return(head);
}

/**
 * Parse a single expression.
 */
struct sdrl_expr *sdrl_parse_expr(struct sdrl_input *input)
{
	char ch;
	struct sdrl_expr *expr;

	ch = sdrl_peek_char(input);
	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (parse_is_digit(ch)) {
		if (!(expr = sdrl_make_number_expr(parse_get_number(input), NULL)))
			return(NULL);
	}
	else if (ch == '\"') {
		sdrl_get_char(input);
		if (!(expr = sdrl_make_string_expr(parse_get_string(input), NULL)))
			return(NULL);
	}
	else {
		if (!(expr = sdrl_make_string_expr(parse_get_name(input), NULL)))
			return(NULL);
		if (sdrl_peek_char(input) == '(') {
			sdrl_get_char(input);
			expr->next = sdrl_parse_input(input);
			if (sdrl_get_char(input) != ')')
				return(NULL);
			expr = sdrl_make_call_expr(expr, NULL);
		}
	}

	if (sdrl_peek_char(input) == ',')
		sdrl_get_char(input);
	return(expr);
}

/**
 * Add a parse rule.
 */
int sdrl_add_parse_rule(char *name, char *pattern, struct sdrl_expr *expr)
{

}


/*** Local Functions ***/

/**
 * Returns the number that was read from input.
 */
static number_t parse_get_number(struct sdrl_input *input)
{
	char ch;
	int i = 0;

	while ((i < PARSE_MAX_BUFFER) && (ch = sdrl_peek_char(input)) && parse_is_number(ch))
		parse_buffer[i++] = sdrl_get_char(input);
	parse_buffer[i] = '\0';
	return(atof(parse_buffer));
}

/*
 * Returns the quoted string that was read from input.
 */
static char *parse_get_string(struct sdrl_input *input)
{
	char ch;
	int i = 0;

	while ((i < PARSE_MAX_BUFFER) && (ch = sdrl_get_raw_char(input)) && (ch != '\"')) {
		if (ch == '\\')
			ch = parse_escape_char(sdrl_get_raw_char(input));
		parse_buffer[i++] = ch;
	}
	parse_buffer[i] = '\0';
	return(parse_buffer);
}

/*
 * Returns the name that was read from input.
 */
static char *parse_get_name(struct sdrl_input *input)
{
	char ch;
	int i = 0;

	while ((i < PARSE_MAX_BUFFER) && (ch = sdrl_peek_char(input)) && parse_is_name(ch))
		parse_buffer[i++] = sdrl_get_char(input);
	parse_buffer[i] = '\0';
	return(parse_buffer);
}

/**
 * Returns the character that corresponds to the escape code ch.
 */
static char parse_escape_char(char ch)
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

/**
 * Returns 1 if ch is a valid digit character, 0 otherwise.
 */
static int parse_is_digit(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid number character, 0 otherwise.
 */
static int parse_is_number(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39) || (ch == '.') || (ch == '-'))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid name character, 0 otherwise.
 */
static int parse_is_name(char ch)
{
	return((ch != '(') && (ch != ')') && (ch != ','));
}


