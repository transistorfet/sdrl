/*
 * Name:	parse.c
 * Version:	0.1
 * Description:	Parser
 */

#include <stdlib.h>

#include "expr.h"
#include "input.h"
#include "parse.h"
#include "types.h"

#define PARSE_MAX_BUFFER		1024


static int parse_is_digit(char);
static int parse_is_number(char);
static int parse_is_name(char);
static number_t parse_get_number(struct sdrl_input *);
static char *parse_get_name(struct sdrl_input *);

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
 * Parse the input stream until EOF and return the expression tree
 */
struct sdrl_expr *sdrl_parse_input(struct sdrl_input *input)
{
	char ch;
	struct sdrl_expr *expr, *cur;

	ch = sdrl_peek_char(input);
	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (ch == '(')
		return(NULL);
	else if (parse_is_digit(ch)) {
		return(sdrl_make_number_expr(parse_get_number(input), NULL));
	}
	else {
		if (!(expr = sdrl_make_name_expr(parse_get_name(input), NULL)))
			return(NULL);
		ch = sdrl_peek_char(input);
		if (ch == '(') {
			sdrl_get_char(input);
			expr->next = sdrl_parse_input(input);
			if (sdrl_get_char(input) != ')')
				return(NULL);
			return(sdrl_make_call_expr(expr, sdrl_parse_input(input)));
		}
		else if (ch == ',') {
			sdrl_get_char(input);
			expr->next = sdrl_parse_input(input);
			return(expr);
		}
		else
			return(expr);
	}
}


/*** Local Functions ***/

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

/**
 * Returns the number that was read from input.
 */
static number_t parse_get_number(struct sdrl_input *input)
{
	char ch;
	int i = 0;
	char buffer[PARSE_MAX_BUFFER];

	while ((ch = sdrl_peek_char(input)) && parse_is_number(ch))
		buffer[i++] = sdrl_get_char(input);
	buffer[i] = '\0';
	return(atof(buffer));
}

/*
 * Returns the name that was read from input.
 */
static char *parse_get_name(struct sdrl_input *input)
{
	char ch;
	int i = 0;
	char *str;
	char buffer[PARSE_MAX_BUFFER];

	while ((ch = sdrl_peek_char(input)) && parse_is_name(ch))
		buffer[i++] = sdrl_get_char(input);
	buffer[i] = '\0';

	if (!(str = (char *) malloc(strlen(buffer))))
		return(NULL);
	strcpy(str, buffer);
	return(str);
}


