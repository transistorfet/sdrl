/*
 * Name:	prims.c
 * Version:	0.1
 * Description:	Primative Functions
 */

#include <stdio.h>

#include <sdrl.h>
#include "prims.h"

#define traverse_value_m(type_name, value, init, line)					\
	if (!(type = sdrl_find_type(mach->type_env, type_name)))			\
		ret = ERR_NOT_FOUND;							\
	else {										\
		result = init;								\
		cur = value;								\
		while (cur) {								\
			if (cur->type != type) {					\
				ret = ERR_INVALID_TYPE;					\
				break;							\
			}								\
			line;								\
			cur = cur->next;						\
		}									\
		mach->ret = sdrl_make_value(type, (sdrl_data_t) result, 0, NULL);	\
	}

static int prim_set_list(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

/**
 * Add all the functions to the environment of machine
 */
int prim_initialize(struct sdrl_machine *mach)
{
	struct sdrl_type *form, *builtin;
	form = sdrl_find_type(mach->type_env, "form");
	builtin = sdrl_find_type(mach->type_env, "builtin");

	sdrl_bind_value(mach->env, "code", sdrl_make_value(form, (sdrl_data_t) (void *) prim_code, 0, NULL));
	sdrl_bind_value(mach->env, "set", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_set, 0, NULL));
	sdrl_bind_value(mach->env, "list", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_list, 0, NULL));
	sdrl_bind_value(mach->env, "if", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_if, 0, NULL));

	sdrl_bind_value(mach->env, "$", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_resolve, 0, NULL));
	sdrl_bind_value(mach->env, "+", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_add, 0, NULL));
	sdrl_bind_value(mach->env, "-", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_subtract, 0, NULL));
	sdrl_bind_value(mach->env, "*", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_multiply, 0, NULL));
	sdrl_bind_value(mach->env, "/", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_divide, 0, NULL));

	sdrl_bind_value(mach->env, "=", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_equals, 0, NULL));
	sdrl_bind_value(mach->env, "!=", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_not_equals, 0, NULL));
	sdrl_bind_value(mach->env, "<", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_less_than, 0, NULL));
	sdrl_bind_value(mach->env, ">", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_greater_than, 0, NULL));
	sdrl_bind_value(mach->env, "<=", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_less_than_equals, 0, NULL));
	sdrl_bind_value(mach->env, ">=", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_greater_than_equals, 0, NULL));

	sdrl_bind_value(mach->env, "print", sdrl_make_value(builtin, (sdrl_data_t) (void *) prim_print, 0, NULL));
	return(0);
}

/**
 * code(<expr>, ...)
 * Return a value of type expr containing the code passed to it.
 */
int prim_code(struct sdrl_machine *mach, struct sdrl_expr *expr)
{
	mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "expr"), (sdrl_data_t) (void *) expr, 0, NULL);
	return(0);
}

/**
 * set(<name>, <value>)
 * Bind value to name.  Returns a duplicate of value.
 */
int prim_set(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int i, ret = 0;
	struct sdrl_value *name;
	struct sdrl_type *string, *list;

	if ((i = sdrl_value_count(value)) < 2)
		ret = ERR_INVALID_PARAMS;
	else if (!(list = sdrl_find_type(mach->type_env, "list")))
		ret = ERR_NOT_FOUND;
	else {
		name = sdrl_shift_value(&value);
		if (name->type == list)
			ret = prim_set_list(mach, name->data.ptr, ((i == 2) && (value->type == list)) ? (struct sdrl_value *) value->data.ptr : value); 
		else
			ret = prim_set_list(mach, name, value); 
		sdrl_destroy_value(name);
	}
	if (ret)
		sdrl_destroy_value(value);
	else
		mach->ret = sdrl_duplicate_value(value);
	return(ret);
}

/**
 * list(<value>, ...)
 * Returns a value of type list containing the list of parameters.
 */
int prim_list(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *bind;

	mach->ret = sdrl_make_value(sdrl_find_type(mach->type_env, "list"), (sdrl_data_t) (void *) value, 0, NULL);
	return(0);
}

/**
 * if(<value>, <expr-value> [, <expr-value>])
 * Evaluates the first expr-value if value is not 0, the second otherwise.
 */
int prim_if(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	struct sdrl_value *name;

	if (!value || !value->next)
		ret = ERR_INVALID_PARAMS;
	else if (  ((sdrl_base_type_m(value->type) == SDRL_BT_NUMBER) && (!value->data.number))
		|| ((sdrl_base_type_m(value->type) == SDRL_BT_STRING) && (!value->data.str || value->data.str[0] == '\0'))
		|| ((sdrl_base_type_m(value->type) == SDRL_BT_POINTER) && (!value->data.ptr))) {
		if (value->next->next)
			ret = sdrl_evaluate_value(mach, value->next->next, NULL);
	}
	else {
		ret = sdrl_evaluate_value(mach, value->next, NULL);
	}
	sdrl_destroy_value(value);
	return(ret);
}


/**
 * $(<name>)
 * Returns a copy of the value bound to the given name.
 */
int prim_resolve(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	struct sdrl_value *bind;

	if (!value || (value->type != sdrl_find_type(mach->type_env, "string")))
		ret = ERR_INVALID_TYPE;
	else if (!(bind = sdrl_find_value(mach->env, value->data.str)))
		ret = ERR_NOT_FOUND;
	else
		mach->ret = sdrl_duplicate_value(bind);
	sdrl_destroy_value(value);
	return(ret);
}

/**
 * +(<number>, ...)
 * Returns a number of the sum of the list of number-values passes.
 */
int prim_add(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value, 0, result += cur->data.number);
	sdrl_destroy_value(value);
	return(ret);
}

/**
 * -(<number>, ...)
 * Returns a number of the difference of the list of number-values passes.
 */
int prim_subtract(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (value)
		traverse_value_m("number", value->next, value->data.number, result -= cur->data.number);
	sdrl_destroy_value(value);
	return(ret);
}

/**
 * *(<number>, ...)
 * Returns a number of the product of the list of number-values passes.
 */
int prim_multiply(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value, 1, result *= cur->data.number);
	sdrl_destroy_value(value);
	return(ret);
}

/**
 * /(<number>, ...)
 * Returns a number of the quotient of the list of number-values passes.
 */
int prim_divide(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 0;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	if (value)
		traverse_value_m("number", value->next, value->data.number, result /= cur->data.number);
	sdrl_destroy_value(value);
	return(ret);
}

/**
 * =(<value>, ...)
 * Returns 1 if all number values are equal to eachother, 0 otherwise.
 */
int prim_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;
/*
	struct sdrl_type *number;

	if (!(number = sdrl_find_type(mach->type_env, "number")))
		ret = ERR_NOT_FOUND;
	else {
		cur = value->next;
		while (cur) {
			if (cur->type != number) {
				ret = ERR_INVALID_TYPE;
				break;
			}
			if (cur->data.number != value->data.number) {
				result = 0;
				break;
			}
			cur = cur->next;
		}
		mach->ret = sdrl_make_value(number, (sdrl_data_t) result, 0, NULL);
	}
*/

	traverse_value_m("number", value->next, 1,
		if (cur->data.number != value->data.number) {
			result = 0;
			break;
		});

	sdrl_destroy_value(value);
	return(ret);
}

/**
 * !=(<value>, ...)
 * Returns 1 if number value is not equal to eachother, 0 otherwise.
 */
int prim_not_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur;
	struct sdrl_type *type;

	traverse_value_m("number", value->next, 1,
		if (cur->data.number == value->data.number) {
			result = 0;
			break;
		});

	sdrl_destroy_value(value);
	return(ret);
}

/**
 * <(<value>, ...)
 * Returns 1 if number value are less than eachother, 0 otherwise.
 */
int prim_less_than(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number >= cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	sdrl_destroy_value(value);
	return(ret);
}

/**
 * >(<value>, ...)
 * Returns 1 if number value are greater than eachother, 0 otherwise.
 */
int prim_greater_than(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number <= cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	sdrl_destroy_value(value);
	return(ret);
}

/**
 * <=(<value>, ...)
 * Returns 1 if number value are less than or equal to eachother, 0 otherwise.
 */
int prim_less_than_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number > cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	sdrl_destroy_value(value);
	return(ret);
}

/**
 * >=(<value>, ...)
 * Returns 1 if number value are greater than or equal to eachother, 0 otherwise.
 */
int prim_greater_than_equals(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int ret = 0;
	number_t result = 1;
	struct sdrl_value *cur, *last;
	struct sdrl_type *type;

	last = value;
	traverse_value_m("number", value->next, 1,
		if (last->data.number < cur->data.number ) {
			result = 0;
			break;
		}
		else
			last = cur;);

	sdrl_destroy_value(value);
	return(ret);
}


/**
 * print(<value, ...)
 * Prints out each of the values in the list and returns the list.
 */
int prim_print(struct sdrl_machine *mach, struct sdrl_value *value)
{
	struct sdrl_value *cur;

	cur = value;
	while (cur) {
		switch (sdrl_base_type_m(cur->type)) {
			case SDRL_BT_NUMBER:
				printf("%f", cur->data.number);
				break;
			case SDRL_BT_STRING:
				printf("%s", cur->data.str);
				break;
			case SDRL_BT_POINTER:
				printf("%d", cur->data.ptr);
				break;
			default:
				break;
		}
		cur = cur->next;
	}
	mach->ret = value;
	return(0);
}

/*** Local Functions ***/

/**
 * Bind each element in the list values with its corresponding name in the list names.
 */
static int prim_set_list(struct sdrl_machine *mach, struct sdrl_value *names, struct sdrl_value *values)
{
	int ret = 0;
	struct sdrl_type *string;
	struct sdrl_value *cur_name, *cur_value, *tmp, *value;

	if (!(string = sdrl_find_type(mach->type_env, "string")))
		return(ERR_NOT_FOUND);
	cur_name = names;
	cur_value = values;
	while (cur_name) {
		if (cur_name->type != string)
			return(ERR_INVALID_TYPE);
		if (cur_value) {
			tmp = cur_value->next;
			value = (cur_name->next) ? sdrl_shift_value(&cur_value) : cur_value;
			cur_value = tmp;
		}
		else
			value = sdrl_make_value(string, (sdrl_data_t) "", 0, NULL);
		if (sdrl_rebind_value(mach->env, cur_name->data.str, value)) {
			if (sdrl_bind_value(mach->env, cur_name->data.str, value))
				return(ERR_OUT_OF_MEMORY);
		}
		cur_name = cur_name->next;
	}
	return(0);
}



