
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
CFLAGS     = -c -Wa -depend !Depend ${THROWBACK} -ff ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R
DFLAGS     =  ${DEBUG}

#
# Libraries
#

ABSSYM     = RISC_OSLib:o.AbsSym
ANSILIB    = CLIB:o.ansilib
CLIB       = CLIB:o.stubs
EVENTLIB   = Tbox:o.eventlib
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
RLIB       = RISCOSLIB:o.risc_oslib
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
RSTUBS     = RISCOSLIB:o.rstubs
TCPIPLIB   = <Lib$Dir>.TCPIPLibs.o.*
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
ZLIB 	   = <Lib$Dir>.zlib.o.zlib

#
# Include files
#
INCLUDES = -Itbox:,C:,TCPIPLibs:

# Program specific options:
#
COMPONENT = UpgFlash
TARGET    = aof.UpgFlash
APP	  = !${COMPONENT}

OBJS      =	\
o.main		\
o.flashprog     \
o.fetcher	\
o.stuff         \
o.endscreen	\
o.messages	\
o.kill          


#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
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
        SetMacro UpgFlashRes$path Resources.$(USERIF).$(LOCALE).,Resources.$(USERIF).UK.
        ${CP} ${TARGET} ${RESDIR}.^.Apps.${APP}.${COMPONENT} ${CPFLAGS}
        ${CP} UpgFlashRes:!Boot ${RESDIR}.^.Apps.${APP}.* ${CPFLAGS}
        ${CP} UpgFlashRes:!Run ${RESDIR}.^.Apps.${APP}.* ${CPFLAGS}
        ${CP} UpgFlashRes:messages ${RESDIR}.^.Apps.${APP}.* ${CPFLAGS}
        UnSet UpgFlashRes$path
        @echo ${COMPONENT}: all complete

install:   ${TARGET}
	${MKDIR} ${APP}
        SetMacro UpgFlashRes$path Resources.$(USERIF).$(LOCALE).,Resources.$(USERIF).UK.
        ${CP} ${TARGET} ${APP}.${COMPONENT} ${CPFLAGS}
        ${CP} UpgFlashRes:!Boot ${APP}.* ${CPFLAGS}
        ${CP} UpgFlashRes:!Run ${APP}.* ${CPFLAGS}
        ${CP} UpgFlashRes:messages ${APP}.* ${CPFLAGS}
        UnSet UpgFlashRes$path
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.*       ${WFLAGS}
        ${WIPE} aof.*       ${WFLAGS}
        ${WIPE} rm.*       ${WFLAGS}
        ${WIPE} install.* ${WFLAGS}
        ${WIPE} ${APP} ${WFLAGS}
        ${RM} ${TARGET}
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
        ${LD} -o $@ ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${ZLIB} ${ANSILIB} ${TCPIPLIB}


#
# Dynamic dependencies:
