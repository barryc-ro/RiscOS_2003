#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
C++	= c++
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

DEFINES = ${DEBUG} ${THROWBACK}
COMMONFLAGS = -zM -c -Wp -ws -depend !Depend ${INCLUDES} ${DEFINES}

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${COMMONFLAGS} -Throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v 
WFLAGS  = ~c~v


#
# Libraries
#
CLIB       = CLIB:o.stubs CLIB:o.c++lib <Lib$Dir>.RISC_OSLib.o.risc_oslib
DLIBS  = <LIB$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
#
# Include files
#
INCLUDES =  -ICLIB:

# Program specific options:
#
COMPONENT = EngMenu
TARGET    = !EngMenu.!RunImage

OBJS     = o.main \
o.graphics	\
o.message	\
o.parser	\
o.menuhier	\
o.menu

#
# Rule patterns
#
.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete
dirs:
        ${MKDIR} o

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned
        
#
# Target
#
${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${CLIB} ${DLIBS}

#
# Dynamic dependencies:
o.main:	c++.main
o.main:	h.main
o.main:	CLIB:h.kernel
o.main:	CLIB:h.swis
o.main:	CLIB:h.swis
o.main:	h.menuhier
o.main:	h.menu
o.main:	CLIB:h.kernel
o.main:	CLIB:h.swis
o.main:	h.main
o.main:	h.message
o.main:	CLIB:h.kernel
o.main:	h.graphics
o.main:	h.message
o.main:	C:h.bbc
o.main:	C:h.os
o.graphics:	c++.graphics
o.graphics:	CLIB:h.kernel
o.graphics:	CLIB:h.swis
o.graphics:	C:h.os
o.graphics:	C:h.bbc
o.graphics:	h.main
o.graphics:	CLIB:h.kernel
o.graphics:	CLIB:h.swis
o.graphics:	h.graphics
o.graphics:	h.message
o.graphics:	CLIB:h.kernel
o.message:	c++.message
o.message:	h.message
o.message:	CLIB:h.kernel
o.message:	CLIB:h.kernel
o.message:	CLIB:h.swis
o.message:	<Lib$Dir>.DebugLib.h.DebugLib
o.menuhier:	c++.menuhier
o.menuhier:	h.menuhier
o.menuhier:	h.menu
o.menuhier:	CLIB:h.kernel
o.menuhier:	CLIB:h.swis
o.menuhier:	h.main
o.menuhier:	CLIB:h.kernel
o.menuhier:	CLIB:h.swis
o.menuhier:	h.message
o.menuhier:	CLIB:h.kernel
o.menuhier:	h.graphics
o.menuhier:	h.message
o.menu:	c++.menu
o.menu:	h.menu
o.menu:	CLIB:h.kernel
o.menu:	CLIB:h.swis
o.menu:	h.main
o.menu:	CLIB:h.kernel
o.menu:	CLIB:h.swis
o.menu:	h.message
o.menu:	CLIB:h.kernel
o.menu:	h.graphics
o.menu:	h.message
o.menu:	h.main
o.parser:	c++.parser
o.parser:	h.menuhier
o.parser:	h.menu
o.parser:	CLIB:h.kernel
o.parser:	CLIB:h.swis
o.parser:	h.main
o.parser:	CLIB:h.kernel
o.parser:	CLIB:h.swis
o.parser:	h.message
o.parser:	CLIB:h.kernel
o.parser:	h.graphics
o.parser:	h.message
o.parser:	CLIB:h.ctype
o.parser:	h.main
