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
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${DFLAGS} -wp
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

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

# Swap these lines to choose which version to make (only the second takes effect). Also,
# m.c is only needed when making a DEBUG version.
DFLAGS   = -DDEBUG=2 -throwback
DFLAGS   =

#
# Program specific options:
#
COMPONENT = CodecCtl
APPNAME   = !CodecCtl
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
APP       = abs.!RunImage
OBJS      = o.plugclient o.utils o.client_rep o.clientmidi #o.m o.event o.tbevent o.wimpevent o.wimpmsg
OBJSZ     = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWrap #oz.m oz.event oz.tbevent oz.wimpevent oz.wimpmsg
OBJSZRAM  = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWra1 #oz.m oz.event oz.tbevent oz.wimpevent oz.wimpmsg

# CodecCtl was System specific but not any more.
RDIR      = Resources.RCA.${LOCALE}
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
	Access $@ rw/rw

${RAMTARGET}: ${OBJSZRAM} ${LIBSZRAM}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}
	Access $@ rw/rw

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
