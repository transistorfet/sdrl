

OBJ = main.o library.o sdrl/sdrl.obj

CC = gcc
LINK = gcc

INCLUDE = -Isdrl
NAME = sdrl

$(NAME): $(OBJ)
	$(LINK) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(INCLUDE) -c $<

sdrl/sdrl.obj:
	cd sdrl
	make
	cd ..

clean:
	rm *.o

del:
	del *.o

