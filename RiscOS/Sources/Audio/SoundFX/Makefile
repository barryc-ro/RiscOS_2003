# Makefile for SoundFX
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 14-Mar-97   AB    FX playback
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

LIBS     = C:o.replay

DFLAGS   =

#
# Program specific options:
#
COMPONENT = SoundFX
TARGET    = aof.SoundFX
OBJS      = o.CMHGSndFX o.SoundFX
RDIR      = Resources.<System>
EXPORTS   =
SDIR      = ${RDIR}.Sounds
SOUNDS    = \
            ${SDIR}.connect \
            ${SDIR}.disconnect \
            ${SDIR}.go~back \
            ${SDIR}.handshake \
            ${SDIR}.home \
            ${SDIR}.imminent~t \
            ${SDIR}.info \
            ${SDIR}.keyboard~u \
            ${SDIR}.no-op \
            ${SDIR}.OK_w_air \
            ${SDIR}.sound~off \
            ${SDIR}.sound~on

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESSOUDIR = <resource$dir>.Resources.${COMPONENT}.Sounds

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.SoundFX

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

resources: ${SOUNDS}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESSOUDIR}
        ${CP} ${RDIR}.EventMap          ${RESSOUDIR}.EventMap   ${CPFLAGS}
        ${CP} ${SDIR}.connect           ${RESSOUDIR}.connect    ${CPFLAGS}
        ${CP} ${SDIR}.disconnect        ${RESSOUDIR}.disconnect ${CPFLAGS}
        ${CP} ${SDIR}.go~back           ${RESSOUDIR}.go~back    ${CPFLAGS}
        ${CP} ${SDIR}.handshake         ${RESSOUDIR}.handshake  ${CPFLAGS}
        ${CP} ${SDIR}.home              ${RESSOUDIR}.home       ${CPFLAGS}
        ${CP} ${SDIR}.imminent~t        ${RESSOUDIR}.imminent~t ${CPFLAGS}
        ${CP} ${SDIR}.info              ${RESSOUDIR}.info       ${CPFLAGS}
        ${CP} ${SDIR}.keyboard~u        ${RESSOUDIR}.keyboard~u ${CPFLAGS}
        ${CP} ${SDIR}.no-op             ${RESSOUDIR}.no-op      ${CPFLAGS}
        ${CP} ${SDIR}.OK_w_air          ${RESSOUDIR}.OK_w_air   ${CPFLAGS}
        ${CP} ${SDIR}.sound~off         ${RESSOUDIR}.sound~off  ${CPFLAGS}
        ${CP} ${SDIR}.sound~on          ${RESSOUDIR}.sound~on   ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

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

rm.SoundFX: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.SoundFX:	c.SoundFX
o.SoundFX:	C:h.kernel
o.SoundFX:	C:h.swis
o.SoundFX:	C:h.nvram
