# Makefile for SerialTest
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-May-96  RWB 	  Created
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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Libraries
#
CLIB       = CLIB:o.stubs
SERIALLIB  = <CExport$Dir>.o.serial_ven

#
# Include files
#
INCLUDES = -IC:

# Program specific options:
#
COMPONENT   = SerialTerm
APPLICATION = !RunImage

OBJS      =	\
o.main		\
o.console
  
#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${APPLICATION}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
	${RM} ${APPLICATION}
        @echo ${COMPONENT}: cleaned

#
#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${SERIALLIB}
        ${LD} -o $@ ${OBJS} ${CLIB} ${SERIALLIB}

#
# Dynamic dependencies:
o.terminal:	c.terminal
o.terminal:	C:h.serial
o.terminal:	h.console
o.console:	c.console
o.console:	C:h.kernel
o.console:	C:h.swis
o.main:	c.main
o.main:	C:h.serial_ven
o.main:	h.console
