# Makefile for the Upgrade Fetcher component.

#
# Paths:
#

EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
CD      = dir
CHMOD   = access
CREATE  = create
STAMP   = stamp

ifeq (${UserIf},MPT)
BUILDFLAGS = -DBUILD_MPT
endif

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -Wpc -depend !Depend ${THROWBACK} -ff ${INCLUDES} ${BUILDFLAGS} -DCOMPAT_INET4 -DUSE_CHECKURL ${OPTS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS     = ${DEBUG}

#
# Libraries:
#

include AppLibs

ZLIB  = C:zlib.o.zlib
LIBSD = ${DEBUGLIB} ${SOCK4LIB} ${INETLIB}

#
# Include files:
#

INCLUDES = -Itbox:,C:,TCPIPLibs:

#
# Program specific options:
#

COMPONENT = UpgFetch
TARGET    = abs.UpgFetch
APP       = !${COMPONENT}
INSTAPP   = ${INSTDIR}.Apps.${APP}
INSTLOCAL = Targets.${APP}

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
o.Utils		\
o.ROSLib

#
# Rule patterns:
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cw.o:;     ${CC}   ${CFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@

#
# UpgLaunch utility:
#

UPGLEAF		=	UpgLaunch
UPGLAUNCH	=	UpgLaunch.${UPGLEAF}
UPGLAUNCHCFS	=	${FileSwitch$CurrentFilingSystem}
UPGLAUNCHCSD	=	${UPGLAUNCHCFS}:${FileSwitch$${UPGLAUNCHCFS}$CSD}

${UPGLAUNCH}:
	@echo
	@echo Building ${UPGLEAF}
	@echo
	Set UpgLaunchTemp$Dir ${UPGLAUNCHCSD}
	Dir <UpgLaunchTemp$Dir>.UpgLaunch
	amu_machine all EXT_FLAGS=-throwback
	Dir <UpgLaunchTemp$Dir>
	Unset UpgLaunchTemp$Dir
	@echo
	@echo Finished.
	@echo

#
# Main build:
#

all: ${TARGET}
	@echo ${COMPONENT}: all complete

#
# Binary Build Environment augmenting rules:
#

BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_Dir abs
  BBE_Export_Dir o

#
# RISC OS ROM build rules:
#

resources: ${TARGET}
	${MKDIR} ${RESDIR}.^.Apps.${APP}
	Set UpgFetchRes$Path LocalUserIfRes:,LocalRes:
	${CP} UpgFetchRes:*       ${RESDIR}.^.Apps.${APP}.*            ${CPFLAGS}
	${CP} ${TARGET}           ${RESDIR}.^.Apps.${APP}.${COMPONENT} ${CPFLAGS}
	${CP} ${UPGLAUNCH}        ${RESDIR}.^.Apps.${APP}.${UPGLEAF}   ${CPFLAGS}
	UnSet UpgFetchRes$Path
	@echo ${COMPONENT}: all complete

install: ${TARGET}
	${MKDIR} ${INSTAPP}
	Set UpgFetchRes$Path LocalUserIfRes:,LocalRes:
	${CP} UpgFetchRes:*       ${INSTAPP}.*                         ${CPFLAGS}
	${CP} ${TARGET}           ${INSTAPP}.${COMPONENT}              ${CPFLAGS}
	${CP} ${UPGLAUNCH}        ${INSTAPP}.${UPGLEAF}                ${CPFLAGS}
	UnSet UpgFetchRes$Path
	@echo ${COMPONENT}: all complete

clean:
	${WIPE} o        ${WFLAGS}
	${WIPE} abs      ${WFLAGS}
	${WIPE} Targets  ${WFLAGS}
	${RM} abs.${TARGET}
	${RM} Dirs
	@echo
	@echo ${COMPONENT}: cleaned
	@echo
	Set UpgLaunchTemp$Dir ${UPGLAUNCHCSD}
	Dir <UpgLaunchTemp$Dir>.UpgLaunch
	amu_machine clean_all
	StripDepnd Makefile
	Dir <UpgLaunchTemp$Dir>
	Unset UpgLaunchTemp$Dir
	@echo

dirs:
	${MKDIR}  o
	${MKDIR}  abs
	${MKDIR}  Targets
	${MKDIR}  Targets.Apps
	${CREATE} Dirs 0
	${STAMP}  Dirs


#
# ROM target (re-linked at ROM Image build time):
#

${TARGET}: ${OBJS} ${UPGLAUNCH} dirs
	${LD} ${LFLAGS} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TBOXLIB} ${WIMPLIB} ${LIBSD} ${ZLIB}

#
# Dynamic dependencies:
