# Makefile for CodecCtl
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
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} -DFORCE_OWN_WINDOW=1 ${DFLAGS}
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
#DFLAGS   = -DDEBUG=4

#
# Program specific options:
#
COMPONENT = CodecCtl
APPNAME   = !CodecCtl
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
APP       = abs.!RunImage
OBJS      = o.plugclient o.utils o.client_rep o.clientmidi
OBJSZ     = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWrap
OBJSZRAM  = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWra1
RDIR      = Resources.<System>.${LOCALE}
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

ram:    ${RAMTARGET}
	@echo ${COMPONENT}: soft-load module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.!BootROM ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} ${RDIR}.!RunROM ${RESAPP}.!Run ${CPFLAGS}
        ${CP} ${RDIR}.Messages ${RESAPP}.Messages ${CPFLAGS}
        ${CP} ${RDIR}.Res ${RESAPP}.Res ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Static dependencies
#
${APP}: ${OBJS} ${LIBS}
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

${RAMTARGET}: ${OBJSZRAM} ${LIBSZRAM}
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
o.plugclient:	c.plugclient
o.plugclient:	C:h.swis
o.plugclient:	C:h.kernel
o.plugclient:	tbox:h.toolbox
o.plugclient:	tbox:h.wimp
o.plugclient:	tbox:h.window
o.plugclient:	tbox:h.gadgets
o.plugclient:	tbox:h.quit
o.plugclient:	tbox:h.event
o.plugclient:	tbox:h.wimplib
o.plugclient:	h.plugin
o.plugclient:	h.plugclient
o.plugclient:	h.version
o.plugclient:	h.client
o.plugclient:	h.utils
o.plugclient:	h.utils
o.utils:	c.utils
o.utils:	C:h.swis
o.utils:	C:h.kernel
o.utils:	tbox:h.wimp
o.utils:	tbox:h.wimplib
o.utils:	C:h.kernel
o.utils:	h.version
o.utils:	h.utils
o.client_rep:	c.client_rep
o.client_rep:	C:h.swis
o.client_rep:	C:h.kernel
o.client_rep:	tbox:h.wimplib
o.client_rep:	tbox:h.wimp
o.client_rep:	tbox:h.window
o.client_rep:	tbox:h.toolbox
o.client_rep:	tbox:h.gadgets
o.client_rep:	tbox:h.event
o.client_rep:	h.plugclient
o.client_rep:	h.plugin
o.client_rep:	h.client
o.client_rep:	h.utils
o.client_rep:	h.utils
o.clientmidi:	c.clientmidi
o.clientmidi:	C:h.swis
o.clientmidi:	C:h.kernel
o.clientmidi:	tbox:h.wimplib
o.clientmidi:	tbox:h.wimp
o.clientmidi:	tbox:h.window
o.clientmidi:	tbox:h.toolbox
o.clientmidi:	tbox:h.gadgets
o.clientmidi:	tbox:h.event
o.clientmidi:	h.plugclient
o.clientmidi:	h.plugin
o.clientmidi:	h.client
o.clientmidi:	h.utils
o.clientmidi:	h.version
o.clientmidi:	h.utils
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
oz.client_rep:	c.client_rep
oz.client_rep:	C:h.swis
oz.client_rep:	C:h.kernel
oz.client_rep:	tbox:h.wimplib
oz.client_rep:	tbox:h.wimp
oz.client_rep:	tbox:h.window
oz.client_rep:	tbox:h.toolbox
oz.client_rep:	tbox:h.gadgets
oz.client_rep:	tbox:h.event
oz.client_rep:	h.plugclient
oz.client_rep:	h.plugin
oz.client_rep:	h.client
oz.client_rep:	h.utils
oz.client_rep:	h.utils
oz.clientmidi:	c.clientmidi
oz.clientmidi:	C:h.swis
oz.clientmidi:	C:h.kernel
oz.clientmidi:	tbox:h.wimplib
oz.clientmidi:	tbox:h.wimp
oz.clientmidi:	tbox:h.window
oz.clientmidi:	tbox:h.toolbox
oz.clientmidi:	tbox:h.gadgets
oz.clientmidi:	tbox:h.event
oz.clientmidi:	h.plugclient
oz.clientmidi:	h.plugin
oz.clientmidi:	h.client
oz.clientmidi:	h.utils
oz.clientmidi:	h.version
oz.clientmidi:	h.utils
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
oz.plugclient:	c.plugclient
oz.plugclient:	C:h.swis
oz.plugclient:	C:h.kernel
oz.plugclient:	tbox:h.toolbox
oz.plugclient:	tbox:h.wimp
oz.plugclient:	tbox:h.window
oz.plugclient:	tbox:h.gadgets
oz.plugclient:	tbox:h.quit
oz.plugclient:	tbox:h.event
oz.plugclient:	tbox:h.wimplib
oz.plugclient:	h.plugin
oz.plugclient:	h.plugclient
oz.plugclient:	h.version
oz.plugclient:	h.client
oz.plugclient:	h.utils
oz.plugclient:	h.utils
oz.utils:	c.utils
oz.utils:	C:h.swis
oz.utils:	C:h.kernel
oz.utils:	tbox:h.wimp
oz.utils:	tbox:h.wimplib
oz.utils:	C:h.kernel
oz.utils:	h.version
oz.utils:	h.utils
