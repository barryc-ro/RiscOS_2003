# Makefile for PFCClock
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name    Description
# ----         ----    -----------
# 28-Mar-2002  BJGA    Created.

COMPONENT = PFCClock
TARGET    = PFCClock
DIRS      = local_dirs
CFLAGS    = -ffah -zM -Wpe -zps1
RAM_OBJS  = o.module ${OBJS}
ROM_OBJS  = o.moduleROM ${OBJS}
OBJS      =            o.clock  o.decoder  o.event  o.global o.int64  o.llist  o.mess o.resmess o.time o.modhdr
DBG_OBJS  = do.module do.clock do.decoder do.event do.global o.int64 do.llist do.mess o.resmess o.time o.modhdr
LIBDIR    = <Lib$Dir>
LIBS      = C:MPEG.o.MPEG
DBG_LIBS  = ${LIBS} ${LIBDIR}.DebugLib.o.debuglibzm TCPIPLibs:o.socklib5zm TCPIPLibs:o.inetlibzm
DBG_MODULE = drm.${TARGET}
EXPORTS   = ${LIBDIR}.PRISM.h.PFCClock

include Makefiles:StdTools
include Makefiles:ModuleLibs
include Makefiles:ModStdRule
include Makefiles:RAMCModule
include Makefiles:ROMCModule

.SUFFIXES: .do
.c.do:; ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<

local_dirs:
        ${MKDIR} do
        ${MKDIR} o

export: ${EXPORTS}
        @${ECHO} ${COMPONENT}: export complete

${EXPORTS}: modhdr.h PFCClock.h
        ${MKDIR} ${LIBDIR}.PRISM.h
        ${CP} h.PFCClock $@ ${CPFLAGS}
        Do ${AWK} -- "/.define PFCClock_00/,/endif/" h.modhdr >> $@

resources: Messages${CMDHELP}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Messages${CMDHELP} ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        ${RM} Messages${CMDHELP}
        @${ECHO} ${COMPONENT}: resource files copied

clean:
        ${RM} Messages
        ${RM} h.modhdr
        ifthere linked then wipe linked ${WFLAGS}
        ifthere aof    then wipe aof    ${WFLAGS}
        ifthere drm    then wipe drm    ${WFLAGS}
        ifthere do     then wipe do     ${WFLAGS}
        ifthere rm     then wipe rm     ${WFLAGS}
        ifthere o      then wipe o      ${WFLAGS}
        @${ECHO} ${COMPONENT}: cleaned

debug: ${DBG_MODULE}
        @${ECHO} ${COMPONENT}: debug module built

${DBG_MODULE}: ${DBG_OBJS} ${DBG_LIBS} ${CLIB} ${DIRS}
        ${MKDIR} drm
        ${LD} ${LDFLAGS} -o $@ -rmf ${DBG_OBJS} ${DBG_LIBS} ${CLIB}
        ${CHMOD} -R a+rx drm

o.module: modhdr.h
do.module: modhdr.h

moduleROM.o: module.c modhdr.h
        ${CC} ${CFLAGS} -DROM_MODULE -o moduleROM.o module.c

resmess.o: Messages${CMDHELP}
	ResGen resmess_ResourcesFiles o.resmess Messages${CMDHELP} Resources.PFCClock.Messages

Messages: LocalRes:Messages LocalRes:CmdHelp
        FAppend $@ LocalRes:Messages LocalRes:CmdHelp

MessagesNone: LocalRes:Messages
        ${CP} LocalRes:Messages $@ ${CPFLAGS}

# Dynamic dependencies:
