# Makefile for replayspt
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
WIPE    = wipe
DESTROY = -Destroy
IGNORE  = -IgnoreErr
DEFMOD  = DefMod

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} -DNDEBUG -DNO_ACCESS ${DFLAGS}
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
COMPONENT = ReplaySpt
TARGET    = aof.${COMPONENT}
BASEOBJS  =o.modhdr o.muldiv \
     o.access o.arhdr o.arinfo o.arsnd o.arsndcode o.play o.rpspt_s \
     o.msgs o.stream o.utils o.veneers o.soundplay

OBJS= ${BASEOBJS} o.resources o.main
ROMOBJS=${BASEOBJS} oz.resources oz.main

EXPORTS   = h.replay hdr.replay o.replay

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

defs: h.replay hdr.replay o.replay

h.replay: Documents.def.replay
        ${DEFMOD} -h > h.replay < Documents.def.replay

hdr.replay: Documents.def.replay
        ${DEFMOD} -hdr > hdr.replay < Documents.def.replay

o.replay: Documents.def.replay
	${DEFMOD} -l -o DefMod < Documents.def.replay
	LibFile -c -o o.replay -via viafile
	${RM} viafile
	${WIPE} Defmod ~cr

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.replay ${EXP_HDR}.h.replay ${CPFLAGS}
        ${CP} hdr.replay ${EXP_HDR1}.replay ${CPFLAGS}
        ${CP} o.replay ${EXP_HDR}.o.replay ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${DESTROY} ${OBJS}
	${DESTROY} ${ROMOBJS}
        ${IGNORE} ${WIPE}    linked.${COMPONENT} ${WFLAGS}
        ${IGNORE} ${WIPE}    map.${COMPONENT} ${WFLAGS}
        ${IGNORE} ${RM}      ${TARGET}
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

rm.${COMPONENT}: ${OBJS}
        ${TLD} -o $@ -module ${OBJS}

# Dynamic dependencies:
