/*
 * Name:	value.h
 * Version:	0.1
 * Description:	Value Manager Header
 */


#ifndef VALUE_H
#define VALUE_H

#include "types.h"

#define VT_ERROR	0x00
#define VT_NUMBER	0x01
#define VT_FUNCPTR	0x02
#define VT_STRING	0x04
#define VT_SYMBOL	0x05
#define VT_CODE		0x06
#define VT_VALUE	0x08
#define VT_ENVIRONMENT	0x09
#define VT_USER		0x0c

struct sdrl_value {
	uubyte type;
	usbyte binds;
	int size;
	union {
		int num;
		char *str;
		struct sdrl_value *val;
	} data;
	struct sdrl_value *next;
};

struct sdrl_value *value_makevalue(uubyte, struct sdrl_value *, struct sdrl_value *);
struct sdrl_value *value_makestring(uubyte, char *, struct sdrl_value *);
struct sdrl_value *value_makenumber(uubyte, int, struct sdrl_value *);
struct sdrl_value *duplicate_value(struct sdrl_value *);
int destroy_value(struct sdrl_value *);

struct sdrl_value *value_gethead(struct sdrl_value *, struct sdrl_value **);
struct sdrl_value *value_gettail(struct sdrl_value *);
struct sdrl_value *value_destroyhead(struct sdrl_value *);

int value_count(struct sdrl_value *);
char value_getescape(char);

int value_isvalue(char);
int value_isseperator(char);
int value_iswhitespace(char);
int value_atoi(char *, int);
int value_digit(char);

#endif

