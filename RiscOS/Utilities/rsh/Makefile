# Makefile for RSH
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Jun-99  SNB          Created.
#

#
# Paths
#
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe
SQUEEZE = squeeze

CCFLAGS = -c -depend !Depend -Wp -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

TCPIPLIBS = TCPIPLibs:rpc.o.rpclib TCPIPLibs:o.unixlib\
 TCPIPLibs:o.socklib TCPIPLibs:o.inetlib

#
# Include files
#
INCLUDES  = -IC: -ITCPIPLibs:
DFLAGS    = -DCOMPAT_INET4

#
# Program specific options:
#
COMPONENT  = rsh
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS=	o.rsh

LIBS=	${TCPIPLIBS} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<

#
# build a the library:
#
all:	${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

o._dirs: 
	${MKDIR} o
	${TOUCH} $@
	
#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} o._dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
	${SQUEEZE} ${COMPONENT}

# Dynamic dependencies:
