# Project:   TestMod

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
TCPIPLib   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklib5zm
DebugLib   = <Lib$Dir>.DebugLib.o.DebugLibzm
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
TOOLBOXLIB = TBox:o.toolboxlib
EVENTLIB   = TBox:o.eventlib
WIMPLIB	   = TBox:o.wimplib

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

# Debug flags

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DEBUG} -Throwback
C++FLAGS  = -c -depend !Depend ${INCLUDES} ${DEBUG} -Throwback
Linkflags = -rmf -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -IC:

COMPONENT = TestMod
TARGET    = TestMod

OBJS      =	\
modHdr.o	\
main.o


#
# Rule patterns
#
.c++.o:;    ${C++} ${CFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.modHdr
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete
dirs:
        ${MKDIR} o
        ${MKDIR} h

h.modHdr:	cmhg.modHdr
		${CMHG} ${CMHGFLAGS} -o o.$* $? $@
		@stamp $@

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB} ${DebugLib} ${TCPIPLib}


# Dynamic dependencies:
