# Makefile for EraseCVS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06 May 1997  RWB 	  New for !EraseCVS
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

# Debug flags

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DEBUG} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = TBox:o.eventlib
TOOLBOXLIB = TBox:o.toolboxlib
WIMPLIB	   = TBox:o.wimplib
#
# Include files
#
INCLUDES = -ITbox:,C:

# Program specific options:
#
COMPONENT = EraseCVS
TARGET    = !RunImage

OBJS      =	\
o.main		\
o.cmdline	\

#
# Rule patterns
#
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
        ${RM} o
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Target
#
${TARGET}: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
        ${LD} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}

#
# Dynamic dependencies:
