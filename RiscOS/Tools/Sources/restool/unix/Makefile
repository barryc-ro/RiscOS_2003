# Project:   restool

CC = cc
AS = as
LD = cc

CCflags = -c -DUNIX -I..
LDflags = -o $@

.SUFFIXES: .c .s .o

.c.o:;	$(CC) -DROM=0 -DDEBUG=0 $(CCflags) -o $@ $<

OBJS =  restool.o

all: restool

restool: $(OBJS)
	${LD} $(LDflags) $(OBJS)

clean:
	@-rm restool
	@-rm *.o
	echo Done clean

realclean: clean
	find . -type l -print | xargs rm
	echo Done realclean

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
