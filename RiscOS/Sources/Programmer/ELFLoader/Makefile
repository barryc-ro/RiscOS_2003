# Makefile for ELF Loader
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Nov-00  SNB          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
include Makefiles:ModuleLibs

#
# Program specific options:
#
COMPONENT = ELFLoader
OBJS      = o.elfloader o.elfgo o.elf o.header
DIRS	  = o._dirs

CINCLUDES = -IC:CLX
CFLAGS    = -zm -zps1

#
# Generic options:
#
include Makefiles:StdTools
include Makefiles:RAMCModule
include Makefiles:ROMCModule
include Makefiles:ModStdRule

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${RM} h.header
        @echo ${COMPONENT}: cleaned

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

o.elf o.elfloader: h.header

# Dynamic dependencies:
