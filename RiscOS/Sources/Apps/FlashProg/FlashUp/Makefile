# Makefile for FlashUp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Jul-96  JRH          Created.
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

AFLAGS  = -depend !Depend -Stamp -quit
DFLAGS  = -DDEBUG -g -fn
CFLAGS  = -c -depend !Depend -zps1 ${INCLUDES} ${DFLAGS}
LDFLAGS = -map -list map.${COMPONENT} -symbols map.syms
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:


#
# Program specific options:
#
COMPONENT = FlashUp
TARGETSA  = ${COMPONENT}
OBJS      = FlashUp.o stuff.o
EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: app built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: app installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# App
#
${TARGETSA}: ${OBJS} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB}


# Dynamic dependencies:
o.upgrade: s.upgrade
o.upgrade: Hdr:ListOpts
o.upgrade: Hdr:Macros
o.upgrade: Hdr:System
o.upgrade: Hdr:SWIs
o.upgrade: Hdr:CPU.Generic26
o.upgrade: Hdr:IO.GenericIO
o.upgrade: Hdr:RISCOS
o.upgrade: Hdr:Debug
o.interrupts: s.interrupts
o.FlashUp:	c.FlashUp
o.FlashUp:	C:h.kernel
o.FlashUp:	C:h.swis
o.FlashUp:	h.flashup
o.FlashUp:	h.error
o.stuff: s.stuff
o.stuff: Hdr:ListOpts
o.stuff: Hdr:Macros
o.stuff: Hdr:System
o.stuff: Hdr:SWIs
o.stuff: Hdr:CPU.Generic26
o.stuff: Hdr:IO.GenericIO
o.stuff: Hdr:RISCOS
o.stuff: Hdr:Debug
o.stuff: Hdr:Machine.<Machine>
o.stuff: Hdr:IO.IOMDL
o.stuff: Hdr:MEMM.ARM600
