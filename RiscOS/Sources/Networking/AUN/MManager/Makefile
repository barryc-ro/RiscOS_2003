# Makefile for Mbuf Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 21-Nov-94  AMcC   Created
# 26-Mar-98  KJB    Source imported
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
AFLAGS  = -depend !Depend ${THROWBACK} -Stamp -quit
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

#
# Program specific options:
#
COMPONENT  = MManager
TARGET     = rm.${COMPONENT}
OBJS       = module.o newcopy.o copiers.o splus.o asmstat.o

#
# Rule patterns
#
.s.o:;      ${AS}   ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

all: ${TARGET}
        @echo ${COMPONENT}: standalone module built

${TARGET}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS}

# Dynamic dependencies:
