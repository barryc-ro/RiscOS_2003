# Makefile for Library
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

#
# Include files
#
INCLUDES = -IC:

# Program specific options:
#
COMPONENT   = serial_ven

OBJS      =	\
o.serial_ven
  
#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${COMPONENT}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${COMPONENT}: ${OBJS}
        ${CP} h.${COMPONENT} <CExport$Dir>.h.${COMPONENT} ${CPFLAGS}
        ${CP} o.${COMPONENT} <CExport$Dir>.o.${COMPONENT} ${CPFLAGS}

#
#
# Static dependencies:
#

#
# Dynamic dependencies:
o.serial_ven:	c.serial_ven
o.serial_ven:	C:h.kernel
o.serial_ven:	C:h.swis
o.serial_ven:	h.serial
