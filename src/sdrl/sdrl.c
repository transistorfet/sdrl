/*
 * Name:	sdrl.c
 * Version:	0.1
 * Description:	SDRL Interpreter
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

static int sdrl_returnvalue(struct sdrl_machine *, char *);
static char sdrl_getinput(struct sdrl_machine *, char *);

struct sdrl_machine *create_sdrl_machine(uubyte bitflags)
{
	struct sdrl_machine *mach;

	if (!(mach = (struct sdrl_machine *) malloc(sizeof(struct sdrl_machine))))
		return(NULL);
	mach->ret = NULL;
	mach->opr = NULL;
	mach->env = create_environment(NULL, 0);
	if (!(bitflags & SBF_NOPRIMATIVES))
		prim_initialize(mach->env);
	mach->input = create_input_stack(SDRL_INPUT_STACK);

	return(mach);
}

int destroy_sdrl_machine(struct sdrl_machine *mach)
{
	destroy_input_stack(mach->input);
	destroy_environment(mach->env);
	destroy_value(mach->ret);
	destroy_value(mach->opr);
	free(mach);
	return(0);
}

int sdrl_run(struct sdrl_machine *mach)
{
	int ret;

	while (!(ret = sdrl_step(mach))) ;
	if (ret == 1)
		return(0);
	return(ret);
}

int sdrl_evaluate(struct sdrl_machine *mach, char *code, uuword bitflags)
{
	int ret;

	io_stackstring(mach->input, code, (IBF_ENDOFINPUT | (SEBF_IOSTRING & bitflags)));
	while (!(ret = sdrl_step(mach))) ;
	if (ret == 1)
		return(0);
	return(ret);
}

struct value *sdrl_function_call(struct sdrl_machine *mach, char *name, struct value *opr)
{
	struct binding *bind;

	mach->opr = opr;
	if (bind = find_binding(mach->env, name))
		sdrl_function_call_value(mach, bind->val, 0);
	else {
		destroy_value(opr);
		mach->opr = NULL;
	}
	opr = mach->ret;
	mach->ret = NULL;
	return(opr);
}

int sdrl_loadfile(struct sdrl_machine *mach, char *file)
{
	return(io_loadfile(mach->input, file));
}

int sdrl_loadopr(struct sdrl_machine *mach, struct value *val)
{
	destroy_value(mach->opr);
	mach->opr = duplicate_value(val);
	return(0);
}


int sdrl_add_binding(struct sdrl_machine *mach, char *name, struct value *val)
{
	return(add_binding(mach->env, name, val));
}

int sdrl_remove_binding(struct sdrl_machine *mach, char *name)
{
	return(remove_binding(mach->env, name));
}

int sdrl_replace_binding(struct sdrl_machine *mach, char *name, struct value *val)
{
	return(replace_binding(mach->env, name, val));
}

struct binding *sdrl_find_binding(struct sdrl_machine *mach, char *name)
{
	return(find_binding(mach->env, name));
}


int sdrl_evaluate_value(struct sdrl_machine *mach, struct value *val, uuword bitflags)
{
	if (val->type == VT_CODE)
		return(sdrl_evaluate(mach, val->data.str, bitflags));
	mach->ret = duplicate_value(val);
	return(0);
}

int sdrl_function_call_value(struct sdrl_machine *mach, struct value *val, uuword bitflags)
{
	int ret;
	struct environment *parent;

	if (val->type == VT_FUNCPTR)
		return(((int (*)(struct sdrl_machine *)) val->data.str)(mach));

	if (val->type != VT_CODE) {
		mach->ret = val;
		return(0);
	}

	parent = mach->env;
	mach->env = create_environment(parent, EBF_PRIVATE);
	if (mach->opr)
		add_binding(mach->env, "$", mach->opr);
	else
		add_binding(mach->env, "$", value_makenumber(VT_NUMBER, 0, NULL));
	mach->opr = NULL;
	ret = sdrl_evaluate(mach, val->data.str, bitflags);
	remove_binding(mach->env, "$");
	destroy_environment(mach->env);
	mach->env = parent;

	return(ret);
}

int sdrl_changeflags(struct sdrl_machine *mach, int i, uubyte bitflags, uubyte set)
{
	return(io_changeflags(mach->input, i, bitflags, set));
}

int sdrl_step(struct sdrl_machine *mach)
{
	char sep;
	int ret = 0;
	int i = 0, j = 1;
	struct binding *bind;
	char buffer[MAX_BUFFER];
	struct environment *parent;
	struct value *tmp, *head = NULL;

	if (!(sep = sdrl_getinput(mach, buffer)))
		return(1);

	destroy_value(mach->ret);
	mach->ret = NULL;
	if (value_isvalue(buffer[0])) {
		if (sep == ')')
			io_ungetchar(mach->input, 1);
		return(sdrl_returnvalue(mach, buffer));
	}
	else if (bind = find_binding(mach->env, buffer)) {
		if (sep == '(') {
			if ((bind->val->type != VT_FUNCPTR) && (bind->val->type != VT_CODE))
				return(ERR_SYNTAXERROR);

			while (((sep = io_getchar(mach->input)) != ')')) {
				io_ungetchar(mach->input, 1);
				if (ret = sdrl_step(mach))
					return(ret);
				if (mach->ret) {
					if (mach->ret->next)
						mach->ret = value_makevalue(VT_VALUE, mach->ret, NULL);
					if (!head) {
						head = mach->ret;
						tmp = mach->ret;
					}
					else {
						tmp->next = mach->ret;
						tmp = tmp->next;
					}
				}
				mach->ret = NULL;
			}
			mach->opr = head;

			if (bind->val->type == VT_FUNCPTR)
				ret = ((int (*)(struct sdrl_machine *)) bind->val->data.str)(mach);
			else if (bind->val->type == VT_CODE)
				ret = sdrl_function_call_value(mach, bind->val, 0);
			destroy_value(mach->opr);
			mach->opr = NULL;
		}
		else {
			if (sep == ')')
				io_ungetchar(mach->input, 1);
			mach->ret = duplicate_value(bind->val);
		}
		return(ret);
	}

	return(ERR_NOTFOUND);
}

static int sdrl_returnvalue(struct sdrl_machine *mach, char *buffer)
{
	int num;

	if (buffer[0] == '\"')
		mach->ret = value_makestring(VT_STRING, &buffer[1], NULL);
	else if (buffer[0] == '\'')
		mach->ret = value_makestring(VT_SYMBOL, &buffer[1], NULL);
	else if (buffer[0] == '{')
		mach->ret = value_makestring(VT_CODE, &buffer[1], NULL);
	else if (buffer[0] == '\\')
		mach->ret = value_makenumber(VT_NUMBER, value_getescape(buffer[1]), NULL);		
	else if ((buffer[0] >= 0x30) && (buffer[0] <= 0x39)) {
		if (buffer[0] == '0') {
			if (buffer[1] == 'x')
				num = value_atoi(&buffer[2], 16);
			else
				num = value_atoi(&buffer[1], 8);
		}
		else
			num = value_atoi(buffer, 10);
		if (num == ERR_OUTOFBOUNDS)
			return(ERR_OUTOFBOUNDS);
		mach->ret = value_makenumber(VT_NUMBER, num, NULL);
	}
	else
		return(ERR_INVALIDTYPE);
	return(0);		
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
