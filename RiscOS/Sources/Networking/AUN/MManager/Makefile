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
o.module: s.module
o.module: s.mmhdr
o.module: Hdr:ListOpts
o.module: Hdr:Macros
o.module: Hdr:System
o.module: Hdr:SWIs
o.module: Hdr:CPU.Generic26
o.module: Hdr:IO.GenericIO
o.module: Hdr:RISCOS
o.module: Hdr:ModHand
o.module: s.NoDebug
o.module: s.stathdr
o.module: s.splus
o.module: s.mm
o.newcopy: s.newcopy
o.newcopy: s.mmhdr
o.newcopy: Hdr:ListOpts
o.newcopy: Hdr:Macros
o.newcopy: Hdr:System
o.newcopy: Hdr:SWIs
o.newcopy: Hdr:CPU.Generic26
o.newcopy: Hdr:IO.GenericIO
o.newcopy: Hdr:RISCOS
o.newcopy: Hdr:ModHand
o.newcopy: s.NoDebug
o.newcopy: s.stathdr
o.newcopy: s.splus
o.copiers: s.copiers
o.copiers: s.mmhdr
o.copiers: Hdr:ListOpts
o.copiers: Hdr:Macros
o.copiers: Hdr:System
o.copiers: Hdr:SWIs
o.copiers: Hdr:CPU.Generic26
o.copiers: Hdr:IO.GenericIO
o.copiers: Hdr:RISCOS
o.copiers: Hdr:ModHand
o.copiers: s.NoDebug
o.copiers: s.stathdr
o.copiers: s.splus
o.splus: s.splus
o.asmstat: s.asmstat
o.asmstat: s.mmhdr
o.asmstat: Hdr:ListOpts
o.asmstat: Hdr:Macros
o.asmstat: Hdr:System
o.asmstat: Hdr:SWIs
o.asmstat: Hdr:CPU.Generic26
o.asmstat: Hdr:IO.GenericIO
o.asmstat: Hdr:RISCOS
o.asmstat: Hdr:ModHand
o.asmstat: s.NoDebug
o.asmstat: s.stathdr
o.asmstat: s.splus
