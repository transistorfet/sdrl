/*
 * Name:	parse.h
 * Version:	0.1
 * Description:	Parser Header
 */


#ifndef PARSE_H
#define PARSE_H

#include "sdrl.h"
#include "types.h"

#define SDRL_EXP_CALL		0x01
#define SDRL_EXP_VALUE		0x03

struct sdrl_exp {
	char *name;
	int type;
	struct sdrl_exp *params;
	struct sdrl_exp *next;
};

struct sdrl_exp *parse_get_expression(struct sdrl_machine *);
struct sdrl_exp *parse_make_call(char *, struct sdrl_exp *, struct sdrl_exp *);
struct sdrl_exp *parse_make_value(char *, struct sdrl_exp *);

#endif

