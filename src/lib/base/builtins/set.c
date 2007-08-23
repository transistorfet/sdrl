/*
 * Builtin Name:	set.c
 * Module Requirements:	string type ; list type
 * Description:		Binding Assignment Expression
 */

#include <sdrl/sdrl.h>

static int sdrl_base_set_list(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

/**
 * Args:	<name>, <value>
 * Description:	Bind value to name.  Returns a duplicate of value.
 */
int sdrl_base_set(struct sdrl_machine *mach, struct sdrl_value *value)
{
	int i, ret = 0;
	struct sdrl_type *list;
	struct sdrl_value *name;

	if ((i = sdrl_value_count(value)) < 2)
		ret = SDRL_ERR_INVALID_PARAMS;
	else if (!(list = sdrl_find_binding(mach->type_env, "list")))
		ret = SDRL_ERR_NOT_FOUND;
	else {
		name = sdrl_shift_value(&value);
		if (name->type == list)
			ret = sdrl_base_set_list(mach, name->data.ptr, ((i == 2) && (value->type == list)) ? (struct sdrl_value *) value->data.ptr : value); 
		else
			ret = sdrl_base_set_list(mach, name, value); 
	}
	if (!ret)
		mach->ret = SDRL_MAKE_REFERENCE(value);
	return(ret);
}

/*** Local Functions ***/

/**
 * Bind each element in the list values with its corresponding name in the list names.
 */
static int sdrl_base_set_list(struct sdrl_machine *mach, struct sdrl_value *names, struct sdrl_value *values)
{
	struct sdrl_type *string;
	struct sdrl_value *cur_name, *cur_value, *tmp, *value;

	if (!(string = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERR_NOT_FOUND);
	cur_name = names;
	cur_value = values;
	while (cur_name) {
		if (cur_name->type != string)
			return(SDRL_ERR_INVALID_TYPE);
		if (cur_value) {
			tmp = cur_value->next;
			value = (cur_name->next) ? sdrl_shift_value(&cur_value) : cur_value;
			cur_value = tmp;
		}
		else
			value = sdrl_make_value(mach->heap, string, (sdrl_data_t) "", 0, NULL);
		if (sdrl_replace_binding(mach->env, cur_name->data.str, value)) {
			if (sdrl_add_binding(mach->env, cur_name->data.str, value))
				return(SDRL_ERR_OUT_OF_MEMORY);
		}
		cur_name = cur_name->next;
	}
	return(0);
}



