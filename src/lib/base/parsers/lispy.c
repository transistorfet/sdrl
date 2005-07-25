/*
 * Parser Name:		lispy.c
 * Version:		0.2
 * Description:		Lispy Like Parser
 */

#include <sdrl/sdrl.h>
#include <sdrl/lib/base.h>

static int lispy_is_identifier(char);

/**
 * Parse the input stream until EOF and return the expression tree
 */
struct sdrl_expr *sdrl_base_parse_lispy_input(struct sdrl_input *input, void *param)
{
	struct sdrl_expr *head, *cur;

	head = cur = sdrl_base_parse_lispy_expr(input, param);
	while (cur) {
		cur->next = sdrl_base_parse_lispy_expr(input, param);
		cur = cur->next;
	}
	return(head);
}

/**
 * Parse a single expression.
 */
struct sdrl_expr *sdrl_base_parse_lispy_expr(struct sdrl_input *input, void *param)
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
	else if (ch == '(') {
		sdrl_get_char(input);
		expr = sdrl_base_parse_lispy_input(input, param);
		if (sdrl_get_input(input) != ')')
			return(NULL);
		expr = sdrl_make_call_expr(line, expr, NULL);
	}
	else {
		if (!(expr = sdrl_make_string_expr(line, sdrl_read_word(input, lispy_is_identifier), NULL)))
			return(NULL);
	}

	while (sdrl_is_space(sdrl_peek_char(input)))
		sdrl_get_char(input);

	return(expr);
}

/*** Local Functions ***/

/**
 * Returns 1 if ch is a valid word character, 0 otherwise.
 */
static int lispy_is_identifier(char ch)
{
	if ((ch != '(') && (ch != ')') && !sdrl_is_space(ch))
		return(1);
	return(0);
}


