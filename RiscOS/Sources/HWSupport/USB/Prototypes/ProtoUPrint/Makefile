COMPONENT	= ProtoUPrint
RAM_MODULE	= rm.ProtoUPrint
DIRS		= o._dummy
RAM_OBJS	= o.modhead o.cmodule o.bufman
RAM_LIBS	= 	\
c:o.stubs		\
c:tboxlibint.o.tboxlibm \
c:debuglib.o.debuglibzm	\
tcpiplibs:o.socklib5zm	\
tcpiplibs:o.inetlibzm	\
c:callx.o.callx		\
c:asmutils.o.asmutils

CFLAGS	.= -throwback -zm -zps1 -ffah ${COPTIONS}
CINCLUDES = -IOS:,C:

include makefiles:StdTools
include makefiles:ModStdRule
include makefiles:RAMModule

clean:
	${WIPE}	o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${RM}	h.modhead

o._dummy:
	${MKDIR} o
	${MKDIR} c
	${MKDIR} cmhg
	${MKDIR} h
	${TOUCH} o._dummy

cmodule.o:	modhead.h

# Dynamic dependencies:
