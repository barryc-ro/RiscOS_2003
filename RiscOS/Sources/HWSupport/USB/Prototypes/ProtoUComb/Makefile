# Makefile for Prototype USB Combined keyboard/mouse driver

# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date          Name         Description
# ----          ----         -----------
# 21-Mar-01     SNB          Recreated

COMPONENT	= PrototypeUComb
TARGET		= ProtoUComb

DIRS		= o._dirs
RAM_OBJS	= o.modhead o.cmodule
ROM_OBJS	= ${RAM_OBJS}
RAM_LIBS	= ${DEBUGLIB} ${NET5LIBS} ${CALLXLIB} ${ASMUTILS}
ROM_LIBS        = ${RAM_LIBS}

CFLAGS          = -zM -zps1 -ffah
#CDEFINES = -DDEBUGLIB -DEMULATE
CDEFINES = ${OPTIONS}
CINCLUDES = -IC:USB -IOS: -IC:

include StdTools
include ModStdRule
include ModuleLibs
include RAMCModule
include ROMCModule

clean:
	${WIPE} aof ${WFLAGS}
	${WIPE} o ${WFLAGS}
	${WIPE} h ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} linked ${WFLAGS}
        @${ECHO} ${COMPONENT}: cleaned

${DIRS}:
	${MKDIR} o
	${MKDIR} h
	${TOUCH} $@

cmodule.o:	modhead.h

# Dynamic dependencies:
