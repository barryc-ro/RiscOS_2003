# Makefile for PCMCIA Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Jul-94  AMcC         Created.
# 11-Oct-94  WT           Constituent files changed

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
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -zM -ff
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
COMPONENT = PCMCIA_Mgr
RELOC_MOD = rm.PCMCIA_Mgr
TARGET    = aof.PCMCIA_Mgr
EXPORTS   = ${EXP_HDR}.PCMCIA
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.PCMCIA.Manager

OBJS =\
 CDIrqVenr.o\
 Cis.o\
 Debug.o\
 Driver.o\
 Driver_s.o\
 Error.o\
 Errors.o\
 FSOp.o\
 Header.o\
 Init.o\
 Interrupts.o\
 Misc.o\
 PowerOp.o\
 Read.o\
 Read_s.o\
 Resources.o\
 Services.o\
 StarCmd.o\
 Swi.o\
 Variables.o\
 Write.o\
 Write_s.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.cmhg.o:;    ${CMHG} $< $@

#
# Build rules:
#
all: ${RELOC_MOD}
        @echo ${COMPONENT}: all built

rom: ${TARGET}
        @echo ${COMPONENT}: all built (rom version)

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.^.^.PCMCIA
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.Messages ${RESDIR}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

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

${RELOC_MOD}: ${OBJS} ${CLIB}
        ${LD} -o $@ -Module ${OBJS} ${CLIB}

${EXP_HDR}.PCMCIA: hdr.PCMCIA
        ${CP} hdr.PCMCIA $@ ${CPFLAGS}

# Dynamic dependencies:
