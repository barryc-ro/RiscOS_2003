# Simple Makefile for a CLI app

CC 		= cc
LD 		= Link
ASM 		= ObjAsm
CMHG 		= CMHG
LB 		= LibFile
RM 		= Remove
WIPE 		= -Wipe
MKDIR 		= cdir

LIBDIR 		= <Lib$Dir>
CFLAGS 		= -c -depend !Depend -IC: -fah ${THROWBACK}
CPPFLAGS 	= -c -depend !Depend -IC: ${THROWBACK}
LDFLAGS		= -aif -c++
ASMFLAGS 	= -NoCache -depend !Depend ${THROWBACK}
CMHGFLAGS 	= -depend !Depend ${THROWBACK} -IC:
WIPEFLAGS 	= ~C~V

PDEBUG 		= ${LIBDIR}.PDebug.o.Lib_PL

TARGET 		= SimpleSend

OBJS = \
 o.send \

LIBS = \
 C:o.stubs \
 ${PDEBUG} \

.SUFFIXES:	.o .c
.c.o:;		${CC} ${CFLAGS} -o $@ $<

all:    ${TARGET}

clean:
	${RM}		${TARGET}
	${WIPE}	o	${WIPEFLAGS}


o.dirs:
	${MKDIR} o
	create o.dirs
	stamp  o.dirs

${TARGET}: ${OBJS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

# Dynamic dependencies:
