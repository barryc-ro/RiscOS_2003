# Makefile for WatchAbort
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name    Description
# ----         ----    -----------
# 30-Mar-2001  BJGA    Created.

COMPONENT = WatchAbort
TARGET    = ${COMPONENT}
OBJS      = ModHdr.o Module.o Vector.o
LIBS      =
DIRS      = local_dirs
CFLAGS    = -ffah -zM

include Makefiles:StdTools
include Makefiles:ModuleLibs
include Makefiles:ModStdRule
include Makefiles:RAMCModule
include Makefiles:ROMCModule

Module.o: Module.c ModHdr.h
        ${CC} ${CFLAGS} -o $@ Module.c

local_dirs:
        ${MKDIR} o

clean:
        ${RM} h.ModHdr
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
