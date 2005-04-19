
SDRL = ../../..

LIB_TARGET = $(SDRL)/lib/libbase.a
OBJS =	common/load.o		\
	types/builtin.o		\
	types/expr.o		\
	types/form.o		\
	types/list.o		\
	forms/code.o		\
	builtins/add.o		\
	builtins/and.o		\
	builtins/array.o	\
	builtins/divide.o	\
	builtins/equals.o	\
	builtins/gthan.o	\
	builtins/gethan.o	\
	builtins/head.o		\
	builtins/if.o		\
	builtins/lethan.o	\
	builtins/list.o		\
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
	parsers/parser.o

CFLAGS = -g -I$(SDRL)/src/core -I$(SDRL)/include -I.
ARFLAGS = rcu

include $(SDRL)/builder/builder.mk


