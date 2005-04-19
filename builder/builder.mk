#
# Name:		builder.mk
# Version:	0.1
# Description:	Builder make include file
#

.SUFFIXES: .o .c .h .cpp .s .asm

CC	= gcc
LD	= gcc
AS	= as
AR	= ar
RANLIB	= ranlib
MAKE	= make
DEPEND	= $(CC) -MM

ifndef CFLAGS
	CFLAGS = -g
endif
ifndef DIST
	DIST = $(TARGET)
endif

all: $(TARGET) $(LIB_TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

$(LIB_TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RANLIB) $(LIB_TARGET)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

%/:
	$(MAKE) $(MAKEFLAGS) -C $*

depend:
	$(DEPEND) $(SRCS) > .depend

dist:
	tar -cvf $(DIST).tar $(SRCS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) .depend

count:
	cat `ls *.[ch]` | wc


