# Makefile for SMURFS thrasher
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 12 May 1997  RWB 	    New for smurf

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
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = -DDEBUG
DFLAGS  =
#
# Libraries
#
CLIB       = CLIB:o.stubs
INETLIB    = TCPIPLibs:o.inetlib

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

# Program specific options:
#
COMPONENT   = smurf
APPLICATION = ${COMPONENT}

OBJS      =	\
o.main		\
o.error		\

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${APPLICATION} dirs
        @echo ${COMPONENT}: all complete

dirs:
        ${MKDIR} o

clean:
        ${WIPE} o ${WFLAGS}
	${RM} ${APPLICATION}
        @echo ${COMPONENT}: cleaned

#
#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${CLIB} ${INETLIB}
        ${LD} -o $@ ${OBJS} ${CLIB} ${INETLIB}
        Access ${APPLICATION} RW/R

#
# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	TCPIPLibs:h.inetlib
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:machine.h.endian
o.main:	TCPIPLibs:machine.h.ansi
o.main:	TCPIPLibs:machine.h.types
o.main:	TCPIPLibs:netinet.h.in
o.main:	TCPIPLibs:h.netdb
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	h.constants
o.main:	h.csfs
o.main:	h.error
o.main:	C:h.kernel
