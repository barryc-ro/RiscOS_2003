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
RM      = remove
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
BASEOBJS  = o.modhdr o.msgs o.utils \
	o.filechange o.handlers o.interest

OBJS= ${BASEOBJS} o.resources o.main
ROMOBJS=${BASEOBJS} oz.resources oz.main

EXPORTS   = h.filewatch hdr.filewatch o.filewatch

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

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.filewatch ${EXP_HDR}.h.filewatch ${CPFLAGS}
        ${CP} hdr.filewatch ${EXP_HDR1}.filewatch ${CPFLAGS}
        ${CP} o.filewatch ${EXP_HDR}.o.filewatch ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
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

rm.${COMPONENT}: ${OBJS}
        ${TLD} -o $@ -module ${OBJS}

# Dynamic dependencies:
o.resources: s.resources
o.resources: hdr:ListOpts
o.resources: hdr:Macros
o.resources: hdr:System
o.resources: Hdr:SWIs
o.resources: Hdr:CPU.Generic26
o.resources: Hdr:IO.GenericIO
o.resources: Hdr:RISCOS
o.resources: hdr:ModHand
o.resources: hdr:MsgTrans
o.resources: hdr:ResourceFS
o.resources: hdr:Proc
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	OS:h.os
o.main:	OS:h.types
o.main:	OS:h.resourcefs
o.main:	h.modhdr
o.main:	h.main
o.main:	h.msgs
o.main:	h.resources
o.main:	h.utils
o.main:	h.fw
o.main:	h.filewatch
o.msgs:	c.msgs
o.msgs:	C:h.swis
o.msgs:	C:h.kernel
o.msgs:	h.filewatch
o.msgs:	OS:h.types
o.msgs:	OS:h.os
o.msgs:	h.msgs
o.utils:	c.utils
o.utils:	C:h.swis
o.utils:	C:h.kernel
o.utils:	h.msgs
o.utils:	OS:h.os
o.utils:	OS:h.types
o.utils:	h.utils
o.filechange:	c.filechange
o.filechange:	h.fw
o.filechange:	h.filewatch
o.filechange:	OS:h.types
o.filechange:	OS:h.os
o.filechange:	h.utils
o.handlers:	c.handlers
o.handlers:	OS:h.osfind
o.handlers:	OS:h.types
o.handlers:	OS:h.os
o.handlers:	OS:h.osfscontrol
o.handlers:	h.fw
o.handlers:	h.filewatch
o.handlers:	h.main
o.handlers:	h.utils
o.interest:	c.interest
o.interest:	C:h.swis
o.interest:	C:h.kernel
o.interest:	OS:h.osfscontrol
o.interest:	OS:h.types
o.interest:	OS:h.os
o.interest:	h.fw
o.interest:	h.filewatch
o.interest:	h.msgs
o.interest:	h.utils
o.interest:	h.main
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
oz.main:	h.modhdr
oz.main:	h.main
oz.main:	h.msgs
oz.main:	h.resources
oz.main:	h.utils
oz.main:	h.fw
oz.main:	h.filewatch
