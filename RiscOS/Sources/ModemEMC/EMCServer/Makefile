# Makefile for ModemEMC
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28-May-96  RWB 	  New for !ModemEMC
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
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = -DDEBUG
DFLAGS  = 
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
COMPONENT   = ModemEMC
APPLICATION = !RunImage

OBJS      =	\
o.main		\
o.support
  
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
${APPLICATION}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.constants
o.main:	h.support
o.main:	C:h.kernel
o.main:	h.constants
o.main:	h.serdevvars
