# Makefile for UnitTest
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 1997-09-17	RWB		Created
# 1997-12-01	BAL		Added clean_all, cleans o${BUILD}, creates
#				o${BUILD}
# 1999-02-01    SBF		Added STB3 targets, plus call to StripDepnd tool
# 1999-02-02    NCE		Added TestCard BASIC Prog
# 1999-02-04    SBF		Added wipe rule to remove o${BUILD} on clean all
# 1999-02-08    SBF		Modified to place crunched BASIC into "crunched" directory
# 				instead of leaving the object files in the root.  Also
#				creates directories at build time
# 1999-02-12	NCE		Added Printer BASIC prog
# 1999-02-16    SBF             Added LDFLAGS to allow addition of link tool flags
# 1999-02-23    SBF             Removed Printer target (BASIC file now removed)
# 1999-03-03    SBF             Added TCPIPLibs
# 1999-03-08    SBF             Modified install phase to build targets (if necessary)
# 1999-08-04    NCE             Created resource directorys for individual machine builds.
# 1999-08-04    NCE		Added -DUSERIF${USSERIF} to cflags so that I can switch things
# 				on UserIF.  This is used on Lazarus builds.
# 1999-08-10	NCE		Added Lazarus Target
# 2000-10-19    RLT             Added NVRAMTags to Lazarus target

#
# Paths
#
EXP_HDR = <export$dir>
RESDIR  = <Resource$Dir>.Resources2.Test
#
# Generic options:
#
MKDIR   = cdir
#AS      = aasm
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = -stripdepnd
CRUNCH  = bascrunch
CD	= dir

#AFLAGS  = -depend !Depend -Stamp -quit -To $@ -From
AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -Wp ${INCLUDES} -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 -DUSERIF${USERIF}=1 ${DFLAGS} ${EXT_FLAGS}
#LDFLAGS =
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = ${DEBUG}
BFLAGS  = 31

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = Tbox:o.eventlib
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
ASMUTILS   = C:AsmUtils.o.AsmUtils
#
# Include files
#
INCLUDES = -ITbox: -IC: -ITCPIPLibs: -IC:AsmUtils


# Program specific options:
#
COMPONENT = UnitTest
TARGET    = !RunImage
APP	  = !${COMPONENT}
CRUNCHED  = crunched
OM        = o${BUILD}


BASEOBJS  = ${OM}.Display ${OM}.DRAMTest ${OM}.InputTest \
            ${OM}.Modem ${OM}.Msgs ${OM}.NVRAMTest ${OM}.ROMCheck \
            ${OM}.Sound ${OM}.Support ${OM}.System ${OM}.UserDiag \
            ${OM}.CPUType ${OM}.IOControl ${OM}.MemSrcNCOS \
            ${OM}.ROMChksum TCPIPLibs:o.Unixlib TCPIPLibs:o.Inetlib TCPIPLibs:o.Socklib5
            
Module    = ${OM}.header ${OM}.module ${OM}.UnitTest

Applic    = ${OM}.AppMain

BOCA      = ${BASEOBJS} ${Module} ${OM}.FaultCodes ${OM}.SCardTest ${OM}.storedinfo ${OM}.veneer

Morris2   = ${BASEOBJS} ${Applic} ${OM}.FlashCheck ${OM}.Serial ${OM}.bufman ${OM}.serial_lib \
            ${OM}.CheckIC ${OM}.serflash ${OM}.ledtest

Daytona   = ${BASEOBJS} ${Applic} ${OM}.FlashCheck ${OM}.Serial ${OM}.bufman ${OM}.serial_lib \
            ${OM}.CheckIC ${OM}.serflash ${OM}.ledtest

STB3      = ${BASEOBJS} ${Applic} ${OM}.serial_lib ${OM}.IRRemote ${OM}.MouseTest \
            ${OM}.Graphics ${OM}.CMOS ${OM}.SerialTest ${OM}.TestCard ${OM}.Printer \
            ${OM}.MPEG ${OM}.Server
            
Lazarus   = ${BASEOBJS} ${Applic} ${OM}.serial_lib ${OM}.IRRemote ${OM}.MouseTest \
            ${OM}.Graphics ${OM}.CMOS ${OM}.SerialTest ${OM}.TestCard ${OM}.Printer \
            ${OM}.MPEG ${OM}.Server ${OM}.Parallel ${OM}.Pop ${OM}.Serial ${OM}.bufman \
            ${OM}.NVRAMTags

OBJS      = ${Lazarus}


.SUFFIXES: .${OM}

#
# Rule patterns
#
.c.${OM}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${OM}:;   ${CMHG} -o $@ $<
.s.${OM}:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE}	o.*		${WFLAGS}
        ${WIPE}	o${BUILD}.*	${WFLAGS}
        ${WIPE} @.${CRUNCHED}.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${WIPE}	${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}
	${WIPE} rm		${WFLAGS}
        ${WIPE} o${BUILD}	${WFLAGS}
        ${WIPE} ${CRUNCHED}	${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

install: ${TARGET}
	SetMacro LocalResMac$Path  Resources.${MACHINE}.<Locale>.,Resources.${MACHINE}.UK.
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} LocalResMac:* ${APP}.* ${CPFLAGS}
	${CP} ${CRUNCHED}.TestCard ${APP}.TestCard ${CPFLAGS}
	${CP} ${CRUNCHED}.Gen_lib ${APP}.Gen_lib ${CPFLAGS}
	UnSet LocalResMac$Path
	@echo ${COMPONENT}: installed

dirs:
	${MKDIR} o
	${MKDIR} rm
	${MKDIR} o${BUILD}
	${MKDIR} ${CRUNCHED}
	
resources: ${TARGET}
	${MKDIR} ${APP}
	SetMacro LocalResMac$Path  Resources.${MACHINE}.<Locale>.,Resources.${MACHINE}.UK.
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} LocalResMac:* ${APP}.* ${CPFLAGS}
	${CP} ${CRUNCHED}.TestCard ${APP}.TestCard ${CPFLAGS}
	${CP} ${CRUNCHED}.Gen_lib ${APP}.Gen_lib ${CPFLAGS}
	${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
	UnSet LocalResMac$Path
	@echo ${COMPONENT}: built & installed

#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${ASMUTILS} ${CLIB}
        @echo ${COMPONENT}: link stage complete
        ${CRUNCH} ${BFLAGS} @.b.TestCard @.${CRUNCHED}.TestCard
        ${CRUNCH} ${BFLAGS} @.b.Gen_lib @.${CRUNCHED}.Gen_lib


# Dynamic dependencies:
