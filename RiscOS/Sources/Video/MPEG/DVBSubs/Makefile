# Makefile for DVBSubs
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name    Description
# ----         ----    -----------
# 12-Feb-2002  BJGA    Created.

COMPONENT = DVBSubtitles
TARGET    = DVBSubs
DIRS      = o._dirs
CFLAGS    = -ffah -zM -Wp
RAM_OBJS  = o.module ${OBJS}
ROM_OBJS  = o.moduleROM ${OBJS}
OBJS      =            o.basetask  o.global  o.maintask  o.mess o.modentry o.packet o.resmess o.modhdr
DBG_OBJS  = do.module do.basetask do.global do.maintask do.mess o.modentry o.packet o.resmess o.modhdr
LIBS      = ${EVENTLIB} ${WIMPLIB} C:MPEG.o.MPEG
DBG_LIBS  = ${LIBS} <Lib$Dir>.DebugLib.o.debuglibzm TCPIPLibs:o.socklib5zm TCPIPLibs:o.inetlibzm
DBG_MODULE = drm.${TARGET}

include Makefiles:StdTools
include Makefiles:ModuleLibs
include Makefiles:ModStdRule
include Makefiles:RAMCModule
include Makefiles:ROMCModule

.SUFFIXES: .do
.c.do:; ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<

o._dirs:
        ${MKDIR} do
        ${MKDIR} o
        ${TOUCH} $@

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

o.basetask: modhdr.h
do.basetask: modhdr.h
o.global: modhdr.h
do.global: modhdr.h
o.module: modhdr.h
do.module: modhdr.h
o.maintask: modhdr.h
do.maintask: modhdr.h

moduleROM.o: module.c modhdr.h
        ${CC} ${CFLAGS} -DROM_MODULE -o moduleROM.o module.c

resmess.o: Messages${CMDHELP}
	ResGen resmess_ResourcesFiles o.resmess Messages${CMDHELP} Resources.DVBSubtitles.Messages

Messages: LocalRes:Messages LocalRes:CmdHelp
        FAppend $@ LocalRes:Messages LocalRes:CmdHelp

MessagesNone: LocalRes:Messages
        ${CP} LocalRes:Messages $@ ${CPFLAGS}

# Dynamic dependencies:
