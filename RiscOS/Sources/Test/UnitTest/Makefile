# Makefile for UnitTest
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created
# 1997-12-01	BAL		Added clean_all, cleans o${BUILD}, creates
#				o${BUILD}


#
# Paths
#
EXP_HDR = <export$dir>

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
CD	= dir

#AFLAGS = -depend !Depend -Stamp -quit -To $@ -From
AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DFLAGS  = ${DEBUG}

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
#
# Include files
#
INCLUDES = -ITbox: -IC:


# Program specific options:
#
COMPONENT = UnitTest
TARGET    = !RunImage
APP	  = !${COMPONENT}
OM        = o${BUILD}

BASEOBJS  = ${OM}.Display ${OM}.DRAMTest ${OM}.InputTest \
            ${OM}.Modem ${OM}.Msgs ${OM}.NVRAMTest ${OM}.ROMCheck \
            ${OM}.Sound ${OM}.Support ${OM}.System ${OM}.UserDiag \
            ${OM}.CPUType ${OM}.IOControl ${OM}.MemSrcNCOS \
            ${OM}.ROMChksum
Module    = ${OM}.header ${OM}.module ${OM}.usermode ${OM}.UnitTest
Applic    = ${OM}.AppMain
BOCA      = ${BASEOBJS} ${Module} ${OM}.FaultCodes ${OM}.SCardTest ${OM}.storedinfo ${OM}.veneer
Morris2   = ${BASEOBJS} ${Applic} ${OM}.FlashCheck ${OM}.Serial ${OM}.bufman ${OM}.serial_lib \
            ${OM}.CheckIC ${OM}.serflash

OBJS      = ${Morris2}

.SUFFIXES: .${OM}

#
# Rule patterns
#
.c.${OM}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${OM}:;   ${CMHG} -o $@ $<
.s.${OM}:;      ${AS}   ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE}	o.*		${WFLAGS}
        ${WIPE}	o${BUILD}.*	${WFLAGS}
        ${WIPE}	${TARGET}	${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${WIPE}	${APP}	${WFLAGS}
	@echo ${COMPONENT}: cleaned all

install:
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	@echo ${COMPONENT}: installed

dirs:
	${MKDIR}	o${BUILD}

#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} -o $@ ${OBJS} ${CLIB}

# Dynamic dependencies:
