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
o.resourcesz: s.resources
o.resourcesz: hdr:ListOpts
o.resourcesz: hdr:Macros
o.resourcesz: hdr:system
o.resourcesz: Hdr:SWIs
o.resourcesz: Hdr:CPU.Generic26
o.resourcesz: Hdr:IO.GenericIO
o.resourcesz: Hdr:RISCOS
o.resourcesz: hdr:ModHand
o.resourcesz: hdr:ResourceFS
o.resourcesz: hdr:MsgTrans
o.resourcesz: hdr:Proc
o.msgs:	c.msgs
o.msgs:	C:h.swis
o.msgs:	C:h.kernel
o.msgs:	OS:h.messagetrans
o.msgs:	OS:h.types
o.msgs:	OS:h.os
o.msgs:	h.msgs
o.stream:	c.stream
o.stream:	C:h.swis
o.stream:	C:h.kernel
o.stream:	OS:h.macros
o.stream:	OS:h.osfind
o.stream:	OS:h.types
o.stream:	OS:h.os
o.stream:	OS:h.osargs
o.stream:	OS:h.osgbpb
o.stream:	h.utils
o.stream:	h.msgs
o.stream:	h.stream
o.stream:	OS:h.osfile
o.stream:	h.soundfile
o.utils:	c.utils
o.utils:	h.utils
o.fromieee:	c.fromieee
o.fromieee:	h.utils
o.iff:	c.iff
o.iff:	h.msgs
o.iff:	OS:h.os
o.iff:	OS:h.types
o.iff:	h.iffsound
o.iff:	h.stream
o.iff:	OS:h.osfile
o.iff:	h.soundfile
o.iff:	OS:h.osgbpb
o.iff:	h.sf
o.iff:	h.utils
o.objfile:	c.objfile
o.objfile:	h.msgs
o.objfile:	OS:h.os
o.objfile:	OS:h.types
o.objfile:	h.objfile
o.objfile:	h.stream
o.objfile:	OS:h.osfile
o.objfile:	h.soundfile
o.objfile:	OS:h.osgbpb
o.objfile:	h.sf
o.open:	c.open
o.open:	C:h.swis
o.open:	C:h.kernel
o.open:	h.msgs
o.open:	OS:h.os
o.open:	OS:h.types
o.open:	h.sf
o.open:	h.stream
o.open:	OS:h.osfile
o.open:	h.soundfile
o.open:	OS:h.osgbpb
o.open:	h.utils
o.open:	h.identify
o.play:	c.play
o.play:	h.sf
o.play:	h.stream
o.play:	OS:h.osfile
o.play:	OS:h.types
o.play:	OS:h.os
o.play:	h.soundfile
o.play:	OS:h.osgbpb
o.readdata:	c.readdata
o.readdata:	OS:h.macros
o.readdata:	h.msgs
o.readdata:	OS:h.os
o.readdata:	OS:h.types
o.readdata:	h.sf
o.readdata:	h.stream
o.readdata:	OS:h.osfile
o.readdata:	h.soundfile
o.readdata:	OS:h.osgbpb
o.miscop:	c.miscop
o.miscop:	OS:h.macros
o.miscop:	h.sf
o.miscop:	h.stream
o.miscop:	OS:h.osfile
o.miscop:	OS:h.types
o.miscop:	OS:h.os
o.miscop:	h.soundfile
o.miscop:	OS:h.osgbpb
o.veneers: s.veneers
o.veneers: os:hdr.macros
o.veneers: hdr.soundfile
o.veneers: OS:Hdr.Types
o.veneers: OS:Hdr.OS
o.veneers: OS:Hdr.OSGBPB
o.de_leave: s.de_leave
o.de_leave: hdr.soundfile
o.de_leave: OS:Hdr.Types
o.de_leave: OS:Hdr.OS
o.de_leave: OS:Hdr.OSGBPB
o.muldiv: s.muldiv
o.aiff:	c.aiff
o.aiff:	OS:h.macros
o.aiff:	h.iffsound
o.aiff:	h.stream
o.aiff:	OS:h.osfile
o.aiff:	OS:h.types
o.aiff:	OS:h.os
o.aiff:	h.soundfile
o.aiff:	OS:h.osgbpb
o.aiff:	h.utils
o.aiff:	h.identify
o.armadeus:	c.armadeus
o.armadeus:	h.identify
o.armadeus:	h.stream
o.armadeus:	OS:h.osfile
o.armadeus:	OS:h.types
o.armadeus:	OS:h.os
o.armadeus:	h.soundfile
o.armadeus:	OS:h.osgbpb
o.au:	c.au
o.au:	OS:h.macros
o.au:	h.msgs
o.au:	OS:h.os
o.au:	OS:h.types
o.au:	h.utils
o.au:	h.identify
o.au:	h.stream
o.au:	OS:h.osfile
o.au:	h.soundfile
o.au:	OS:h.osgbpb
o.audioworks:	c.audioworks
o.audioworks:	h.msgs
o.audioworks:	OS:h.os
o.audioworks:	OS:h.types
o.audioworks:	OS:h.macros
o.audioworks:	h.objfile
o.audioworks:	h.stream
o.audioworks:	OS:h.osfile
o.audioworks:	h.soundfile
o.audioworks:	OS:h.osgbpb
o.audioworks:	h.identify
o.eightsvx:	c.eightsvx
o.eightsvx:	OS:h.macros
o.eightsvx:	h.iffsound
o.eightsvx:	h.stream
o.eightsvx:	OS:h.osfile
o.eightsvx:	OS:h.types
o.eightsvx:	OS:h.os
o.eightsvx:	h.soundfile
o.eightsvx:	OS:h.osgbpb
o.eightsvx:	h.utils
o.eightsvx:	h.identify
o.iota:	c.iota
o.iota:	h.msgs
o.iota:	OS:h.os
o.iota:	OS:h.types
o.iota:	h.objfile
o.iota:	h.stream
o.iota:	OS:h.osfile
o.iota:	h.soundfile
o.iota:	OS:h.osgbpb
o.iota:	h.identify
o.psion:	c.psion
o.psion:	h.identify
o.psion:	h.stream
o.psion:	OS:h.osfile
o.psion:	OS:h.types
o.psion:	OS:h.os
o.psion:	h.soundfile
o.psion:	OS:h.osgbpb
o.psion:	h.utils
o.stsample:	c.stsample
o.stsample:	h.identify
o.stsample:	h.stream
o.stsample:	OS:h.osfile
o.stsample:	OS:h.types
o.stsample:	OS:h.os
o.stsample:	h.soundfile
o.stsample:	OS:h.osgbpb
o.voc:	c.voc
o.voc:	h.msgs
o.voc:	OS:h.os
o.voc:	OS:h.types
o.voc:	h.identify
o.voc:	h.stream
o.voc:	OS:h.osfile
o.voc:	h.soundfile
o.voc:	OS:h.osgbpb
o.wave:	c.wave
o.wave:	OS:h.macros
o.wave:	h.msgs
o.wave:	OS:h.os
o.wave:	OS:h.types
o.wave:	h.iffsound
o.wave:	h.stream
o.wave:	OS:h.osfile
o.wave:	h.soundfile
o.wave:	OS:h.osgbpb
o.wave:	h.identify
o.e8to16: s.e8to16
o.e8to16: hdr:ListOpts
o.e8to16: hdr:Macros
o.e8to16: hdr:System
o.e8to16: Hdr:SWIs
o.e8to16: Hdr:CPU.Generic26
o.e8to16: Hdr:IO.GenericIO
o.e8to16: Hdr:RISCOS
o.e8to16: hdr:Proc
o.u8to16: s.u8to16
o.u8to16: hdr:ListOpts
o.u8to16: hdr:Macros
o.u8to16: hdr:System
o.u8to16: Hdr:SWIs
o.u8to16: Hdr:CPU.Generic26
o.u8to16: Hdr:IO.GenericIO
o.u8to16: Hdr:RISCOS
o.u8to16: hdr:Proc
o.u12to16: s.u12to16
o.u12to16: hdr:ListOpts
o.u12to16: hdr:Macros
o.u12to16: hdr:System
o.u12to16: Hdr:SWIs
o.u12to16: Hdr:CPU.Generic26
o.u12to16: Hdr:IO.GenericIO
o.u12to16: Hdr:RISCOS
o.u12to16: hdr:Proc
o.s16to16: s.s16to16
o.s16to16: hdr:ListOpts
o.s16to16: hdr:Macros
o.s16to16: hdr:System
o.s16to16: Hdr:SWIs
o.s16to16: Hdr:CPU.Generic26
o.s16to16: Hdr:IO.GenericIO
o.s16to16: Hdr:RISCOS
o.s16to16: hdr:Proc
o.u16to16: s.u16to16
o.u16to16: hdr:ListOpts
o.u16to16: hdr:Macros
o.u16to16: hdr:System
o.u16to16: Hdr:SWIs
o.u16to16: Hdr:CPU.Generic26
o.u16to16: Hdr:IO.GenericIO
o.u16to16: Hdr:RISCOS
o.u16to16: hdr:Proc
o.ur16to16: s.ur16to16
o.ur16to16: hdr:ListOpts
o.ur16to16: hdr:Macros
o.ur16to16: hdr:System
o.ur16to16: Hdr:SWIs
o.ur16to16: Hdr:CPU.Generic26
o.ur16to16: Hdr:IO.GenericIO
o.ur16to16: Hdr:RISCOS
o.ur16to16: hdr:Proc
o.resources: s.resources
o.resources: hdr:ListOpts
o.resources: hdr:Macros
o.resources: hdr:system
o.resources: Hdr:SWIs
o.resources: Hdr:CPU.Generic26
o.resources: Hdr:IO.GenericIO
o.resources: Hdr:RISCOS
o.resources: hdr:ModHand
o.resources: hdr:ResourceFS
o.resources: hdr:MsgTrans
o.resources: hdr:Proc
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	OS:h.os
o.main:	OS:h.types
o.main:	OS:h.osmodule
o.main:	OS:h.resourcefs
o.main:	h.modhdr
o.main:	h.main
o.main:	h.resources
o.main:	h.msgs
o.main:	h.sf
o.main:	h.stream
o.main:	OS:h.osfile
o.main:	h.soundfile
o.main:	OS:h.osgbpb
oz.resources: s.resources
oz.resources: hdr:ListOpts
oz.resources: hdr:Macros
oz.resources: hdr:system
oz.resources: Hdr:SWIs
oz.resources: Hdr:CPU.Generic26
oz.resources: Hdr:IO.GenericIO
oz.resources: Hdr:RISCOS
oz.resources: hdr:ModHand
oz.resources: hdr:ResourceFS
oz.resources: hdr:MsgTrans
oz.resources: hdr:Proc
oz.main:	c.main
oz.main:	C:h.swis
oz.main:	C:h.kernel
oz.main:	OS:h.os
oz.main:	OS:h.types
oz.main:	OS:h.osmodule
oz.main:	OS:h.resourcefs
oz.main:	h.modhdr
oz.main:	h.main
oz.main:	h.resources
oz.main:	h.msgs
oz.main:	h.sf
oz.main:	h.stream
oz.main:	OS:h.osfile
oz.main:	h.soundfile
oz.main:	OS:h.osgbpb
