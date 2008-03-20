/*
 * Parser Name:		lispy.c
 * Description:		Lambda Calculus Parser (Bracketed)
 */

#include <stdlib.h>

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>
#include <sdrl/lib/base/input.h>

#define LISPY_MAX_NUMBER		128
#define LISPY_MAX_STRING		1024

static struct sdrl_expr *lispy_parse_input(struct sdrl_type *, int, struct sdrl_input *);
static struct sdrl_expr *lispy_parse_expr(struct sdrl_type *, int, struct sdrl_input *);
static struct sdrl_expr *lispy_parse_number(struct sdrl_type *, struct sdrl_input *, char, linenumber_t);
static struct sdrl_expr *lispy_parse_string(struct sdrl_type *, struct sdrl_input *, char, linenumber_t);
static struct sdrl_expr *lispy_parse_identifier(struct sdrl_type *, struct sdrl_input *, char, linenumber_t);
static int lispy_get_next_char(struct sdrl_input *);
static inline int lispy_is_identifier(char);
static inline int lispy_is_digit(char);
static inline int lispy_is_whitespace(char);
static inline int lispy_is_word(char);
static inline char lispy_escape_char(char);

/**
 * Parse the input stream until EOF and return the expression tree
 */
struct sdrl_expr *sdrl_base_parse_lispy_input(struct sdrl_machine *mach, struct sdrl_input *input)
{
	struct sdrl_type *expr_type;

	if (!(expr_type = sdrl_find_binding(mach->type_env, "*expr*")))
		return(NULL);
	return(lispy_parse_input(expr_type, 0, input));
}

/*** Local Functions ***/

/**
 * Parse input stream.
 */
struct sdrl_expr *lispy_parse_input(struct sdrl_type *type, int openfunc, struct sdrl_input *input)
{
	struct sdrl_expr *head, *cur;

	head = cur = lispy_parse_expr(type, openfunc, input);
	while (cur) {
		cur->next = lispy_parse_expr(type, 0, input);
		cur = cur->next;
	}
	return(head);
}

/**
 * Parse a single expression.
 */
static struct sdrl_expr *lispy_parse_expr(struct sdrl_type *type, int openfunc, struct sdrl_input *input)
{
	char ch;
	linenumber_t line;
	struct sdrl_expr *expr;

	line = sdrl_get_linenumber(input);
	ch = lispy_get_next_char(input);

	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (lispy_is_digit(ch) || ((ch == '-') && lispy_is_digit(sdrl_peek_char(input)))) {
		if (!(expr = lispy_parse_number(type, input, ch, line)))
			return(NULL);
	}
	else if ((ch == '\"') || (ch == '\'')) {
		if (!(expr = lispy_parse_string(type, input, ch, line)))
			return(NULL);
	}
	else if (ch == '(') {
		if(!(expr = lispy_parse_input(type, 1, input)))
			return(NULL);
		expr = sdrl_make_call_expr(type, line, expr, NULL);
	}
	else {
		if (!(expr = lispy_parse_identifier(type, input, ch, line)))
			return(NULL);
		if (!openfunc) {
			if (!(expr = sdrl_make_string_expr(type, line, "$", expr))
			    || !(expr = sdrl_make_call_expr(type, line, expr, NULL)))
				return(NULL);
		}
	}
	return(expr);
}

static struct sdrl_expr *lispy_parse_number(struct sdrl_type *type, struct sdrl_input *input, char first, linenumber_t line)
{
	int i = 0;
	char buffer[LISPY_MAX_NUMBER];

	// TODO make this parse octal and hex numbers
	buffer[0] = first;
	while ((i < LISPY_MAX_NUMBER - 1) && (buffer[++i] = sdrl_get_char(input))) {
		if (!lispy_is_identifier(buffer[i]))
			break;
	}
	if (buffer[i] == ')')
		sdrl_unget_char(input, ')');
	buffer[i] = '\0';
	return(sdrl_make_number_expr(type, line, strtod(buffer, NULL), NULL));
}

static struct sdrl_expr *lispy_parse_string(struct sdrl_type *type, struct sdrl_input *input, char first, linenumber_t line)
{
	int i;
	char ch;
	char buffer[LISPY_MAX_STRING];

	for (i = 0; (i < LISPY_MAX_STRING) && (buffer[i] = sdrl_get_char(input)); i++) {
		if ((buffer[i] == '\\') && (first == '\"')) {
			if (!(ch = sdrl_get_char(input)))
				break;
			buffer[i] = lispy_escape_char(ch);
		}
		else if (buffer[i] == first)
			break;
	}
	buffer[i] = '\0';
	return(sdrl_make_string_expr(type, line, buffer, NULL));
}

static struct sdrl_expr *lispy_parse_identifier(struct sdrl_type *type, struct sdrl_input *input, char first, linenumber_t line)
{
	int i;
	char buffer[LISPY_MAX_STRING];

	buffer[0] = first;
	for (i = 1; (i < LISPY_MAX_STRING) && (buffer[i] = sdrl_get_char(input)); i++) {
		if (!lispy_is_identifier(buffer[i]))
			break;
	}
	if (buffer[i] == ')')
		sdrl_unget_char(input, ')');
	buffer[i] = '\0';
	return(sdrl_make_string_expr(type, line, buffer, NULL));
}

static int lispy_get_next_char(struct sdrl_input *input)
{
	char ch;

	while ((ch = sdrl_get_char(input))) {
		if (ch == '#') {
			while ((ch = sdrl_get_char(input)) && (ch != '\n')) ;
		}
		else if (!lispy_is_whitespace(ch))
			break;
	}
	return(ch);
}

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static inline int lispy_is_identifier(char ch)
{
	if ((ch != '(') && (ch != ')') && !lispy_is_whitespace(ch))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid digit character, 0 otherwise.
 */
static inline int lispy_is_digit(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid space character, 0 otherwise.
 */
static inline int lispy_is_whitespace(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r'))
		return(1);
	return(0);
}

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static inline int lispy_is_word(char ch)
{
	if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_'))
		return(1);
	return(0);
}

/**
 * Returns the character that corresponds to the escape code ch.
 */
static inline char lispy_escape_char(char ch)
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


