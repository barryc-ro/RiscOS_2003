# Makefile for canonical
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name         Description
# ----        ----         -----------
# 18 Sep 1997 RWB          new
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES}
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

#
# Include files
#
INCLUDES = -IC:

# Program specific options:
#
COMPONENT = canonical
TARGET	  = ${COMPONENT}

OBJS      = canonical.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install: ${TARGET}
	${CP} ${TARGET} <Build$Dir>.Library.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: installed

#
# Target
#
${TARGET}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}
        Access ${TARGET} RW/R

#
# Dynamic dependencies:
o.canonical:	c.canonical
o.canonical:	C:h.kernel
o.canonical:	C:h.swis
