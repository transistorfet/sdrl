

OBJ = sdrl.o bindings.o expr.o value.o type.o parse.o input.o

CC = gcc
LINK = ld
LIBS =

NAME = sdrl.obj

$(NAME): $(OBJ)
	$(LINK) $(LIBS) -r -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) -c $<

clean:
	rm *.o

del:
	del *.o

