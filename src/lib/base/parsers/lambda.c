/*
 * Parser Name:		lambda.c
 * Description:		Lambda Calculus Parser (Bracketed)
 */

#include <stdlib.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

#define LAMBDA_MAX_NUMBER		128
#define LAMBDA_MAX_STRING		1024

static struct sdrl_expr *lambda_parse_expr(struct sdrl_input *);
static struct sdrl_expr *lambda_parse_number(struct sdrl_input *, char, linenumber_t);
static struct sdrl_expr *lambda_parse_identifier(struct sdrl_input *, char, linenumber_t);
static struct sdrl_expr *lambda_parse_string(struct sdrl_input *, char, linenumber_t);
static int lambda_get_next_char(struct sdrl_input *);
static inline int lambda_is_identifier(char);
static inline int lambda_is_digit(char);
static inline int lambda_is_whitespace(char);
static inline int lambda_is_word(char);
static inline char lambda_escape_char(char);

/**
 * Parse the input stream until EOF and return the expression tree
 */
struct sdrl_expr *sdrl_base_parse_lambda_input(struct sdrl_input *input, void *param)
{
	struct sdrl_expr *head, *cur;

	head = cur = lambda_parse_expr(input);
	while (cur) {
		cur->next = lambda_parse_expr(input);
		cur = cur->next;
	}
	return(head);
}

/*** Local Functions ***/

/**
 * Parse a single expression.
 */
static struct sdrl_expr *lambda_parse_expr(struct sdrl_input *input)
{
	char ch;
	linenumber_t line;
	struct sdrl_expr *expr;

	ch = lambda_get_next_char(input);
	line = sdrl_get_linenumber(input);

	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (lambda_is_digit(ch) || ((ch == '-') && lambda_is_digit(sdrl_peek_char(input)))) {
		if (!(expr = lambda_parse_number(input, ch, line)))
			return(NULL);
	}
	else if ((ch == '\"') || (ch == '\'')) {
		if (!(expr = lambda_parse_string(input, ch, line)))
			return(NULL);
	}
	else if (ch == '(') {
		if(!(expr = sdrl_base_parse_lambda_input(input, NULL)))
			return(NULL);
		expr = sdrl_make_call_expr(line, expr, NULL);
	}
	else {
		if (!(expr = lambda_parse_identifier(input, ch, line)))
			return(NULL);
	}
	return(expr);
}

static struct sdrl_expr *lambda_parse_number(struct sdrl_input *input, char first, linenumber_t line)
{
	int i = 0;
	char buffer[LAMBDA_MAX_NUMBER];

	// TODO make this parse octal and hex numbers
	buffer[0] = first;
	while ((i < LAMBDA_MAX_NUMBER - 1) && (buffer[++i] = sdrl_get_char(input))) {
		if (!lambda_is_identifier(buffer[i]))
			break;
	}
	if (buffer[i] == ')')
		sdrl_unget_char(input, ')');
	buffer[i] = '\0';
	return(sdrl_make_number_expr(line, strtod(buffer, NULL), NULL));
}

static struct sdrl_expr *lambda_parse_string(struct sdrl_input *input, char first, linenumber_t line)
{
	int i;
	char ch;
	char buffer[LAMBDA_MAX_STRING];

	for (i = 0; (i < LAMBDA_MAX_STRING) && (buffer[i] = sdrl_get_char(input)); i++) {
		if ((buffer[i] == '\\') && (first == '\"')) {
			if (!(ch = sdrl_get_char(input)))
				break;
			buffer[i] = lambda_escape_char(ch);
		}
		else if (buffer[i] == first)
			break;
	}
	buffer[i] = '\0';
	return(sdrl_make_string_expr(line, buffer, NULL));
}

static struct sdrl_expr *lambda_parse_identifier(struct sdrl_input *input, char first, linenumber_t line)
{
	int i;
	char buffer[LAMBDA_MAX_STRING];

	buffer[0] = first;
	for (i = 1; (i < LAMBDA_MAX_STRING) && (buffer[i] = sdrl_get_char(input)); i++) {
		if (!lambda_is_identifier(buffer[i]))
			break;
	}
	if (buffer[i] == ')')
		sdrl_unget_char(input, ')');
	buffer[i] = '\0';
	return(sdrl_make_string_expr(line, buffer, NULL));
}

static int lambda_get_next_char(struct sdrl_input *input)
{
	char ch;

	while ((ch = sdrl_get_char(input))) {
		if (ch == '#') {
			while ((ch = sdrl_get_char(input)) && (ch != '\n')) ;
		}
		else if (!lambda_is_whitespace(ch))
			break;
	}
	return(ch);
}

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static inline int lambda_is_identifier(char ch)
{
	if ((ch != '(') && (ch != ')') && !lambda_is_whitespace(ch))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid digit character, 0 otherwise.
 */
static inline int lambda_is_digit(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid space character, 0 otherwise.
 */
static inline int lambda_is_whitespace(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r'))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static inline int lambda_is_word(char ch)
{
	if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_'))
		return(1);
	return(0);
}

/**
 * Returns the character that corresponds to the escape code ch.
 */
static inline char lambda_escape_char(char ch)
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


