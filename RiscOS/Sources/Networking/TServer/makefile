#
# Make file for TServer, a telnet server for RISC OS
#

# Path macros
COMPONENT       = TServer
TARGET          = ${COMPONENT}
DIRS            = o._dirs
RAM_OBJS        = TServer.o modhead.o cstart.o
RAM_LIBS        = ${DEBUGLIB} ${NET5LIBS} ${WIMPLIB}

# Tool macros
CFLAGS          += -zM -wp -ffah ${OPTIONS}
CINCLUDES       = -Itbox:,c:tboxlibint,C:,TCPIPLibs:,Unixlib:
CMHGINCLUDES    = -IC:

# includes
include StdTools
include ModStdRule
include ModuleLibs
include RAMCMODULE
include ROMCModule

# targets
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

# Static dependancies

TServer.o:      modhead.h

# Dynamic dependencies:
