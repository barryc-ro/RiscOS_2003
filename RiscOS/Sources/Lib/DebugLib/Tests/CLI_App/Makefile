CC 		= cc
LD 		= Link
ASM 		= ObjAsm
CMHG	 	= CMHG
LB  		= LibFile
RM  		= Remove
WIPE 		= -Wipe
MKDIR 		= cdir

LIBDIR 		= <Lib$Dir>
CFLAGS 		= -c -depend !Depend -IC: -fah ${THROWBACK}
CPPFLAGS 	= -c -depend !Depend -IC: ${THROWBACK}
LDFLAGS		= -aif -c++
ASMFLAGS 	= -NoCache -depend !Depend
CMHGFLAGS 	= -depend !Depend ${THROWBACK}
WIPEFLAGS 	= ~CFR~V

DEBUGLIB 	= ${LIBDIR}.Debuglib.o.debuglib

DFLAGS 		= -DDEBUGLIB

TARGET 		= CLI_App

OBJS = \
 o.main \

LIBS = \
 C:o.stubs \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \

.SUFFIXES:	.o .c
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} -o $@ $<

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
