# Project:   TestApp

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
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
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -IC:

COMPONENT = TestApp
TARGET    = ^.!TestApp

OBJS      =	\
main.o


#
# Rule patterns
#
.c++.o:;    ${C++} ${CFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB}


# Dynamic dependencies:
