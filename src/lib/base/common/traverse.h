/*
 * Header Name:	traverse.h
 * Version:	0.1
 * Description:	Traversal Macro Header
 */


#ifndef _SDRL_LIB_BASE_TRAVERSE_H
#define _SDRL_LIB_BASE_TRAVERSE_H


#define traverse_value_m(type_name, value, init, line)						\
	if (!(type = sdrl_find_binding(mach->type_env, type_name)))				\
		ret = ERR_NOT_FOUND;								\
	else {											\
		result = init;									\
		cur = value;									\
		while (cur) {									\
			if (cur->type != type) {						\
				ret = ERR_INVALID_TYPE;						\
				break;								\
			}									\
			line;									\
			cur = cur->next;							\
		}										\
		mach->ret = sdrl_make_value(mach->heap, type, (sdrl_data_t) result, 0, NULL);	\
	}

#endif


