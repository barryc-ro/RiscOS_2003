# Makefile for CodecCtl
#

#
# Paths
#

EXP_HDR = <export$dir>

#
# Program specific options:
#

COMPONENT  = CodecCtl
APPNAME    = !CodecCtl
ROM_MODULE = aof.${COMPONENT}
MODULE     = rm.${COMPONENT}
APP        = abs.!RunImage

#
# Generic options:
#

MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -depend !Depend ${INCLUDES} ${THROWBACK} ${DFLAGS} -wp -fk
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
# Swap these lines to choose which version to make (only the second takes effect). Also,
# m.c is only needed when making a TRACE or CHECK version.
# NB!! This software currently *FAILS* for unexplained reasons if compiled without memory
# checking.
#DFLAGS   = -DTRACE=5 -DCHECK=1 -throwback -fah -strict
#DFLAGS   = -DCHECK=1 -ff
DFLAGS   = -ffah -strict ${OPTS}

#
# Libraries
#

CLIB       = CLIB:o.stubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.C_AbsSym
WRAPPER    = RISCOSLIB:s.ModuleWrap
LOCAL_WRAP = src.ModuleWrap

LIBS     = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib
LIBSZ    = ${ROMCSTUBS} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAM = ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib

#
# Include files
#

INCLUDES = -Itbox:,C:

FILES = \
 ${APP}

DIRS     = o._dirs

#
# Objects
#

OBJS      = o.plugclient o.utils o.client_rep o.clientmidi o.m
OBJSZ     = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWrap
OBJSZRAM  = oz.plugclient oz.utils oz.client_rep oz.clientmidi oz.ModuleWra1 oz.m
ASMS      = s.plugclient s.utils s.client_rep s.clientmidi s.event s.tbevent s.wimpevent s.wimpmsg s.m

# CodecCtl was System specific but not any more.
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

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

ram:    ${MODULE}
        @echo ${COMPONENT}: soft-load module installed

clean:
        ${WIPE} abs ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} src ${WFLAGS}
        ${RM} ${ROM_MODULE}
        ${RM} ${MODULE}
        ${RM} ${APP}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

${DIRS}:
        @${MKDIR} abs
        @${MKDIR} aof
        @${MKDIR} linked
        @${MKDIR} map
        @${MKDIR} o
        @${MKDIR} oz
        @${MKDIR} rm
        @${MKDIR} src
        ${CP} ${WRAPPER} ${LOCAL_WRAP} ${CPFLAGS}
        create ${DIRS}
        stamp  ${DIRS}

resources:
        ${MKDIR} ${RESAPP}
        Set CodecCtlRes$Path Resources.RCA.${LOCALE}.,Resources.RCA.UK.
        TokenCheck CodecCtlRes:Messages
        ${CP} CodecCtlRes:!BootROM ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} CodecCtlRes:!RunROM  ${RESAPP}.!Run ${CPFLAGS}
        ${CP} CodecCtlRes:Messages ${RESAPP}.Messages ${CPFLAGS}
        ${CP} CodecCtlRes:Res      ${RESAPP}.Res ${CPFLAGS}
        Unset CodecCtlRes$Path
        @echo ${COMPONENT}: resource files copied

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        Set CodecCtlRes$Path Resources.RCA.${LOCALE}.,Resources.RCA.UK.
        TokenCheck CodecCtlRes:Messages
        ${CP} CodecCtlRes:!Boot ${INSTDIR}.!Boot ${CPFLAGS}
        ${CP} CodecCtlRes:!Run  ${INSTDIR}.!Run ${CPFLAGS}
        ${CP} CodecCtlRes:Messages ${INSTDIR}.Messages ${CPFLAGS}
        ${CP} CodecCtlRes:Res      ${INSTDIR}.Res ${CPFLAGS}
        Unset CodecCtlRes$Path
        ${CP} ${APP}               ${INSTDIR}.!RunImage ${CPFLAGS}
        @echo ${COMPONENT}: Application installed to ${INSTDIR}
#
# Static dependencies
#

${APP}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
        Access $@ rw/rw

${MODULE}: ${OBJSZRAM} ${LIBSZRAM} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}
        Access $@ rw/rw

oz.ModuleWrap: s.AppName ${DIRS}
        ${AS} ${AFLAGS} ${LOCAL_WRAP} $@

oz.ModuleWra1: s.AppName ${DIRS}
        ${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" ${LOCAL_WRAP} $@

asms: ${ASMS}

#
# ROM target (re-linked at ROM Image build time)
#

${ROM_MODULE}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ} ${DIRS}
        ${LD} -o $@ -aof ${OBJSZ} ${LIBSZ}
        Access $@ rw/rw

#
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} aof.${TARGET} ${ABSSYM} > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
