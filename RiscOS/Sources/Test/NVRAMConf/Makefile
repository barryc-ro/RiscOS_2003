# Project:   NVRAMConf

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
TCPIPLib   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklib5zm
DebugLib   = <Lib$Dir>.DebugLib.o.DebugLibzm
CLIB       = CLIB:o.stubs <Lib$Dir>.c++lib.o.c++lib
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
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -IC:

COMPONENT = NVRAMConf
TARGET    = NVRAMConf

DCOMP     = NVRAMConfD
DTARGET   = NVRAMConfD

OBJS      =	\
o.main

dOBJS      =	\
od.main


#
# Rule patterns
#
.SUFFIXES:	.od .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.od:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.modHdr
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

debug: ${DTARGET} dirs
        @echo ${DCOMP}: debug complete

dirs:
        ${MKDIR} o
        ${MKDIR} od

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${DTARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB}

${DTARGET}: ${dOBJS}
        ${LD} ${Linkflags} ${dOBJS} ${COBJ} ${CLIB} ${DebugLib} ${TCPIPLIB}


# Dynamic dependencies:

o.main: c.main C:h.swis C:h.kernel C:h.kernel <Lib$Dir>.DebugLib.h.DebugLib
od.main: c.main C:h.swis C:h.kernel C:h.kernel <Lib$Dir>.DebugLib.h.DebugLib C:debug.h.remote
