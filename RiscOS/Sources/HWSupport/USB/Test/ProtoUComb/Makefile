COMPONENT	= ProtoUComb
TARGET		= ${COMPONENT}
DIRS		= o._dirs
RAM_OBJS	= o.modhead o.cmodule
ROM_OBJS	= ${RAM_OBJS}
RAM_LIBS	= ${DEBUGLIB} ${SOCK5LIB} ${INETLIB} c:callx.o.callx ${ASMUTILS}
ROM_LIBS        = ${RAM_LIBS}

CFLAGS	.= -zM -ffah
#CDEFINES = -DDEBUGLIB -DEMULATE
CINCLUDES = -IOS:,C:

include makefiles:StdTools
include makefiles:ModStdRule
include makefiles:ModuleLibs
include makefiles:RAMCModule
include makefiles:ROMCModule

clean:
	${WIPE} aof ${WFLAGS}
	${WIPE}	o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${RM}	h.modhead

o._dirs:
	${MKDIR} o
	${MKDIR} c
	${MKDIR} cmhg
	${MKDIR} h
	${TOUCH} o._dirs

cmodule.o:	modhead.h

# Dynamic dependencies:
