# Makefile for SharedSound
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When          Who     What
# ----          ---     ----
# 28-Jun-2001   BJGA    Created

COMPONENT = SSound
TARGET    = SSound
OBJS      = Gets.o

include Makefiles:StdTools
include Makefiles:StdRules
include Makefiles:ROMCModule
include Makefiles:RAMCModule

clean:
        ${RM} o.Gets
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
