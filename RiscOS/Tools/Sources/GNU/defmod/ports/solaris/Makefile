#
# Makefile for defmod for UNIX.
#

.SUFFIXES:
.SUFFIXES: .o .c

CC      = gcc
CFLAGS  = -I. -DPACE_BUILD -DUNIX -DEXECUTE_ON_UNIX
LD      = gcc
LDFLAGS = 
YACC    = bison
YFLAGS  =

OBJS    = defmod.o lookup.o asmhelp.o chelp.o def.o cheader.o \
          objasm.o realloc.o hdr.o unix.o cstrong.o x.o

defmod: ${OBJS}
	${LD} ${LDFLAGS} -o $@ ${OBJS}

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

defmod.o: defmod.c

defmod.c: defmod.y
	${YACC} -y defmod.y -o defmod.c

clean:
	rm -f *.o defmod defmod.c

realclean:
	rm -f *.o defmod defmod.c
	find . -type l -exec rm -f {} \;

links:
	./mklinks
