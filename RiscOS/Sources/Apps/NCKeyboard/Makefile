# Makefile for NCKeyboard
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
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${DFLAGS}
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
WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Include files
#
INCLUDES = -Itbox:,C:

LIBS     = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib
LIBSZ    = ${ROMCSTUBS} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAM = ${RLIB} ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib

DFLAGS   =

#
# Program specific options:
#
COMPONENT = NCKeyboard
APPNAME   = !NCKeybd
TARGET    = aof.NCKeyboard
APP       = abs.!RunImage
OBJS      = o.handlers o.main o.oskbutils o.windowops
OBJSZ     = oz.handlers oz.main oz.oskbutils oz.windowops oz.ModuleWrap
OBJSZRAM  = oz.handlers oz.main oz.oskbutils oz.windowops oz.ModuleWra1
RDIR      = Resources.<Machine>.${LOCALE}
EXPORTS   = 

.SUFFIXES: .oz

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APPNAME}

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zM -zps1 -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${APP}

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

ram:    rm.${COMPONENT}
	@echo ${COMPONENT}: soft-load module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.!BootROM ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} ${RDIR}.!RunROM ${RESAPP}.!Run ${CPFLAGS}
        ${CP} ${RDIR}.Messages ${RESAPP}.Messages ${CPFLAGS}
        ${CP} ${RDIR}.res ${RESAPP}.res ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Static dependencies
#
${APP}: ${OBJS} ${LIBS}
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

rm.${COMPONENT}: ${OBJSZRAM} ${LIBSZRAM}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}

oz.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} s.ModuleWrap $@

oz.ModuleWra1: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ}
        ${LD} -o $@ -aof ${OBJSZ} ${LIBSZ}

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

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
oz.ModuleWrap: s.ModuleWrap
oz.ModuleWrap: s.AppName
oz.ModuleWrap: Hdr:ListOpts
oz.ModuleWrap: Hdr:Macros
oz.ModuleWrap: Hdr:System
oz.ModuleWrap: Hdr:SWIs
oz.ModuleWrap: Hdr:CPU.Generic26
oz.ModuleWrap: Hdr:IO.GenericIO
oz.ModuleWrap: Hdr:RISCOS
oz.ModuleWrap: Hdr:ModHand
oz.ModuleWrap: Hdr:EnvNumbers
oz.ModuleWrap: Hdr:Proc
oz.ModuleWrap: Hdr:FSNumbers
oz.ModuleWrap: Hdr:NewErrors
oz.ModuleWrap: Hdr:Services
oz.ModuleWrap: Hdr:ResourceFS
oz.ModuleWrap: Hdr:MsgTrans
oz.ModuleWrap: Hdr:Variables
oz.ModuleWrap: Hdr:UpCall
oz.ModuleWrap: s.ResFiles
oz.handlers:	c.handlers
oz.handlers:	C:h.kernel
oz.handlers:	tbox:h.wimp
oz.handlers:	tbox:h.toolbox
oz.handlers:	tbox:h.wimplib
oz.handlers:	h.handlers
oz.handlers:	tbox:h.toolbox
oz.handlers:	tbox:h.wimp
oz.handlers:	h.oskbutils
oz.handlers:	h.NCKeybd
oz.handlers:	c.handlers
oz.handlers:	C:h.kernel
oz.handlers:	tbox:h.wimp
oz.handlers:	tbox:h.toolbox
oz.handlers:	tbox:h.wimplib
oz.handlers:	h.handlers
oz.handlers:	tbox:h.toolbox
oz.handlers:	tbox:h.wimp
oz.handlers:	h.oskbutils
oz.handlers:	h.NCKeybd
oz.main:	c.main
oz.main:	tbox:h.wimp
oz.main:	tbox:h.toolbox
oz.main:	C:h.kernel
oz.main:	tbox:h.event
oz.main:	tbox:h.wimplib
oz.main:	tbox:h.window
oz.main:	tbox:h.gadgets
oz.main:	h.NCKeybd
oz.main:	h.windowops
oz.main:	h.NCKeybd
oz.main:	h.oskbutils
oz.main:	h.NCKeybd
oz.main:	h.handlers
oz.main:	tbox:h.toolbox
oz.main:	tbox:h.wimp
oz.windowops:	c.windowops
oz.windowops:	C:h.kernel
oz.windowops:	C:h.swis
oz.windowops:	tbox:h.toolbox
oz.windowops:	tbox:h.wimp
oz.windowops:	tbox:h.wimp
oz.windowops:	tbox:h.wimplib
oz.windowops:	tbox:h.window
oz.windowops:	tbox:h.gadgets
oz.windowops:	h.NCKeybd
oz.windowops:	h.windowops
oz.windowops:	h.NCKeybd
o.handlers:	c.handlers
o.handlers:	C:h.kernel
o.handlers:	tbox:h.wimp
o.handlers:	tbox:h.toolbox
o.handlers:	tbox:h.wimplib
o.handlers:	h.handlers
o.handlers:	tbox:h.toolbox
o.handlers:	tbox:h.wimp
o.handlers:	h.oskbutils
o.handlers:	h.NCKeybd
o.main:	c.main
o.main:	tbox:h.wimp
o.main:	tbox:h.toolbox
o.main:	C:h.kernel
o.main:	tbox:h.event
o.main:	tbox:h.wimplib
o.main:	tbox:h.window
o.main:	tbox:h.gadgets
o.main:	h.NCKeybd
o.main:	h.windowops
o.main:	h.NCKeybd
o.main:	h.oskbutils
o.main:	h.NCKeybd
o.main:	h.handlers
o.main:	tbox:h.toolbox
o.main:	tbox:h.wimp
o.oskbutils:	c.oskbutils
o.oskbutils:	C:h.kernel
o.oskbutils:	tbox:h.wimplib
o.oskbutils:	tbox:h.wimp
o.oskbutils:	C:h.swis
o.oskbutils:	tbox:h.gadgets
o.oskbutils:	tbox:h.toolbox
o.oskbutils:	tbox:h.event
o.oskbutils:	tbox:h.window
o.oskbutils:	h.NCKeybd
o.oskbutils:	h.windowops
o.oskbutils:	h.NCKeybd
o.oskbutils:	h.oskbutils
o.oskbutils:	h.NCKeybd
o.windowops:	c.windowops
o.windowops:	C:h.kernel
o.windowops:	C:h.swis
o.windowops:	tbox:h.toolbox
o.windowops:	tbox:h.wimp
o.windowops:	tbox:h.wimp
o.windowops:	tbox:h.wimplib
o.windowops:	tbox:h.window
o.windowops:	tbox:h.gadgets
o.windowops:	h.NCKeybd
o.windowops:	h.windowops
o.windowops:	h.NCKeybd
oz.oskbutils:	c.oskbutils
oz.oskbutils:	C:h.kernel
oz.oskbutils:	tbox:h.wimplib
oz.oskbutils:	tbox:h.wimp
oz.oskbutils:	C:h.swis
oz.oskbutils:	tbox:h.gadgets
oz.oskbutils:	tbox:h.toolbox
oz.oskbutils:	tbox:h.event
oz.oskbutils:	tbox:h.window
oz.oskbutils:	h.NCKeybd
oz.oskbutils:	h.windowops
oz.oskbutils:	h.NCKeybd
oz.oskbutils:	h.oskbutils
oz.oskbutils:	h.NCKeybd
oz.oskbutils:	h.handlers
oz.oskbutils:	tbox:h.toolbox
oz.oskbutils:	tbox:h.wimp
oz.ModuleWra1: s.ModuleWrap
oz.ModuleWra1: s.AppName
oz.ModuleWra1: Hdr:ListOpts
oz.ModuleWra1: Hdr:Macros
oz.ModuleWra1: Hdr:System
oz.ModuleWra1: Hdr:SWIs
oz.ModuleWra1: Hdr:CPU.Generic26
oz.ModuleWra1: Hdr:IO.GenericIO
oz.ModuleWra1: Hdr:RISCOS
oz.ModuleWra1: Hdr:ModHand
oz.ModuleWra1: Hdr:EnvNumbers
oz.ModuleWra1: Hdr:Proc
oz.ModuleWra1: Hdr:FSNumbers
oz.ModuleWra1: Hdr:NewErrors
oz.ModuleWra1: Hdr:Services
oz.ModuleWra1: Hdr:ResourceFS
oz.ModuleWra1: Hdr:MsgTrans
oz.ModuleWra1: Hdr:Variables
oz.ModuleWra1: Hdr:UpCall
oz.ModuleWra1: s.ResFiles
