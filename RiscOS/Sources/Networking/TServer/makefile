COMPONENT       = TServer
TARGET          = ${COMPONENT}
DIRS            = o._dirs
RAM_OBJS        = TServer.o modhead.o

RAM_LIBS        = ${DEBUGLIB} ${NET5LIBS} ${WIMPLIB}

CFLAGS          += -zM -wp -ffah ${OPTIONS}
CINCLUDES       = -Itbox:,c:tboxlibint,C:,TCPIPLibs:,Unixlib:
CMHGINCLUDES    = -IC:

include Makefiles:StdTools
include Makefiles:ModStdRule
include makefiles:ModuleLibs
include Makefiles:RAMCMODULE
include makefiles:ROMCModule

all: o._dirs

clean:
        ${WIPE} o ${WFLAGS} 
        ${RM} h.modhead
        ${WIPE} rm ${WFLAGS}

o._dirs:
        ${MKDIR} o
        ${MKDIR} h
        ${MKDIR} rm
        ${TOUCH} $@

TServer.o:      modhead.h

# Static dependancies

# Dynamic dependencies:
