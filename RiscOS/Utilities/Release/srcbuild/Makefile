# Makefile for srcbuild
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name         Description
# ----        ----         -----------
# 19 Aug 1997 RWB 	   new srcbuild
# 05 Sep 1997 RWB          added install
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
COMPONENT = srcbuild
TARGET	  = ${COMPONENT}

OBJS      = srcbuild.o riscos.o build.o parse.o

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
o.parse:	c.parse
o.parse:	h.srcbuild
o.riscos:	c.riscos
o.riscos:	C:h.kernel
o.riscos:	C:h.swis
o.riscos:	h.srcbuild
o.srcbuild:	c.srcbuild
o.srcbuild:	h.srcbuild
o.srcbuild:	h.build
o.srcbuild:	h.parse
o.srcbuild:	h.riscos
o.build:	c.build
o.build:	h.srcbuild
o.build:	h.parse
o.build:	h.riscos
