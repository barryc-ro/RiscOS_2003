# Makefile for ToolAction
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
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK} -DNDEBUG
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
OSLIB     = OSLib:o.OSLib
#
# Include files
#
INCLUDES = -IOS:,C:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}

#
# Program specific options:
#
COMPONENT = ToolAction
TARGET    = aof.${COMPONENT}
BASEOBJS  = o.modhdr o.msgs o.utils \
	o.toolact

OBJS	= o.resources o.main o.string32 ${BASEOBJS} 
ROMOBJS	= oz.resources oz.main o.string32 ${BASEOBJS} 

EXPORTS   = h.ToolAction hdr.ToolAction o.ToolAction

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DROM=0
.c.oz:;     ${CC} ${CFLAGS} -o $@ $< -DROM=1
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
.cmhg.h:;   ${CMHG} -o o.$* $< $@
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM SETL {TRUE}"

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# component build rules
#

defs: h.ToolAction hdr.ToolAction o.ToolAction

h.ToolAction: Documents.def.ToolAction
        ${DEFMOD} -h > h.ToolAction < Documents.def.ToolAction

hdr.ToolAction: Documents.def.ToolAction
        ${DEFMOD} -hdr > hdr.ToolAction < Documents.def.ToolAction

o.ToolAction: Documents.def.ToolAction
	${DEFMOD} -l -o DefMod < Documents.def.ToolAction
	LibFile -c -o o.ToolAction -via viafile
	${RM} viafile
	${WIPE} Defmod ~cr

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.ToolAction ${EXP_HDR}.h.ToolAction ${CPFLAGS}
        ${CP} hdr.ToolAction ${EXP_HDR1}.ToolAction ${CPFLAGS}
        ${CP} o.ToolAction ${EXP_HDR}.o.ToolAction ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} h.toolaction
        ${RM} h.modhdr
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${OSLIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${OSLIB}
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${OSLIB}
        ${LD} -o $@ -module ${OBJS} ${OSLIB} ${CLIB}

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
o.resources: hdr.CVars
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	OS:h.os
o.main:	OS:h.types
o.main:	OS:h.resourcefs
o.main:	OS:h.fileswitch
o.main:	h.modhdr
o.main:	h.msgs
o.main:	h.resources
o.main:	h.utils
o.main:	h.toolact
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
oz.main:	h.msgs
oz.main:	h.resources
oz.main:	h.utils
oz.main:	h.toolact
o.string32:	c.string32
o.string32:	h.string32
o.msgs:	c.msgs
o.msgs:	C:h.swis
o.msgs:	C:h.kernel
o.msgs:	h.toolaction
o.msgs:	OS:h.types
o.msgs:	OS:h.os
o.msgs:	h.toolbox
o.msgs:	OS:h.wimp
o.msgs:	OS:h.osspriteop
o.msgs:	OS:h.font
o.msgs:	OS:h.messagetrans
o.msgs:	h.msgs
o.utils:	c.utils
o.utils:	C:h.swis
o.utils:	C:h.kernel
o.utils:	h.msgs
o.utils:	OS:h.os
o.utils:	OS:h.types
o.utils:	h.utils
o.toolact:	c.toolact
o.toolact:	C:h.swis
o.toolact:	C:h.kernel
o.toolact:	h.string32
o.toolact:	OS:h.macros
o.toolact:	OS:h.os
o.toolact:	OS:h.types
o.toolact:	h.toolbox
o.toolact:	OS:h.wimp
o.toolact:	OS:h.osspriteop
o.toolact:	OS:h.font
o.toolact:	OS:h.messagetrans
o.toolact:	h.gadget
o.toolact:	h.toolaction
o.toolact:	h.toolact
o.toolact:	h.msgs
o.toolact:	h.tbem
