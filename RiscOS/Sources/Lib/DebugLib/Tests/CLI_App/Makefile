CC 		= ${MemChecKCC} cc
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
MEMCHECKLIB	= MemCheck:o.MC_M

DFLAGS 		= -DDEBUGLIB

TARGET 		= CLI_App
TARGETMC	= CLI_AppMC

OBJS = \
 o.main \

OBJSMC = \
 oMC.main \

LIBS = \
 C:o.stubs \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \

LIBSMC = \
 C:o.stubs \
 ${DEBUGLIB}MC \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.inetlib \
 ${MEMCHECKLIB} \

.SUFFIXES:	.o .oMC .c
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.oMC:;	${CC} ${CFLAGS} ${DFLAGS} -fn -o $@ $<

all:    ${TARGET}

memcheck:	${TARGETMC}

clean:
	${RM}		${TARGET}
	${RM}		${TARGETMC}
	${WIPE}	o	${WIPEFLAGS}
	${WIPE} oMC	${WIPEFLAGS}

o.dirs:
	${MKDIR} o
	${MKDIR} oMC
	create o.dirs
	stamp  o.dirs

${TARGET}: ${OBJS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

${TARGETMC}: ${OBJSMC} ${LIBSMC} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJSMC} ${LIBSMC}

# Dynamic dependencies:
