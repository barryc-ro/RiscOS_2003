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
CFLAGS  = -depend !Depend ${INCLUDES} ${DFLAGS} -wp
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
# Swap these lines to choose which version to make (only the second takes effect). Also,
# m.c is only needed when making a TRACE or CHECK version.
DFLAGS   = -DTRACE=2 -DCHECK=1 -throwback -fah -strict
# NB!! This software currently *FAILS* for unexplained reasons if compiled without memory
# checking.
DFLAGS   = -DCHECK=1 -ff

#
# Libraries
#
CLIB      = CLIB:o.stubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.C_AbsSym
WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Include files
#
INCLUDES = -Itbox:,C:

LIBS     = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib
LIBSZ    = ${ROMCSTUBS} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAM = ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib

#
# Program specific options:
#
COMPONENT = CodecCtl
APPNAME   = !CodecCtl
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
APP       = abs.!RunImage
OBJS      = o.plugclient o.utils o.client_rep o.clientmidi o.event o.tbevent o.wimpevent o.wimpmsg o.m
OBJSZ     = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWrap oz.event oz.tbevent oz.wimpevent oz.wimpmsg oz.m
OBJSZRAM  = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWra1 oz.event oz.tbevent oz.wimpevent oz.wimpmsg oz.m
ASMS      = s.plugclient s.utils s.client_rep s.clientmidi s.event s.tbevent s.wimpevent s.wimpmsg s.m

# CodecCtl was System specific but not any more.
RDIR      = Resources.RCA.${LOCALE}
EXPORTS   =

.SUFFIXES: .oz .c .o .src .cmhg .s

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APPNAME}

#
# Rule patterns
#
.c.o:;    ${CC} -c ${CFLAGS} $<
.c.oz:;   ${CC} -c ${CFLAGS} -zM -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<
.src.o:;  ${AS} ${AFLAGS} $< $@
.c.s:;    ${CC} -s ${CFLAGS} $<

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
        ${WIPE} s.* ${WFLAGS}
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
	Access $@ rw/rw

${RAMTARGET}: ${OBJSZRAM} ${LIBSZRAM}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}
	Access $@ rw/rw

oz.ModuleWrap: src.ModuleWrap hdr.AppName
	${AS} ${AFLAGS} src.ModuleWrap $@

oz.ModuleWra1: src.ModuleWrap hdr.AppName
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" src.ModuleWrap $@

src.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

asms: ${ASMS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ}
        ${LD} -o $@ -aof ${OBJSZ} ${LIBSZ}
	Access $@ rw/rw

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
o.m:	c.m
o.m:	C:h.kernel
o.m:	h.m
o.m:	h.utils
o.plugclient:	c.plugclient
o.plugclient:	C:h.swis
o.plugclient:	C:h.kernel
o.plugclient:	tbox:h.toolbox
o.plugclient:	tbox:h.wimp
o.plugclient:	tbox:h.window
o.plugclient:	tbox:h.gadgets
o.plugclient:	tbox:h.quit
o.plugclient:	h.event
o.plugclient:	tbox:h.wimplib
o.plugclient:	h.m
o.plugclient:	C:h.plugin
o.plugclient:	h.plugclient
o.plugclient:	h.version
o.plugclient:	h.client
o.plugclient:	h.utils
o.plugclient:	h.utils
o.utils:	c.utils
o.utils:	C:h.kernel
o.utils:	C:h.swis
o.utils:	h.m
o.utils:	tbox:h.wimp
o.utils:	tbox:h.wimplib
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
o.client_rep:	h.event
o.client_rep:	h.m
o.client_rep:	h.plugclient
o.client_rep:	C:h.plugin
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
o.clientmidi:	h.event
o.clientmidi:	h.m
o.clientmidi:	h.plugclient
o.clientmidi:	C:h.plugin
o.clientmidi:	h.client
o.clientmidi:	h.utils
o.clientmidi:	h.version
o.clientmidi:	h.utils
o.event:	c.event
o.event:	C:h.kernel
o.event:	h.m
o.event:	h.utils
o.event:	h.types
o.event:	tbox:h.wimp
o.event:	tbox:h.toolbox
o.event:	h.event
o.event:	h.event
o.event:	h.wimpevent
o.event:	h.tbevent
o.event:	h.wimpmsg
o.tbevent:	c.tbevent
o.tbevent:	C:h.kernel
o.tbevent:	h.m
o.tbevent:	h.event
o.tbevent:	tbox:h.wimp
o.tbevent:	tbox:h.toolbox
o.tbevent:	h.utils
o.tbevent:	h.types
o.tbevent:	h.tbevent
o.wimpevent:	c.wimpevent
o.wimpevent:	C:h.kernel
o.wimpevent:	h.m
o.wimpevent:	h.utils
o.wimpevent:	h.event
o.wimpevent:	tbox:h.wimp
o.wimpevent:	tbox:h.toolbox
o.wimpevent:	h.types
o.wimpevent:	h.wimpevent
o.wimpmsg:	c.wimpmsg
o.wimpmsg:	C:h.kernel
o.wimpmsg:	h.m
o.wimpmsg:	h.utils
o.wimpmsg:	h.event
o.wimpmsg:	tbox:h.wimp
o.wimpmsg:	tbox:h.toolbox
o.wimpmsg:	h.types
o.wimpmsg:	h.wimpmsg
oz.ModuleWrap: src.ModuleWrap
oz.ModuleWrap: hdr.AppName
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
oz.ModuleWrap: hdr.ResFiles
oz.plugclient:	c.plugclient
oz.plugclient:	C:h.swis
oz.plugclient:	C:h.kernel
oz.plugclient:	tbox:h.toolbox
oz.plugclient:	tbox:h.wimp
oz.plugclient:	tbox:h.window
oz.plugclient:	tbox:h.gadgets
oz.plugclient:	tbox:h.quit
oz.plugclient:	h.event
oz.plugclient:	tbox:h.wimplib
oz.plugclient:	h.m
oz.plugclient:	C:h.plugin
oz.plugclient:	h.plugclient
oz.plugclient:	h.version
oz.plugclient:	h.client
oz.plugclient:	h.utils
oz.plugclient:	h.utils
oz.utils:	c.utils
oz.utils:	C:h.kernel
oz.utils:	C:h.swis
oz.utils:	h.m
oz.utils:	tbox:h.wimp
oz.utils:	tbox:h.wimplib
oz.utils:	h.version
oz.utils:	h.utils
oz.client_rep:	c.client_rep
oz.client_rep:	C:h.swis
oz.client_rep:	C:h.kernel
oz.client_rep:	tbox:h.wimplib
oz.client_rep:	tbox:h.wimp
oz.client_rep:	tbox:h.window
oz.client_rep:	tbox:h.toolbox
oz.client_rep:	tbox:h.gadgets
oz.client_rep:	h.event
oz.client_rep:	h.m
oz.client_rep:	h.plugclient
oz.client_rep:	C:h.plugin
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
oz.clientmidi:	h.event
oz.clientmidi:	h.m
oz.clientmidi:	h.plugclient
oz.clientmidi:	C:h.plugin
oz.clientmidi:	h.client
oz.clientmidi:	h.utils
oz.clientmidi:	h.version
oz.clientmidi:	h.utils
oz.ModuleWra1: src.ModuleWrap
oz.ModuleWra1: hdr.AppName
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
oz.ModuleWra1: hdr.ResFiles
oz.event:	c.event
oz.event:	C:h.kernel
oz.event:	h.m
oz.event:	h.utils
oz.event:	h.types
oz.event:	tbox:h.wimp
oz.event:	tbox:h.toolbox
oz.event:	h.event
oz.event:	h.event
oz.event:	h.wimpevent
oz.event:	h.tbevent
oz.event:	h.wimpmsg
oz.tbevent:	c.tbevent
oz.tbevent:	C:h.kernel
oz.tbevent:	h.m
oz.tbevent:	h.event
oz.tbevent:	tbox:h.wimp
oz.tbevent:	tbox:h.toolbox
oz.tbevent:	h.utils
oz.tbevent:	h.types
oz.tbevent:	h.tbevent
oz.wimpevent:	c.wimpevent
oz.wimpevent:	C:h.kernel
oz.wimpevent:	h.m
oz.wimpevent:	h.utils
oz.wimpevent:	h.event
oz.wimpevent:	tbox:h.wimp
oz.wimpevent:	tbox:h.toolbox
oz.wimpevent:	h.types
oz.wimpevent:	h.wimpevent
oz.wimpmsg:	c.wimpmsg
oz.wimpmsg:	C:h.kernel
oz.wimpmsg:	h.m
oz.wimpmsg:	h.utils
oz.wimpmsg:	h.event
oz.wimpmsg:	tbox:h.wimp
oz.wimpmsg:	tbox:h.toolbox
oz.wimpmsg:	h.types
oz.wimpmsg:	h.wimpmsg
oz.m:	c.m
oz.m:	C:h.kernel
oz.m:	h.m
oz.m:	h.utils
