

OBJ = machine.o bindings.o expr.o value.o type.o parse.o input.o

CC = gcc
CFLAGS = -g -I.
LINK = ld
LIBS =

NAME = sdrl.obj

$(NAME): $(OBJ)
	$(LINK) $(LIBS) -r -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm *.o

del:
	del *.o

