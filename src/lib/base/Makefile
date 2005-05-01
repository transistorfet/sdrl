
SDRL = ../../..

LIB_TARGET = $(SDRL)/lib/libbase.a
OBJS =	common/load.o		\
	types/builtin.o		\
	types/expr.o		\
	types/form.o		\
	types/list.o		\
	types/env.o		\
	forms/code.o		\
	builtins/add.o		\
	builtins/and.o		\
	builtins/array.o	\
	builtins/defenv.o	\
	builtins/divide.o	\
	builtins/equals.o	\
	builtins/gthan.o	\
	builtins/gethan.o	\
	builtins/head.o		\
	builtins/if.o		\
	builtins/lethan.o	\
	builtins/list.o		\
	builtins/lookup.o	\
	builtins/lthan.o	\
	builtins/multiply.o	\
	builtins/nequals.o	\
	builtins/not.o		\
	builtins/null.o		\
	builtins/or.o		\
	builtins/print.o	\
	builtins/resolve.o	\
	builtins/set.o		\
	builtins/subtract.o	\
	builtins/tail.o		\
	builtins/unlist.o	\
	parsers/lispy.o		\
	parsers/parser.o	\
	utils/disperr.o		\
	utils/dispexpr.o	\
	utils/parsef.o		\
	utils/parses.o

CFLAGS = -g -I$(SDRL)/include -I.
ARFLAGS = rc

include $(SDRL)/builder/builder.mk


