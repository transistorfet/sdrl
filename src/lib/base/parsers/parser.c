/*
 * Parser Name:	parser.c
 * Version:	0.2
 * Description:	SDRL Parser
 */

#include <sdrl.h>
#include <lib/base.h>


static int parser_is_identifier(char);

/**
 * Parse the input stream until EOF and return the expression tree.  (Param is not used).
 */
struct sdrl_expr *sdrl_base_parse_input(struct sdrl_input *input, void *param)
{
	struct sdrl_expr *head, *cur;

	head = cur = sdrl_base_parse_expr(input, param);
	while (cur) {
		cur->next = sdrl_base_parse_expr(input, param);
		cur = cur->next;
	}
	return(head);
}

/**
 * Parse a single expression.  (Param is not used).
 */
struct sdrl_expr *sdrl_base_parse_expr(struct sdrl_input *input, void *param)
{
	char ch;
	linenumber_t line;
	struct sdrl_expr *expr;

	ch = sdrl_peek_input(input);
	line = sdrl_get_linenumber(input);

	if (!ch)
		return(NULL);
	else if (ch == ')')
		return(NULL);
	else if (sdrl_is_digit(ch)) {
		if (!(expr = sdrl_make_number_expr(line, sdrl_read_number(input), NULL)))
			return(NULL);
	}
	else if (ch == '\"') {
		sdrl_get_char(input);
		if (!(expr = sdrl_make_string_expr(line, sdrl_read_string(input, '\"'), NULL)))
			return(NULL);
	}
	else {
		if (!(expr = sdrl_make_string_expr(line, sdrl_read_word(input, parser_is_identifier), NULL)))
			return(NULL);
		if (sdrl_peek_input(input) == '(') {
			sdrl_get_char(input);
			expr->next = sdrl_base_parse_input(input, param);
			if (sdrl_get_input(input) != ')')
				return(NULL);
			expr = sdrl_make_call_expr(line, expr, NULL);
		}
	}

	if (sdrl_peek_input(input) == ',')
		sdrl_get_char(input);
	return(expr);
}

/*** Local Functions ***/

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static int parser_is_identifier(char ch)
{
	if ((ch != '(') && (ch != ')') && (ch != ','))
		return(1);
	return(0);
}


