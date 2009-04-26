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

static sdExpr *lambda_parse_input(sdHeap *, sdType *, sdInput *);
static sdExpr *lambda_parse_expr(sdHeap *, sdType *, sdInput *);
static sdExpr *lambda_parse_number(sdHeap *, sdType *, sdInput *, char, linenumber_t);
static sdExpr *lambda_parse_string(sdHeap *, sdType *, sdInput *, char, linenumber_t);
static sdExpr *lambda_parse_identifier(sdHeap *, sdType *, sdInput *, char, linenumber_t);
static int lambda_get_next_char(sdInput *);
static inline int lambda_is_identifier(char);
static inline int lambda_is_digit(char);
static inline int lambda_is_whitespace(char);
static inline int lambda_is_word(char);
static inline char lambda_escape_char(char);

/**
 * Parse the input stream until EOF and return the expression tree
 */
sdExpr *sdrl_base_parse_lambda_input(sdMachine *mach, sdInput *input)
{
	return(lambda_parse_input(mach->heap, &sdExprTypeDef, input));
}

/*** Local Functions ***/

/**
 * Parse input stream.
 */
sdExpr *lambda_parse_input(sdHeap *heap, sdType *type, sdInput *input)
{
	sdExpr *head, *cur;

	head = cur = lambda_parse_expr(heap, type, input);
	while (cur) {
		cur->next = lambda_parse_expr(heap, type, input);
		cur = cur->next;
	}
	return(head);
}

/**
 * Parse a single expression.
 */
static sdExpr *lambda_parse_expr(sdHeap *heap, sdType *type, sdInput *input)
{
	char ch;
	linenumber_t line;
	sdExpr *expr;

	line = sdrl_get_linenumber(input);
	ch = lambda_get_next_char(input);

	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (lambda_is_digit(ch) || ((ch == '-') && lambda_is_digit(sdrl_peek_char(input)))) {
		if (!(expr = lambda_parse_number(heap, type, input, ch, line)))
			return(NULL);
	}
	else if ((ch == '\"') || (ch == '\'')) {
		if (!(expr = lambda_parse_string(heap, type, input, ch, line)))
			return(NULL);
	}
	else if (ch == '(') {
		if(!(expr = lambda_parse_input(heap, type, input)))
			return(NULL);
		expr = sdrl_make_expr_expr(heap, type, SDRL_ET_CALL, line, expr, NULL);
	}
	else {
		if (!(expr = lambda_parse_identifier(heap, type, input, ch, line)))
			return(NULL);
	}
	return(expr);
}

static sdExpr *lambda_parse_number(sdHeap *heap, sdType *type, sdInput *input, char first, linenumber_t line)
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
	return(sdrl_make_number_expr(heap, type, SDRL_ET_NUMBER, line, strtod(buffer, NULL), NULL));
}

static sdExpr *lambda_parse_string(sdHeap *heap, sdType *type, sdInput *input, char first, linenumber_t line)
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
	return(sdrl_make_string_expr(heap, type, SDRL_ET_STRING, line, buffer, NULL));
}

static sdExpr *lambda_parse_identifier(sdHeap *heap, sdType *type, sdInput *input, char first, linenumber_t line)
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
	return(sdrl_make_string_expr(heap, type, SDRL_ET_IDENTIFIER, line, buffer, NULL));
}

static int lambda_get_next_char(sdInput *input)
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


