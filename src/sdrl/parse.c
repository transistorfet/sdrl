/*
 * Name:	parse.c
 * Version:	0.1
 * Description:	Parser
 */


#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "sdrl.h"
#include "value.h"
#include "prims.h"
#include "bindings.h"
#include "types.h"

#define MAX_BUFFER		1024

struct sdrl_exp *parse_get_expression(struct sdrl_machine *mach)
{

}

struct sdrl_exp *parse_make_call(char *name, struct sdrl_exp *params, struct sdrl_exp *next)
{
	struct sdrl_exp *exp;

	if (!(exp = (struct sdrl_exp *) malloc(sizeof(struct sdrl_exp) + strlen(name))))
		return(NULL);

	exp->name = (char *) ((usize_t) exp + sizeof(struct sdrl_exp));
	strcpy(exp->name, name);
	exp->type = SDRL_EXP_CALL;
	exp->params = params;
	exp->next = next;
	return(exp);
}

struct sdrl_exp *parse_make_value(char *name, struct sdrl_exp *next)
{
	struct sdrl_exp *exp;

	if (!(exp = (struct sdrl_exp *) malloc(sizeof(struct sdrl_exp) + strlen(name))))
		return(NULL);

	exp->name = (char *) ((usize_t) exp + sizeof(struct sdrl_exp));
	strcpy(exp->name, name);
	exp->type = SDRL_EXP_VALUE;
	exp->params = NULL;
	exp->next = next;
	return(exp);
}

static char sdrl_getinput(struct sdrl_machine *mach, char *buffer)
{
	char sep;
	int i = 0, j = 1;

	while ((buffer[0] = io_getchar(mach->input)) == ',') ;

	if (buffer[0] == 0)
		return(0);

	if (buffer[0] == '\"') {
		i++;
		while ((buffer[i] = io_getrawchar(mach->input)) && (buffer[i] != '\"')) {
			if (buffer[i] == '\\')
				buffer[i] = value_getescape(io_getrawchar(mach->input));
			if (++i >= MAX_BUFFER)
				break;
		}
	}
	else if (buffer[0] == '{') {
		while ((j) && (++i < MAX_BUFFER) && (buffer[i] = io_getrawchar(mach->input))) {
			if (buffer[i] == '\\')
				buffer[i] = value_getescape(io_getchar(mach->input));
			else if (buffer[i] == '{')
				j++;
			else if (buffer[i] == '}')
				j--;
		}
	}
	else
		while ((++i < MAX_BUFFER) && (buffer[i] = io_getchar(mach->input)) && !(value_isseperator(buffer[i]))) ;
	sep = buffer[i];
	buffer[i] = '\0';

	if (!(sep)) {
		io_pushnull(mach->input);
		sep = ',';
	}
	return(sep);
}
