CC =		cc
LD = 		Link
ASM =	 	ObjAsm
CMHG =	 	CMHG
LB =		LibFile
RM =		Remove
WIPE =		-Wipe
MKDIR = 	cdir

LIBDIR =	<Lib$Dir>
CFLAGS =	-c -depend !Depend -IC: -DDEBUGLIB -fah -strict -throwback
CPPFLAGS = 	-c -depend !Depend -IC: -throwback
LDFLAGS	= 	-aif -c++
ASMFLAGS =	-throwback -NoCache -depend !Depend
CMHGFLAGS = 	-depend !Depend -throwback -IC:
WIPEFLAGS =	~C~V

DEBUGLIB =	${LIBDIR}.Debuglib.o.debuglib
NBLIB =		${LIBDIR}.NBLib.o.NBLib

TARGET = 	NBLibTest

OBJECTS	= \
 o.main \
 o.dirstst \
 o.files \
 o.internal \
 o.misc \
 o.stringtst \

LIBS = \
 C:o.stubs \
 ${NBLIB} \

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

${TARGET}: ${OBJECTS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJECTS} ${LIBS}
	
# Dynamic dependencies:
