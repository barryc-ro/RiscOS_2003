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
.cmhg.o:;   ${CMHG} -o $@ $<
.cmhg.h:;   ${CMHG} -d h.$* $<
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

o.main oz.main: h.modhdr

#
# RISC OS ROM build rules:
#
rom: ${TARGET} local_dirs
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

export_hdrs: h.replay hdr.replay
        ${CP} h.replay ${EXP_HDR}.h.replay ${CPFLAGS}
        ${CP} hdr.replay ${EXP_HDR1}.replay ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: o.replay
        ${CP} o.replay ${EXP_HDR}.o.replay ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

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
	${RM} h.modhdr
	${RM} h.replay
	${RM} hdr.replay
	${RM} o.replay
        @echo ${COMPONENT}: cleaned

local_dirs:
	@$(MKDIR) oz

resources:
        ${MKDIR} ${RESDIR}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.Messages ${CPFLAGS}
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
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

# Dynamic dependencies:
o.stream:	c.stream
o.stream:	C:h.swis
o.stream:	C:h.kernel
o.stream:	OS:h.osfind
o.stream:	OS:h.types
o.stream:	OS:h.os
o.stream:	OS:h.fileswitch
o.stream:	OS:h.osargs
o.stream:	OS:h.osgbpb
o.stream:	OS:h.macros
o.stream:	h.utils
o.stream:	h.stream
o.stream:	OS:h.osfile
o.stream:	h.msgs
o.stream:	C:h.replay
o.stream:	OS:h.osspriteop
o.stream:	h.stream
o.utils:	c.utils
o.utils:	h.msgs
o.utils:	OS:h.os
o.utils:	OS:h.types
o.utils:	h.stream
o.utils:	OS:h.osfile
o.utils:	OS:h.fileswitch
o.utils:	h.utils
o.veneers: s.veneers
o.veneers: os:hdr.macros
o.veneers: hdr:soundfile
o.veneers: OS:Hdr.Types
o.veneers: OS:Hdr.OS
o.veneers: OS:Hdr.OSGBPB
o.veneers: OS:Hdr.FileSwitch
o.soundplay:	c.soundplay
o.soundplay:	C:h.swis
o.soundplay:	C:h.kernel
o.soundplay:	C:h.kernel
o.soundplay:	OS:h.osmodule
o.soundplay:	OS:h.types
o.soundplay:	OS:h.os
o.soundplay:	C:h.replay
o.soundplay:	OS:h.osspriteop
o.soundplay:	C:h.soundfile
o.soundplay:	OS:h.osgbpb
o.soundplay:	OS:h.fileswitch
o.soundplay:	h.msgs
o.soundplay:	h.play
o.soundplay:	h.soundplay
o.soundplay:	h.utils
o.soundplay:	h.stream
o.soundplay:	OS:h.osfile
oz.resources: s.resources
oz.resources: hdr:ListOpts
oz.resources: hdr:Macros
oz.resources: hdr:System
oz.resources: Hdr:SWIs
oz.resources: Hdr:CPU.Generic26
oz.resources: Hdr:IO.GenericIO
oz.resources: Hdr:RISCOS
oz.resources: hdr:ModHand
oz.resources: hdr:MsgTrans
oz.resources: hdr:ResourceFS
oz.resources: hdr:Proc
oz.resources: hdr.CVars
oz.main:	c.main
oz.main:	C:h.swis
oz.main:	C:h.kernel
oz.main:	OS:h.os
oz.main:	OS:h.types
oz.main:	OS:h.resourcefs
oz.main:	OS:h.fileswitch
oz.main:	h.modhdr
oz.main:	h.main
oz.main:	C:h.replay
oz.main:	OS:h.osspriteop
oz.main:	h.msgs
oz.main:	h.resources
oz.main:	h.soundplay
oz.main:	C:h.soundfile
oz.main:	OS:h.osgbpb
oz.main:	h.play
oz.main:	h.utils
oz.main:	h.stream
oz.main:	OS:h.osfile
