# Makefile for the Upgrade Fetcher component.

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o
INSTDIR = <obey$dir>.install
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
CD	= dir
CHMOD	= access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -Wpc -depend !Depend ${THROWBACK} -ff ${INCLUDES} -DUSE_CHECKURL ${OPTS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R
LFLAGS     =

DFLAGS  = ${DEBUG}
#
# Libraries
#

CLIB       = CLIB:o.stubs
ANSILIB    = CLIB:o.ansilib
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
EVENTLIB   = Tbox:o.eventlib
LIBSD      = <Lib$Dir>.DebugLib.o.debuglibzm
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB    = Tbox:o.wimplib
TCPIPLIB   = <Lib$Dir>.TCPIPLibs.o.*
ZLIB       = <Lib$Dir>.zlib.o.zlib

#
# Include files
#
INCLUDES = -Itbox:,C:,TCPIPLibs:

# Program specific options:
#
COMPONENT = UpgFetch
TARGET    = aof.UpgFetch
APP       = !${COMPONENT}

OBJS = 		\
o.crc32		\
o.Dirs		\
o.Errors	\
o.FromROSLib	\
o.Global	\
o.kill		\
o.main		\
o.ui		\
o.URLUtils	\
o.Utils

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cw.o:;     ${CC}   ${CFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${TARGET}
	@echo ${COMPONENT}: all complete

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_Dir aof
  BBE_Export_Dir o

#
# RISC OS ROM build rules:
#

resources: ${TARGET}
	${MKDIR} ${RESDIR}.^.Apps.${APP}
	SetMacro UpgFetchRes$path Resources.$(USERIF).$(LOCALE).,Resources.$(USERIF).UK.
	${CP} ${TARGET} ${RESDIR}.^.Apps.${APP}.${COMPONENT} ${CPFLAGS}
	${CP} UpgFetchRes:* ${RESDIR}.^.Apps.${APP}.* ${CPFLAGS}
	UnSet UpgFetchRes$path
	@echo ${COMPONENT}: all complete


install: ${TARGET}
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${COMPONENT} ${CPFLAGS}
	${CP} Resources.$(USERIF).$(LOCALE).* ${APP}.* ${CPFLAGS}
	@echo ${COMPONENT}: all complete


clean:
        ${WIPE} o.*        ${WFLAGS}
        ${WIPE} aof.*      ${WFLAGS}
        ${WIPE} rm.*       ${WFLAGS}
        ${WIPE} install.*  ${WFLAGS}
        ${WIPE} ${APP}     ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${APP}
        ${RM} o
        ${RM} aof
        ${RM} rm
        @echo ${COMPONENT}: cleaned

dirs:
        ${MKDIR} o
        ${MKDIR} aof
        ${MKDIR} rm

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} dirs
        ${LD} ${LFLAGS} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${LIBSD} ${ZLIB} ${TCPIPLIB} ${SOCKLIB} ${INETLIB} ${RLIB}


#
# Dynamic dependencies:
