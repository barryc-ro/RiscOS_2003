# Makefile for STBState
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Sep-94  JRH          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

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

CFLAGS  = -c -zM -depend !Depend ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
ASFLAGS = -depend !Depend -Stamp -To $@ -From

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
# Program specific options:
#
COMPONENT = STBState
EXPORTS   = ${EXP_HDR}.${COMPONENT}
TARGET    = aof.STBState

OBJS      = o.Module o.STBState o.Reset

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $<

#
# build a relocatable module:
#
all: rm.STBState

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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned


#
# Exported interface headers
#
${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.STBState: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}


# Dynamic dependencies:
o.Reset: s.Reset
o.Reset: Hdr:ListOpts
o.Reset: Hdr:Macros
o.Reset: Hdr:System
o.Reset: Hdr:SWIs
o.Reset: Hdr:CPU.Generic26
o.Reset: Hdr:IO.GenericIO
o.Reset: Hdr:RISCOS
o.Reset: Hdr:ModHand
o.Reset: Hdr:Proc
o.Reset: Hdr:Machine.<Machine>
o.Reset: Hdr:CPU.ARM600
o.Reset: Hdr:IO.IOMDL
o.Reset: Hdr:IO.IOMD
o.Reset: Hdr:MEMM.ARM600
o.Reset: Hdr:Services
o.Reset: Hdr:PublicWS
o.Reset: Hdr:Sound
o.Module:	c.Module
o.Module:	C:h.kernel
o.Module:	C:h.swis
o.Module:	C:h.time
