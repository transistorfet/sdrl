/*
 * Name:	value.c
 * Version:	0.1
 * Description:	Value Manager
 */


#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "types.h"


struct value *value_makevalue(uubyte type, struct value *val, struct value *next)
{
	struct value *tmp;

	if (!(tmp = (struct value *) malloc(sizeof(struct value))))
		return(NULL);
	tmp->type = type;
	tmp->binds = 0;
	tmp->size = sizeof(struct value);
	tmp->data.val = val;
	tmp->next = next;

	return(tmp);
}

struct value *value_makestring(uubyte type, char *str, struct value *next)
{
	int size;
	struct value *tmp;

	size = sizeof(struct value) + strlen(str) + 1;
	if (!(tmp = (struct value *) malloc(size)))
		return(NULL);
	tmp->type = type;
	tmp->binds = 0;
	tmp->size = size;
	tmp->data.str = (char *) ((usize_t) tmp + sizeof(struct value));
	strcpy(tmp->data.str, str);
	tmp->next = next;

	return(tmp);
}

struct value *value_makenumber(uubyte type, int num, struct value *next)
{
	struct value *tmp;

	if (!(tmp = (struct value *) malloc(sizeof(struct value))))
		return(NULL);
	tmp->type = type;
	tmp->binds = 0;
	tmp->size = sizeof(struct value);
	tmp->data.num = num;
	tmp->next = next;

	return(tmp);
}

struct value *duplicate_value(struct value *val)
{
	struct value *tmp, *prev, *head = NULL;

	while (val) {
		if (!(tmp = (struct value *) malloc(val->size)))
			return(NULL);
		memcpy(tmp, val, val->size);
		if (val->type == VT_VALUE)
			tmp->data.val = duplicate_value(val->data.val);
		if (!head)
			head = tmp;
		else
			prev->next = tmp;
		prev = tmp;
		val = val->next;
	}

	return(head);
}

int destroy_value(struct value *val)
{
	struct value *tmp;

	while (val) {
		tmp = val;
		val = val->next;
		if (tmp->type == VT_VALUE)
			destroy_value(tmp->data.val);
		free(tmp);
	}
	return(0);
}

struct value *value_gethead(struct value *val, struct value **tail)
{
	struct value *tmp;

	if (tail)
		*tail = NULL;
	if (!(val))
		return(NULL);
	if (tail)
		*tail = val->next;
	else
		destroy_value(val->next);

	val->next = NULL;
	if (val->type == VT_VALUE) {
		tmp = val->data.val;
		free(val);
		return(tmp);
	}
	return(val);
}

struct value *value_gettail(struct value *val)
{
	struct value *tmp;

	if (!(val))
		return(NULL);
	tmp = val->next;
	val->next = NULL;
	return(tmp);
}

struct value *value_destroyhead(struct value *val)
{
	struct value *tmp;

	if (!(val))
		return(NULL);
	tmp = val->next;
	val->next = NULL;
	destroy_value(val);
	return(tmp);
}

int value_count(struct value *val)
{
	int i = 0;

	while (val) {
		i++;
		val = val->next;
	}
	return(i);
}

char value_getescape(char ch)
{
	switch (ch) {
		case 'n':
			return('\n');
		case 't':
			return('\t');
		case 'r':
			return('\r');
		case '0':
			return('\0');
		default:
			return(ch);
	}
}

int value_isvalue(char ch)
{
	if (((ch >= 0x30) && (ch <= 0x39)) || (ch == '\"') || (ch == '\'') || (ch == '\\') || (ch == '{'))
		return(1);
	return(0);
}

int value_isseperator(char ch)
{
	if ((ch == ',') || (ch == '(') || (ch == ')') || (ch == '\0'))
		return(1);
	return(0);
}

int value_iswhitespace(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r'))
		return(1);
	return(0);
}

int value_atoi(char *str, int base)
{
	int i = -1, ret = 0;

	if (!str)
		return(0);

	while (str[++i] != '\0') {
		ret *= base;
		if ((str[i] = value_digit(str[i])) >= base)
			return(ERR_OUTOFBOUNDS);
		ret += str[i];
	}

	return(ret);
}

int value_digit(char ch)
{
	if ((ch >= 0x30) && (ch <= 0x39))
		return(ch - 0x30);
	else if ((ch >= 0x41) && (ch <= 0x5a))
		return(ch - 0x37);
	else if ((ch >= 0x61) && (ch <= 0x7a))
		return(ch - 0x57);
	return(0);
}
