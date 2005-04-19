
SDRL = ../..

LIB_TARGET = $(SDRL)/lib/libsdrl.a
OBJS = machine.o events.o bindings.o expr.o value.o type.o parse.o input.o heap.o

CFLAGS = -g -I$(SDRL)/include -I.
ARFLAGS = rcu

include $(SDRL)/builder/builder.mk

