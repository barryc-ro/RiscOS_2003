# Makefile for filewatch
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
RM      = rm
WIPE    = -wipe
DEFMOD  = DefMod

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
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
#REMOTEDB  = <Lib$Dir>.debug.o.remotezm
REMOTEDB  =
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
COMPONENT = filewatch
TARGET    = aof.${COMPONENT}
CMHGNAME = modhdr
BASEOBJS  = o.${CMHGNAME} o.msgs o.utils o.filechange o.handlers o.interest

OBJS= ${BASEOBJS} o.resources o.main
ROMOBJS= ${BASEOBJS} oz.resources oz.main

EXPORTS   = h.filewatch hdr.filewatch o.filewatch

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DROM=0
.c.oz:;     ${CC} ${CFLAGS} -o $@ $< -DROM=1
.cmhg.o:;   ${CMHG} -p -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {TRUE}"

#
# build a relocatable module:
#
all: rm.${COMPONENT} local_dirs

#
# component build rules
#

defs: h.filewatch hdr.filewatch o.filewatch

h.filewatch: Documents.def.filewatch
        ${DEFMOD} -h > h.filewatch < Documents.def.filewatch

hdr.filewatch: Documents.def.filewatch
        ${DEFMOD} -hdr > hdr.filewatch < Documents.def.filewatch

o.filewatch: Documents.def.filewatch
	${DEFMOD} -l -o DefMod < Documents.def.filewatch
	LibFile -c -o o.filewatch -via viafile
	${RM} viafile
	${WIPE} Defmod ~cr

# CMHG header file gets generated at the same time as the object file.
h.${CMHGNAME}: o.${CMHGNAME}
	@

o.filechange o.handlers o.interest o.main oz.main: h.${CMHGNAME}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE} local_dirs 

export_hdrs: h.filewatch hdr.filewatch
        ${CP} h.filewatch ${EXP_HDR}.h.filewatch ${CPFLAGS}
        ${CP} hdr.filewatch ${EXP_HDR1}.filewatch ${CPFLAGS}
        @echo ${COMPONENT}: export complete (headers)

export_libs: o.filewatch
        ${CP} o.filewatch ${EXP_HDR}.o.filewatch ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT}
        @echo ${COMPONENT}: installed

clean:
        ${RM} ${OBJS}
        ${RM} ${ROMOBJS}
        ${WIPE} rm.${COMPONENT} ${WFLAGS}
        ${WIPE} aof.${COMPONENT} ${WFLAGS}
        ${WIPE} linked.${COMPONENT} ${WFLAGS}
        ${RM} h.filewatch
        ${RM} hdr.filewatch
        ${RM} ${TARGET}
        ${RM} h.${CMHGNAME}
        ${RM} aof
        @echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RESDIR}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:Messages  ${RESDIR}.Messages ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${REMOTEDB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${REMOTEDB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} ${REMOTEDB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${REMOTEDB}

# Dynamic dependencies:
