

OBJ = sdrl.o prims.o bindings.o io.o value.o

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

