# Makefile for soundfile
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
EXP_HDR = <Cexport$dir>
EXP_HDR1 = <export$dir>.^.Interface2

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
TLD     = tlink
RM      = remove
WIPE    = -wipe
DEFMOD  = DefMod

AFLAGS = -depend !Depend -Stamp -quit ${ADFLAGS}
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

.SUFFIXES: .oz

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
INCLUDES = -IOS:,C:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}

#
# Program specific options:
#
COMPONENT = soundfile
TARGET    = aof.${COMPONENT}
BASEOBJS  = o.modhdr o.msgs \
    o.stream o.utils o.fromieee o.iff o.objfile \
    o.open o.play o.readdata o.miscop o.veneers o.de_leave o.muldiv \
    o.aiff o.armadeus o.au o.audioworks o.eightsvx o.iota \
    o.psion o.stsample o.voc o.wave \
    o.e8to16 o.u8to16 o.u12to16 o.s16to16 o.u16to16 o.ur16to16

OBJS= ${BASEOBJS} o.resources o.main
ROMOBJS=${BASEOBJS} oz.resources oz.main

EXPORTS   = h.soundfile hdr.soundfile o.soundfile

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DROM=0
.c.oz:;     ${CC} ${CFLAGS} -o $@ $< -DROM=1
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {TRUE}"

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# component build rules
#

defs: o.soundfile h.soundfile hdr.soundfile

h.soundfile: Documents.def.soundfile
        ${DEFMOD} -h > h.soundfile < Documents.def.soundfile

hdr.soundfile: Documents.def.soundfile
        ${DEFMOD} -hdr > hdr.soundfile < Documents.def.soundfile

o.soundfile: Documents.def.soundfile
	${DEFMOD} -l -o DefMod < Documents.def.soundfile
	LibFile -c -o o.soundfile -via viafile
	${RM} viafile
	${WIPE} Defmod ~cr

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.soundfile ${EXP_HDR}.h.soundfile ${CPFLAGS}
        ${CP} hdr.soundfile ${EXP_HDR1}.soundfile ${CPFLAGS}
        ${CP} o.soundfile ${EXP_HDR}.o.soundfile ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} 
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS}
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} 
        ${LD} -o $@ -module ${OBJS} ${CLIB}

# Dynamic dependencies:
