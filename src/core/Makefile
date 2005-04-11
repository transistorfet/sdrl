

OBJ = machine.o events.o bindings.o expr.o value.o type.o parse.o input.o heap.o

CC = gcc
CFLAGS = -g -I.
LINK = ld
AR = ar
ARFLAGS = rcu
RANLIB = ranlib
LIBS =

LIB_NAME = libsdrl.a

$(LIB_NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm *.o

del:
	del *.o

size:
	ls -l *.[ch] | awk '{print $$5;next}' | perl -e 'while(<>){$$sum += $$_} print "$$sum\n";'

count:
	cat `ls *.[ch]` | wc

