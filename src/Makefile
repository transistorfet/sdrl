

OBJ = main.o sdrl/sdrl.obj

CC = gcc
CFLAGS = -g -Isdrl
LINK = gcc

NAME = isdrl

$(NAME): $(OBJ)
	$(LINK) -o $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sdrl/sdrl.obj:
	cd sdrl;make;cd ..

tidy:
	rm sdrl/sdrl.obj

clean:
	rm *.o

del:
	del *.o

