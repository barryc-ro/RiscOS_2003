# Makefile for LED
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <CExport$Dir>.h

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} -D${MACHINE}=1 ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

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

DFLAGS   =

#
# Program specific options:
#
COMPONENT = LED
OM        = o${MACHINE}
TDIR      = aof.${MACHINE}
TARGET    = ${TDIR}.${COMPONENT}
RAMTDIR   = rm.${MACHINE}
RAMTARGET = ${RAMTDIR}.${COMPONENT}
LINKEDDIR = linked.${MACHINE}
MAPDIR    = map.${MACHINE}
OBJS      = ${OM}.header ${OM}.module ${OM}.veneer ${OM}.led
EXPORTS   = ${EXP_HDR}.${COMPONENT}

.SUFFIXES: .${OM}

#
# Rule patterns
#
.c.${OM}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${OM}:;   ${CMHG} -o $@ $<
.s.${OM}:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${RAMTARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} ${OM} ${WFLAGS}
        ${WIPE} ${LINKEDDIR} ${WFLAGS}
        ${WIPE} ${MAPDIR} ${WFLAGS}
        ${WIPE} ${TDIR} ${WFLAGS}
        ${WIPE} ${RAMTDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${OM}
        ${MKDIR} ${TDIR}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} ${LINKEDDIR}
        ${MKDIR} ${MAPDIR}
        ${LD} -o ${LINKEDDIR}.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > ${MAPDIR}.${COMPONENT}
        truncate ${MAPDIR}.${COMPONENT} ${LINKEDDIR}.${COMPONENT}
        ${CP} ${LINKEDDIR}.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB} ${OM}
        ${MKDIR} ${RAMTDIR}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: export.h.${COMPONENT}
        ${CP} export.h.${COMPONENT} $@ ${CPFLAGS}

${OM}:
        ${MKDIR} ${OM}

# Dynamic dependencies:
oOmega.module:	c.module
oOmega.module:	C:h.kernel
oOmega.module:	C:h.swis
oOmega.module:	h.module
oOmega.module:	h.led
oOmega.module:	h.error
oOmega.veneer: s.veneer
oOmega.veneer: Hdr:ListOpts
oOmega.veneer: Hdr:Macros
oOmega.veneer: Hdr:System
oOmega.veneer: Hdr:SWIs
oOmega.veneer: Hdr:CPU.Generic26
oOmega.veneer: Hdr:IO.GenericIO
oOmega.veneer: Hdr:RISCOS
oOmega.veneer: Hdr:ModHand
oOmega.led:	c.led
oOmega.led:	C:h.kernel
oOmega.led:	C:h.swis
oOmega.led:	h.module
oOmega.led:	h.led
oOmega.led:	h.veneer
oOmega.led:	h.iomd
oOmega.led:	h.error
oRCA.module:	c.module
oRCA.module:	C:h.kernel
oRCA.module:	C:h.swis
oRCA.module:	h.module
oRCA.module:	h.led
oRCA.module:	h.error
oRCA.veneer: s.veneer
oRCA.veneer: Hdr:ListOpts
oRCA.veneer: Hdr:Macros
oRCA.veneer: Hdr:System
oRCA.veneer: Hdr:SWIs
oRCA.veneer: Hdr:CPU.Generic26
oRCA.veneer: Hdr:IO.GenericIO
oRCA.veneer: Hdr:RISCOS
oRCA.veneer: Hdr:ModHand
oRCA.led:	c.led
oRCA.led:	C:h.kernel
oRCA.led:	C:h.swis
oRCA.led:	h.module
oRCA.led:	h.led
oRCA.led:	h.veneer
oRCA.led:	h.iomd
oRCA.led:	h.error
oBoca.module:	c.module
oBoca.module:	C:h.kernel
oBoca.module:	C:h.swis
oBoca.module:	h.module
oBoca.module:	h.led
oBoca.module:	h.error
oBoca.veneer: s.veneer
oBoca.veneer: Hdr:ListOpts
oBoca.veneer: Hdr:Macros
oBoca.veneer: Hdr:System
oBoca.veneer: Hdr:SWIs
oBoca.veneer: Hdr:CPU.Generic26
oBoca.veneer: Hdr:IO.GenericIO
oBoca.veneer: Hdr:RISCOS
oBoca.veneer: Hdr:ModHand
oBoca.led:	c.led
oBoca.led:	C:h.kernel
oBoca.led:	C:h.swis
oBoca.led:	h.module
oBoca.led:	h.led
oBoca.led:	h.veneer
oBoca.led:	h.iomd
oBoca.led:	h.error
