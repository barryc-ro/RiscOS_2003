COMPONENT	= ProtoUComb
RAM_MODULE	= rm.ProtoUComb
DIRS		= o._dummy
RAM_OBJS	= o.modhead o.cmodule
RAM_LIBS	= 	\
c:o.stubs		\
c:tboxlibint.o.tboxlibm \
c:debuglib.o.debuglibzm	\
tcpiplibs:o.socklib5zm	\
tcpiplibs:o.inetlibzm	\
c:callx.o.callx		\
c:asmutils.o.asmutils

CFLAGS	.= -throwback -zm -ffah
#CDEFINES = -DDEBUGLIB -DEMULATE
CINCLUDES = -IOS:,C:

include makefiles:StdTools
include makefiles:ModStdRule
include makefiles:RAMModule

clean:
	${WIPE}	o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${RM}	h.modhead

o._dirs:
	${MKDIR} o
	${MKDIR} c
	${MKDIR} cmhg
	${MKDIR} h
	${TOUCH} o._dirs

cmodule.o:	modhead.h

# Dynamic dependencies:
