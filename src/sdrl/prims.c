/*
 * Name:	prims.c
 * Version:	0.1
 * Description:	Primative Functions
 */


#include "sdrl.h"
#include "value.h"
#include "prims.h"
#include "bindings.h"
#include "types.h"

#define MAX_STRING		512

static int prim_bind_value(struct environment *, struct value *, struct value *, int (*)(struct environment *, char *, struct value *));

int prim_initialize(struct environment *env)
{
	add_binding(env, "+", value_makevalue(VT_FUNCPTR, (struct value *) prim_add, NULL));
	add_binding(env, "-", value_makevalue(VT_FUNCPTR, (struct value *) prim_subtract, NULL));
	add_binding(env, "*", value_makevalue(VT_FUNCPTR, (struct value *) prim_multiply, NULL));
	add_binding(env, "/", value_makevalue(VT_FUNCPTR, (struct value *) prim_divide, NULL));
	add_binding(env, "&", value_makevalue(VT_FUNCPTR, (struct value *) prim_bitand, NULL));
	add_binding(env, "|", value_makevalue(VT_FUNCPTR, (struct value *) prim_bitor, NULL));
	add_binding(env, "!", value_makevalue(VT_FUNCPTR, (struct value *) prim_bitnot, NULL));
	add_binding(env, "&&", value_makevalue(VT_FUNCPTR, (struct value *) prim_logicaland, NULL));
	add_binding(env, "||", value_makevalue(VT_FUNCPTR, (struct value *) prim_logicalor, NULL));
	add_binding(env, "!!", value_makevalue(VT_FUNCPTR, (struct value *) prim_logicalnot, NULL));
	add_binding(env, "<", value_makevalue(VT_FUNCPTR, (struct value *) prim_lessthan, NULL));
	add_binding(env, "<=", value_makevalue(VT_FUNCPTR, (struct value *) prim_lessthanequal, NULL));
	add_binding(env, ">", value_makevalue(VT_FUNCPTR, (struct value *) prim_greaterthan, NULL));
	add_binding(env, ">=", value_makevalue(VT_FUNCPTR, (struct value *) prim_greaterthanequal, NULL));
	add_binding(env, "=", value_makevalue(VT_FUNCPTR, (struct value *) prim_equals, NULL));
	add_binding(env, "~", value_makevalue(VT_FUNCPTR, (struct value *) prim_stringappend, NULL));
	add_binding(env, "~@", value_makevalue(VT_FUNCPTR, (struct value *) prim_listappend, NULL));
	add_binding(env, ".", value_makevalue(VT_FUNCPTR, (struct value *) prim_value, NULL));
	add_binding(env, "@", value_makevalue(VT_FUNCPTR, (struct value *) prim_list, NULL));
	add_binding(env, "%", value_makevalue(VT_FUNCPTR, (struct value *) prim_begin, NULL));
	add_binding(env, ";", value_makevalue(VT_FUNCPTR, (struct value *) prim_evaluate, NULL));
	add_binding(env, ";*", value_makevalue(VT_FUNCPTR, (struct value *) prim_evaluaterepeat, NULL));
	add_binding(env, ".*", value_makevalue(VT_FUNCPTR, (struct value *) prim_stoprepeat, NULL));
	add_binding(env, "?", value_makevalue(VT_FUNCPTR, (struct value *) prim_conditional, NULL));
	add_binding(env, "?*", value_makevalue(VT_FUNCPTR, (struct value *) prim_multiconditional, NULL));
	add_binding(env, ":$", value_makevalue(VT_FUNCPTR, (struct value *) prim_defineenvironment, NULL));
	add_binding(env, ":", value_makevalue(VT_FUNCPTR, (struct value *) prim_define, NULL));
	add_binding(env, ":=", value_makevalue(VT_FUNCPTR, (struct value *) prim_definereplace, NULL));

	return(0);
}


int prim_add(struct sdrl_machine *mach)
{
	int i = 0;
	struct value *cur;

	cur = mach->opr;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i += cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_subtract(struct sdrl_machine *mach)
{
	int i;
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDPARAMS);
	i = mach->opr->data.num;
	cur = mach->opr->next;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i -= cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_multiply(struct sdrl_machine *mach)
{
	int i = 1;
	struct value *cur;

	if (!(mach->opr))
		i = 0;

	cur = mach->opr;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i *= cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_divide(struct sdrl_machine *mach)
{
	int i;
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDPARAMS);
	i = mach->opr->data.num;
	cur = mach->opr->next;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i /= cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_bitand(struct sdrl_machine *mach)
{
	int i = -1;
	struct value *cur;

	if (!(mach->opr))
		i = 0;

	cur = mach->opr;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i &= cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_bitor(struct sdrl_machine *mach)
{
	int i = 0;
	struct value *cur;

	cur = mach->opr;
	while (cur) {
		if (cur->type != VT_NUMBER)
			return(ERR_INVALIDPARAMS);
		i |= cur->data.num;
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_bitnot(struct sdrl_machine *mach)
{
	int i = 0;

	if (value_count(mach->opr) != 1)
		return(ERR_INVALIDPARAMS);
	i = ~mach->opr->data.num;
	mach->ret = value_makenumber(VT_NUMBER, i, NULL);
	return(0);
}

int prim_logicaland(struct sdrl_machine *mach)
{
	struct value *cur;

	cur = mach->opr;
	while (cur) {
		if (!(cur->data.num)) {
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
			return(0);
		}
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_logicalor(struct sdrl_machine *mach)
{
	struct value *cur;

	cur = mach->opr;
	while (cur) {
		if (cur->data.num) {
			mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
			return(0);
		}
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
	return(0);
}

int prim_logicalnot(struct sdrl_machine *mach)
{
	if (value_count(mach->opr) != 1)
		return(ERR_INVALIDPARAMS);
	if (mach->opr->data.num)
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
	else
		mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_lessthan(struct sdrl_machine *mach)
{
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDTYPE);
	cur = mach->opr;
	while (cur && cur->next) {
		if ((cur->next->type != VT_NUMBER))
			return(ERR_INVALIDTYPE);
		if (cur->data.num >= cur->next->data.num)
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_lessthanequal(struct sdrl_machine *mach)
{
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDTYPE);
	cur = mach->opr;
	while (cur && cur->next) {
		if ((cur->next->type != VT_NUMBER))
			return(ERR_INVALIDTYPE);
		if (cur->data.num > cur->next->data.num)
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_greaterthan(struct sdrl_machine *mach)
{
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDTYPE);
	cur = mach->opr;
	while (cur && cur->next) {
		if ((cur->next->type != VT_NUMBER))
			return(ERR_INVALIDTYPE);
		if (cur->data.num <= cur->next->data.num)
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_greaterthanequal(struct sdrl_machine *mach)
{
	struct value *cur;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDTYPE);
	cur = mach->opr;
	while (cur && cur->next) {
		if ((cur->next->type != VT_NUMBER))
			return(ERR_INVALIDTYPE);
		if (cur->data.num < cur->next->data.num)
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_equals(struct sdrl_machine *mach)
{
	struct value *cur, *val;

	if (!(mach->opr)) {
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
		return(0);
	}

	if (mach->opr->type != VT_NUMBER)
		return(ERR_INVALIDTYPE);
	val = mach->opr;
	cur = val->next;
	while (cur) {
		if ((cur->type != VT_NUMBER))
			return(ERR_INVALIDTYPE);
		if (val->data.num != cur->data.num) {
			mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
			return(0);
		}
		cur = cur->next;
	}
	mach->ret = value_makenumber(VT_NUMBER, 1, NULL);
	return(0);
}

int prim_stringappend(struct sdrl_machine *mach)
{
	int i = 0, j;
	uubyte type = VT_NUMBER;
	char buffer[MAX_STRING];

	if (mach->opr)
		type = mach->opr->type;
	while (mach->opr) {
		if (!(mach->opr->type  & VT_STRING))
			return(ERR_INVALIDPARAMS);
		j = 0;
		while ((mach->opr->data.str[j]) && (i < MAX_STRING))
			buffer[i++] = mach->opr->data.str[j++];
		mach->opr = value_destroyhead(mach->opr);
	}
	buffer[i] = '\0';
	mach->ret = value_makestring(type, buffer, NULL);
	return(0);
}

int prim_listappend(struct sdrl_machine *mach)
{
	struct value *list = NULL;

	mach->ret = value_gethead(mach->opr, &mach->opr);
	list = mach->ret;
	while (mach->opr) {
		while (list->next)
			list = list->next;
		list->next = value_gethead(mach->opr, &mach->opr);
	}
	return(0);
}

int prim_value(struct sdrl_machine *mach)
{
	int i = 0;
	struct binding *bind;
	struct value *val, *index;

	if (!(mach->opr) || (value_count(mach->opr) > 2))
		return(ERR_INVALIDPARAMS);

	val = value_gethead(mach->opr, &mach->opr);
	index = value_gethead(mach->opr, &mach->opr);

	if (val->type == VT_SYMBOL) {
		if (!(bind = find_binding(mach->env, val->data.str)) || !(bind->val)) {
			destroy_value(val);
			destroy_value(index);
			return(ERR_NOTFOUND);
		}
		destroy_value(val);
		val = duplicate_value(bind->val);
	}

	if ((val->next) && (index)) {
		for (;i < index->data.num;i++) {
			if (!(val = value_destroyhead(val))) {
				destroy_value(index);
				return(ERR_OUTOFBOUNDS);
			}
		}
	}
	else if (val->type == VT_ENVIRONMENT) {
		if (index->type != VT_SYMBOL) {
			destroy_value(val);
			destroy_value(index);
			return(ERR_INVALIDPARAMS);
		}
		else if (!(bind = find_binding((struct environment *) val->data.val, index->data.str))) {
			destroy_value(val);
			destroy_value(index);
			return(ERR_NOTFOUND);
		}
		destroy_value(val);
		val = duplicate_value(bind->val);
	}
	mach->ret = val;

	return(0);
}

int prim_list(struct sdrl_machine *mach)
{
	mach->ret = mach->opr;
	mach->opr = NULL;
	return(0);
}

int prim_begin(struct sdrl_machine *mach)
{
	struct value *cur;

	cur = mach->opr;
	while (cur->next)
		cur = value_destroyhead(cur);
	mach->opr = NULL;
	mach->ret = cur;
	return(0);
}

int prim_evaluate(struct sdrl_machine *mach)
{
	int ret;
	struct value *val;

	if (!(mach->opr))
		return(ERR_INVALIDPARAMS);
	val = value_gethead(mach->opr, &mach->opr);
	mach->opr = value_gethead(mach->opr, NULL);
	ret = sdrl_function_call_value(mach, val, 0);
	destroy_value(val);
	return(ret);
}

int prim_evaluaterepeat(struct sdrl_machine *mach)
{
	int ret;
	struct value *val;

	if (!(mach->opr))
		return(ERR_INVALIDPARAMS);
	val = value_gethead(mach->opr, &mach->opr);
	mach->opr = value_gethead(mach->opr, NULL);
	ret = sdrl_function_call_value(mach, val, SEBF_REPEAT);
	destroy_value(val);
	return(ret);
}

int prim_stoprepeat(struct sdrl_machine *mach)
{
	int i = 0;
	struct value *val;

	if ((value_count(mach->opr) > 1) || ((mach->opr) && (mach->opr->type != VT_NUMBER)))
		return(ERR_INVALIDPARAMS);
	if (mach->opr)
		i = mach->opr->data.num;
	sdrl_changeflags(mach, i, SEBF_REPEAT, 0);
	return(0);
}

int prim_conditional(struct sdrl_machine *mach)
{
	int ret = 0;
	struct value *val, *iftrue, *iffalse;

	if (value_count(mach->opr) != 3)
		return(ERR_INVALIDPARAMS);
	val = mach->opr;
	iftrue = value_gethead(value_gettail(val), &iffalse);
	if (val->data.num)
		ret = sdrl_evaluate_value(mach, iftrue, 0);
	else
		ret = sdrl_evaluate_value(mach, iffalse, 0);
	destroy_value(iftrue);
	destroy_value(iffalse);
	return(ret);
}

int prim_multiconditional(struct sdrl_machine *mach)
{
	int result;
	struct value *cur, *tail, *cond, *code;

	if (!(mach->opr))
		return(ERR_INVALIDPARAMS);

	cur = value_gethead(mach->opr, &tail);
	mach->opr = NULL;
	while (cur) {
		if (value_count(cur) != 2) {
			destroy_value(cur);
			destroy_value(tail);
			return(ERR_INVALIDPARAMS);
		}
		cond = value_gethead(cur, &code);
		result = sdrl_evaluate_value(mach, cond, 0);
		destroy_value(cond);
		if (result) {
			destroy_value(code);
			break;
		}
		if (mach->ret)
			result = mach->ret->data.num;
		else
			result = 0;
		destroy_value(mach->ret);
		mach->ret = NULL;
		if (result) {
			result = sdrl_evaluate_value(mach, code, 0);
			destroy_value(code);
			break;
		}
		destroy_value(code);
		cur = value_gethead(tail, &tail);
	}

	destroy_value(tail);
	if (!(mach->ret))
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
	return(result);
}

int prim_defineenvironment(struct sdrl_machine *mach)
{
	if ((value_count(mach->opr) != 1) || (mach->opr->type != VT_SYMBOL))
		return(ERR_INVALIDPARAMS);
	mach->ret = value_makevalue(VT_ENVIRONMENT, (struct value *) create_environment(NULL, 0), NULL);
	add_binding(mach->env, mach->opr->data.str, duplicate_value(mach->ret));
	return(0);
}

int prim_define(struct sdrl_machine *mach)
{
	int i;
	struct environment *env;
	struct value *head, *tail;

	if (!(mach->opr) || ((i = value_count(mach->opr)) > 3))
		return(ERR_INVALIDPARAMS);
	if ((mach->opr->type == VT_ENVIRONMENT))
		env = (struct environment *) value_gethead(mach->opr, &(mach->opr))->data.val;
	else if (i < 3)
		env = mach->env;
	else
		return(ERR_INVALIDPARAMS);
	if (mach->opr->next)
		mach->ret = duplicate_value(mach->opr->next);
	else
		mach->ret = value_makenumber(VT_NUMBER, 0, NULL);
	head = value_gethead(mach->opr, &tail);
	tail = value_gethead(tail, NULL);
	mach->opr = NULL;
	prim_bind_value(env, head, tail, add_binding);
	return(0);
}

int prim_definereplace(struct sdrl_machine *mach)
{
	int i;
	struct environment *env;
	struct value *head, *tail;

	if (!(mach->opr) || ((i = value_count(mach->opr)) > 3))
		return(ERR_INVALIDPARAMS);
	if ((mach->opr->type == VT_ENVIRONMENT))
		env = (struct environment *) value_gethead(mach->opr, &(mach->opr))->data.val;
	else if (i == 2)
		env = mach->env;
	else
		return(ERR_INVALIDPARAMS);
	mach->ret = duplicate_value(mach->opr->next);
	head = value_gethead(mach->opr, &tail);
	tail = value_gethead(tail, NULL);
	mach->opr = NULL;
	prim_bind_value(env, head, tail, replace_binding);
	return(0);
}


static int prim_bind_value(struct environment *env, struct value *sym, struct value *val, int (*func)(struct environment *, char *, struct value *))
{
	struct value *tmp, *head, *tail;

	while ((sym) && (val)) {
		if (sym->type == VT_SYMBOL) {
			if (sym->next)
				head = value_gethead(val, &val);
			else
				head = val;
			if (func(env, sym->data.str, head)) {
				destroy_value(sym);
				destroy_value(val);
				return(ERR_INUSE);
			}
			sym = value_destroyhead(sym);
		}
		else if (sym->type == VT_VALUE) {
			head = value_gethead(sym, &sym);
			tmp = value_gethead(val, &val);
			prim_bind_value(env, head, tmp, func);
		}
		else {
			destroy_value(sym);
			destroy_value(val);
			return(ERR_INVALIDPARAMS);
		}
	}

	if (sym) {
		while (sym) {
			if (sym->type == VT_SYMBOL) {
				if (func(env, sym->data.str, value_makevalue(VT_NUMBER, 0, NULL))) {
					destroy_value(sym);
					return(ERR_INUSE);
				}
				sym = value_destroyhead(sym);
			}
			else if (sym->type == VT_VALUE) {
				head = value_gethead(sym, &sym);
				prim_bind_value(env, head, NULL, func);
			}
			else {
				destroy_value(sym);
				return(ERR_INVALIDPARAMS);
			}
		}
	}

	return(0);	
}

