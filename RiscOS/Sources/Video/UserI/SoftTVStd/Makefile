# Makefile for SoftTVStandard module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 09-Jul-01      BJGA   Created
#

COMPONENT = SoftTVStandard
TARGET    = SoftTVStd
OBJS      = ModHdr.o Module.o
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
        ${MKDIR} h
        ${MKDIR} o

clean:
        ${RM} h.ModHdr
        ${WIPE} aof ${WFLAGS}
        ${WIPE} h ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
