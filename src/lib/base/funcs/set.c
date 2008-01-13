/*
 * Function Name:	set.c
 * Module Requirements:	string type ; list type
 * Description:		Binding Assignment Expression
 */

#include <sdrl/sdrl.h>

static int sdrl_base_set_list(struct sdrl_machine *, struct sdrl_value *, struct sdrl_value *);

/**
 * Args:	<name>, <value>
 * Description:	Bind value to name.  Returns a duplicate of value.
 */
int sdrl_base_set(struct sdrl_machine *mach, struct sdrl_value *args)
{
	int i, ret = 0;
	struct sdrl_type *list;
	struct sdrl_value *name;

	if ((i = sdrl_value_count(args)) < 2)
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_ARGS, NULL));
	else if (!(list = sdrl_find_binding(mach->type_env, "list")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	else {
		name = sdrl_shift_value(&args);
		if (name->type == list)
			ret = sdrl_base_set_list(mach, SDRL_REFERENCE(name)->ref, ((i == 2) && (args->type == list)) ? SDRL_REFERENCE(args)->ref : args); 
		else
			ret = sdrl_base_set_list(mach, name, args); 
	}
	if (!ret)
		mach->ret = SDRL_MAKE_REFERENCE(args);
	return(ret);
}

/*** Local Functions ***/

/**
 * Bind each element in the list values with its corresponding name in the list names.
 */
static int sdrl_base_set_list(struct sdrl_machine *mach, struct sdrl_value *names, struct sdrl_value *values)
{
	struct sdrl_type *str_type;
	struct sdrl_value *cur_name, *cur_value, *tmp, *value;

	if (!(str_type = sdrl_find_binding(mach->type_env, "string")))
		return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_NOT_FOUND, NULL));
	cur_name = names;
	cur_value = values;
	while (cur_name) {
		if (cur_name->type != str_type)
			return(SDRL_ERROR(mach, SDRL_ES_HIGH, SDRL_ERR_INVALID_TYPE, NULL));
		if (cur_value) {
			tmp = cur_value->next;
			value = (cur_name->next) ? sdrl_shift_value(&cur_value) : cur_value;
			cur_value = tmp;
		}
		else
			value = sdrl_make_string(mach->heap, str_type, "", 0);
		if (sdrl_replace_binding(mach->env, SDRL_STRING(cur_name)->str, value)) {
			if (sdrl_add_binding(mach->env, SDRL_STRING(cur_name)->str, value))
				return(SDRL_ERROR(mach, SDRL_ES_FATAL, SDRL_ERR_OUT_OF_MEMORY, NULL));
		}
		cur_name = cur_name->next;
	}
	return(0);
}


