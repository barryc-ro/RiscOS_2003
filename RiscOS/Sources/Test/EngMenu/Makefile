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

DEFINES = ${DEBUG} ${THROWBACK} ${McCabe}
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
CLIB   = CLIB:o.stubs CLIB:o.c++lib <Lib$Dir>.RISC_OSLib.o.risc_oslib TCPIPLibs:o.unixlib
DLIBS  = <LIB$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
#
# Include files
#
INCLUDES =  -ICLIB:,TCPIPLibs:

# Program specific options:
#
COMPONENT = EngMenu
TARGET    = !EngMenu.!RunImage
D_TARGET  = !EngMenu.!RunImaDB
I_TARGET  = !EngMenu.!RunImagI

OBJS     = 	\
o.main 		\
o.graphics	\
o.message	\
o.parser	\
o.menuhier	\
o.interface	\
o.menu

d_OBJS     = 	\
d_o.main 	\
d_o.graphics	\
d_o.message	\
d_o.parser	\
d_o.menuhier	\
d_o.interface	\
d_o.menu

OBJSI     =	\
i.main 		\
i.graphics	\
i.message	\
i.parser	\
i.menuhier	\
i.interface	\
i.menu

OBJSINST   =	\
io.instlib	\
io.main		\
io.graphics	\
io.message	\
io.parser	\
io.menuhier	\
io.interface	\
io.menu

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c++.d_o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} ${DFLAGS} -W -o $@ $<

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

d_all: ${D_TARGET} dirs
        @echo ${COMPONENT}: all complete
        
instrument: ${I_TARGET} dirs
	@echo ${COMPONENT}: instrument build complete
	
preprocess: ${OBJSI} dirs
	@echo ${COMPONENT}: preprocess build complete

dirs:
        ${MKDIR} o
        ${MKDIR} d_o
        ${MKDIR} i
        ${MKDIR} io
        ${MKDIR} inst

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} d_o ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${d_TARGET} ${WFLAGS}
        ${WIPE} ${I_TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned
        
#
# Target
#
${I_TARGET}: ${OBJSINST}
	${LD} ${Linkflags} ${OBJSINST} ${CLIB}

${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${CLIB}

${D_TARGET}: ${d_OBJS}
        ${LD} ${Linkflags} ${d_OBJS} ${CLIB} ${DLIBS}

#
# Dynamic dependencies:
d_o.main:	c++.main
d_o.main:	h.main
d_o.main:	CLIB:h.swis
d_o.main:	CLIB:h.kernel
d_o.main:	h.message
d_o.main:	CLIB:h.kernel
d_o.main:	h.interface
d_o.main:	h.menuhier
d_o.main:	h.menu
d_o.main:	CLIB:h.kernel
d_o.main:	CLIB:h.swis
d_o.main:	h.main
d_o.main:	h.message
d_o.main:	h.graphics
d_o.main:	h.message
d_o.graphics:	c++.graphics
d_o.graphics:	CLIB:h.kernel
d_o.graphics:	CLIB:h.swis
d_o.graphics:	C:h.os
d_o.graphics:	C:h.bbc
d_o.graphics:	h.main
d_o.graphics:	CLIB:h.swis
d_o.graphics:	h.message
d_o.graphics:	CLIB:h.kernel
d_o.graphics:	h.graphics
d_o.graphics:	h.message
d_o.message:	c++.message
d_o.message:	h.message
d_o.message:	CLIB:h.kernel
d_o.message:	CLIB:h.kernel
d_o.message:	CLIB:h.swis
d_o.message:	<Lib$Dir>.DebugLib.h.DebugLib
d_o.parser:	c++.parser
d_o.parser:	h.Parser
d_o.parser:	h.menu
d_o.parser:	CLIB:h.kernel
d_o.parser:	CLIB:h.swis
d_o.parser:	h.main
d_o.parser:	CLIB:h.swis
d_o.parser:	h.message
d_o.parser:	CLIB:h.kernel
d_o.parser:	h.message
d_o.parser:	h.graphics
d_o.parser:	h.message
d_o.parser:	CLIB:h.ctype
d_o.menuhier:	c++.menuhier
d_o.menuhier:	h.menuhier
d_o.menuhier:	h.menu
d_o.menuhier:	CLIB:h.kernel
d_o.menuhier:	CLIB:h.swis
d_o.menuhier:	h.main
d_o.menuhier:	CLIB:h.swis
d_o.menuhier:	h.message
d_o.menuhier:	CLIB:h.kernel
d_o.menuhier:	h.message
d_o.menuhier:	h.graphics
d_o.menuhier:	h.message
d_o.menuhier:	h.parser
d_o.menuhier:	h.menu
d_o.interface:	c++.interface
d_o.interface:	h.main
d_o.interface:	CLIB:h.swis
d_o.interface:	CLIB:h.kernel
d_o.interface:	h.message
d_o.interface:	CLIB:h.kernel
d_o.interface:	h.interface
d_o.interface:	TCPIPLibs:sys.h.dcistructs
d_o.menu:	c++.menu
d_o.menu:	h.menu
d_o.menu:	CLIB:h.kernel
d_o.menu:	CLIB:h.swis
d_o.menu:	h.main
d_o.menu:	CLIB:h.swis
d_o.menu:	h.message
d_o.menu:	CLIB:h.kernel
d_o.menu:	h.message
d_o.menu:	h.graphics
d_o.menu:	h.message
d_o.menu:	h.main
d_o.menu:	CLIB:h.ctype
o.main:	c++.main
o.main:	h.main
o.main:	CLIB:h.swis
o.main:	CLIB:h.kernel
o.main:	h.message
o.main:	CLIB:h.kernel
o.main:	h.interface
o.main:	h.menuhier
o.main:	h.menu
o.main:	CLIB:h.kernel
o.main:	CLIB:h.swis
o.main:	h.main
o.main:	h.message
o.main:	h.graphics
o.main:	h.message
o.graphics:	c++.graphics
o.graphics:	CLIB:h.kernel
o.graphics:	CLIB:h.swis
o.graphics:	C:h.os
o.graphics:	C:h.bbc
o.graphics:	h.main
o.graphics:	CLIB:h.swis
o.graphics:	h.message
o.graphics:	CLIB:h.kernel
o.graphics:	h.graphics
o.graphics:	h.message
o.message:	c++.message
o.message:	h.message
o.message:	CLIB:h.kernel
o.message:	CLIB:h.kernel
o.message:	CLIB:h.swis
o.parser:	c++.parser
o.parser:	h.Parser
o.parser:	h.menu
o.parser:	CLIB:h.kernel
o.parser:	CLIB:h.swis
o.parser:	h.main
o.parser:	CLIB:h.swis
o.parser:	h.message
o.parser:	CLIB:h.kernel
o.parser:	h.message
o.parser:	h.graphics
o.parser:	h.message
o.parser:	CLIB:h.ctype
o.menuhier:	c++.menuhier
o.menuhier:	h.menuhier
o.menuhier:	h.menu
o.menuhier:	CLIB:h.kernel
o.menuhier:	CLIB:h.swis
o.menuhier:	h.main
o.menuhier:	CLIB:h.swis
o.menuhier:	h.message
o.menuhier:	CLIB:h.kernel
o.menuhier:	h.message
o.menuhier:	h.graphics
o.menuhier:	h.message
o.menuhier:	h.parser
o.menuhier:	h.menu
o.interface:	c++.interface
o.interface:	h.main
o.interface:	CLIB:h.swis
o.interface:	CLIB:h.kernel
o.interface:	h.message
o.interface:	CLIB:h.kernel
o.interface:	h.interface
o.interface:	TCPIPLibs:sys.h.dcistructs
o.menu:	c++.menu
o.menu:	h.menu
o.menu:	CLIB:h.kernel
o.menu:	CLIB:h.swis
o.menu:	h.main
o.menu:	CLIB:h.swis
o.menu:	h.message
o.menu:	CLIB:h.kernel
o.menu:	h.message
o.menu:	h.graphics
o.menu:	h.message
o.menu:	h.main
o.menu:	CLIB:h.ctype
io.instlib:	inst.instlib
io.instlib:	TCPIPLibs:h.unistd
io.instlib:	TCPIPLibs:sys.h.cdefs
io.instlib:	TCPIPLibs:sys.h.types
io.instlib:	TCPIPLibs:sys.h.cdefs
io.instlib:	TCPIPLibs:machine.h.endian
io.instlib:	TCPIPLibs:machine.h.ansi
io.instlib:	TCPIPLibs:machine.h.types
io.instlib:	TCPIPLibs:sys.h.unistd
io.main:	inst.main
io.graphics:	inst.graphics
io.message:	inst.message
io.parser:	inst.parser
io.menuhier:	inst.menuhier
io.interface:	inst.interface
io.menu:	inst.menu
