

SDRL = ../..

TARGET = sdrl
OBJS = main.o $(SDRL)/lib/libsdrl.a $(SDRL)/lib/libbase.a $(SDRL)/lib/libsdrl.a

#LDFLAGS = -L$(SDRL)/lib -lsdrl -lbase
CFLAGS = -g -I$(SDRL)/include -L$(SDRL)/lib

include $(SDRL)/builder/builder.mk

