

SDRL = ..
SDRL_CORE = $(SDRL)/core/sdrl.obj
SDRL_LIBS = $(SDRL)/lib/prims.o
OBJ = main.o $(SDRL_CORE) $(SDRL_LIBS)

CC = gcc
CFLAGS = -g -I$(SDRL)/core
LINK = gcc

NAME = sdrl

$(NAME): $(OBJ)
	$(LINK) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(SDRL_CORE):
	cd $(SDRL)/core;make

tidy:
	rm $(SDRL_CORE)

clean:
	rm *.o

del:
	del *.o

