# Project:  ROMpatch

TARGET    = Install.ROMPatch.ROMpatchU

OBJS = o.debug o.rompatch o.asmutils o.module

LIBS = ${CLIB}
       
include Makefiles:AppLibs
include Makefiles:StdTools
include Makefiles:AppStdRule

# Final targets:

ROMpatch: ${TARGET}
        ${ECHO} ${TARGET}: ROMpatch utility built

clean:
        ${XWIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${STRIPDEPEND}

# User-editable dependencies:

${TARGET}: ${OBJS} ${LIBS}
        ${LD} ${OBJS} ${LIBS} -o ${TARGET}

# Static dependencies:

# Dynamic dependencies:
