# Makefile for UniqueID
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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
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

DFLAGS   =

#
# Program specific options:
#
COMPONENT = UniqueID
TDIR      = aof.${MACHINE}
TARGET    = ${TDIR}.${COMPONENT}
RAMTDIR   = rm.${MACHINE}
RAMTARGET = ${RAMTDIR}.${COMPONENT}
O         = o${MACHINE}
OBJS      = ${O}.header ${O}.module ${O}.uniqueid ${O}.idblock
EXPORTS   = ${EXP_HDR}.${COMPONENT}
LINKEDDIR = linked
LDIR      = ${LINKEDDIR}.${MACHINE}
MAPDIR    = map
MDIR      = ${MAPDIR}.${MACHINE}

.SUFFIXES: .${O}

#
# Rule patterns
#
.c.${O}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${O}:;   ${CMHG} -o $@ $<
.s.${O}:;      ${AS} ${AFLAGS} $< $@

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
        ${WIPE} ${O} ${WFLAGS}
        ${WIPE} ${LDIR} ${WFLAGS}
        ${WIPE} ${MDIR} ${WFLAGS}
        ${WIPE} ${TDIR} ${WFLAGS}
        ${WIPE} ${RAMTDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} ${LINKEDDIR}
        ${MKDIR} ${LDIR}
        ${MKDIR} ${MAPDIR}
        ${MKDIR} ${MDIR}
        ${LD} -o ${LDIR}.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > ${MDIR}.${COMPONENT}
        truncate ${MDIR}.${COMPONENT} ${LDIR}.${COMPONENT}
        ${CP} ${LDIR}.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB} dirs
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: export.h.${COMPONENT}
        ${CP} export.h.${COMPONENT} $@ ${CPFLAGS}

${O}.idblock: s.${MACHINE}.idblock
        ${AS} ${AFLAGS} s.${MACHINE}.idblock $@

dirs:
        ${MKDIR} ${O}
        ${MKDIR} ${TDIR}
        ${MKDIR} ${RAMTDIR}

# Dynamic dependencies:
oBoca.module:	c.module
oBoca.module:	C:h.kernel
oBoca.module:	C:h.swis
oBoca.module:	h.module
oBoca.module:	h.uniqueid
oBoca.module:	h.error
oBoca.idblock: s.Boca.idblock
oBoca.uniqueid:	c.uniqueid
oBoca.uniqueid:	C:h.kernel
oBoca.uniqueid:	C:h.swis
oBoca.uniqueid:	h.module
oBoca.uniqueid:	h.uniqueid
oBoca.uniqueid:	h.idblock
oBoca.uniqueid:	C:h.nvram
