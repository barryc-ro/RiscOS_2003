# Project:  ROMpatch

COMPONENT = ROMpatchU
TARGET    = Util.${COMPONENT}

OBJS = o.debug o.rompatch o.asmutils o.module

LIBS = ${CLIB}

include Makefiles:AppLibs
include Makefiles:StdTools
include Makefiles:AppStdRule

# Final targets:

clean:
        ${WIPE} o    ${WFLAGS}
        ${WIPE} Util ${WFLAGS}

ram: ${TARGET} dirs
        @echo ${TARGET}: utility built (disc).

dirs:
        ${MKDIR} o
        ${MKDIR} Util

install: ram
        ${MKDIR} ${INSTDIR}
        Access ${INSTDIR} RW/R
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        Access ${INSTDIR}.${COMPONENT} RW/R
        @echo ${COMPONENT}: module installed (disc).

# User-editable dependencies:

${TARGET}: ${OBJS} ${LIBS}
        ${LD} ${OBJS} ${LIBS} -o ${TARGET}
        @echo ${COMPONENT}: built (disc).

# Static dependencies:

# Dynamic dependencies:
