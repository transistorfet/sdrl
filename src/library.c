/*
 * Name:	library.c
 * Version:	0.1
 * Description:	Library Functions
 */

#include <stdio.h>
#include <string.h>

#include <sdrl.h>
#include <value.h>
#include <bindings.h>
#include <types.h>
#include "library.h"

#define MAX_STRING		256

static int library_printvalue(struct value *);

int library_initialize(struct sdrl_machine *mach)
{
	add_binding(mach->env, "print", value_makevalue(VT_FUNCPTR, (struct value *) library_print, NULL));
	add_binding(mach->env, "gets", value_makevalue(VT_FUNCPTR, (struct value *) library_gets, NULL));
	add_binding(mach->env, "split", value_makevalue(VT_FUNCPTR, (struct value *) library_split, NULL));
	add_binding(mach->env, "strcmp", value_makevalue(VT_FUNCPTR, (struct value *) library_strcmp, NULL));
	return(0);
}

int library_print(struct sdrl_machine *mach)
{
	struct value *cur;

	mach->ret = mach->opr;
	mach->opr = NULL;

	return(library_printvalue(mach->ret));
}

int library_gets(struct sdrl_machine *mach)
{
	char tmp[MAX_STRING];

	gets(tmp);
	mach->ret = value_makestring(VT_STRING, tmp, NULL);
	return(0);
}

int library_split(struct sdrl_machine *mach)
{

	return(0);
}

int library_strcmp(struct sdrl_machine *mach)
{
	int i;
	struct value *cur, *val;

	if (!(val = mach->opr) || !(val->type & VT_STRING))
		return(ERR_INVALIDPARAMS);
	cur = val->next;
	while (cur) {
		if (!(cur->type & VT_STRING))
			return(ERR_INVALIDTYPE);
		if ((i = strcmp(val->data.str, cur->data.str))) {
			mach->ret = value_makenumber(VT_NUMBER, i, NULL);
			return(0);
		}
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
	return(0);
}


static int library_printvalue(struct value *cur)
{
	while (cur) {
		if (cur->type & VT_VALUE) {
			printf("(");
			library_printvalue(cur->data.val);
			printf(")");
		}
		else if (cur->type & VT_STRING)
			printf("%s", cur->data.str);
		else
			printf("%d", cur->data.num);
		cur = cur->next;
	}
	return(0);
}
