# Skeleton OBJASM/C Makefile for application
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created


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
Morris2   = ${BASEOBJS} ${Applic} ${OM}.FlashCheck ${OM}.Serial ${OM}.bufman ${OM}.serial_lib

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

install:
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	@echo ${COMPONENT}: installed
#
# Target 
#
${TARGET}: ${OBJS}  
        ${LD} -o $@ ${OBJS} ${CLIB} 
#${EVENTLIB} 
#${TOOLBOXLIB} 
#${WIMPLIB}
#
# Dynamic dependencies:
oMorris2.CPUType: s.CPUType
oMorris2.CPUType: Hdr:ListOpts
oMorris2.CPUType: Hdr:Macros
oMorris2.CPUType: Hdr:MEMM.ARM600
oMorris2.IOControl: s.IOControl
oMorris2.IOControl: Hdr:ListOpts
oMorris2.IOControl: Hdr:Macros
oMorris2.IOControl: Hdr:System
oMorris2.IOControl: Hdr:SWIs
oMorris2.IOControl: Hdr:CPU.Generic26
oMorris2.IOControl: Hdr:IO.GenericIO
oMorris2.IOControl: Hdr:RISCOS
oMorris2.IOControl: Hdr:Machine.<Machine>
oMorris2.IOControl: Hdr:IO.IOMDL
oMorris2.ROMChksum: s.ROMChksum
oMorris2.ROMChksum: Hdr:ListOpts
oMorris2.ROMChksum: Hdr:Macros
oMorris2.ROMChksum: Hdr:System
oMorris2.ROMChksum: Hdr:SWIs
oMorris2.ROMChksum: Hdr:CPU.Generic26
oMorris2.ROMChksum: Hdr:IO.GenericIO
oMorris2.ROMChksum: Hdr:RISCOS
oMorris2.ROMChksum: Hdr:Machine.<Machine>
oMorris2.ROMChksum: Hdr:IO.IOMDL
oMorris2.Serial:	c.Serial
oMorris2.Serial:	C:h.kernel
oMorris2.Serial:	C:h.swis
oMorris2.Serial:	h.serial_lib
oMorris2.Serial:	C:h.kernel
oMorris2.Serial:	h.Serial
oMorris2.Serial:	h.DebugLib
oMorris2.bufman: s.bufman
oMorris2.bufman: h.CVars
oMorris2.bufman: h.SWINames
oMorris2.serial_lib:	c.serial_lib
oMorris2.serial_lib:	C:h.kernel
oMorris2.serial_lib:	C:h.swis
oMorris2.serial_lib:	h.serial_lib
oMorris2.serial_lib:	C:h.kernel
oMorris2.MemSrcNCOS: s.MemSrcNCOS
oMorris2.MemSrcNCOS: Hdr:ListOpts
oMorris2.MemSrcNCOS: Hdr:Macros
oMorris2.MemSrcNCOS: Hdr:System
oMorris2.MemSrcNCOS: Hdr:SWIs
oMorris2.MemSrcNCOS: Hdr:CPU.Generic26
oMorris2.MemSrcNCOS: Hdr:IO.GenericIO
oMorris2.MemSrcNCOS: Hdr:RISCOS
oMorris2.DRAMTest:	c.DRAMTest
oMorris2.DRAMTest:	C:h.kernel
oMorris2.DRAMTest:	C:h.swis
oMorris2.DRAMTest:	h.support
oMorris2.DRAMTest:	h.module
oMorris2.DRAMTest:	h.dramtest
oMorris2.DRAMTest:	h.display
oMorris2.DRAMTest:	h.msgs
oMorris2.DRAMTest:	h.MemSrcNCOS
oMorris2.InputTest:	c.InputTest
oMorris2.InputTest:	C:h.kernel
oMorris2.InputTest:	C:h.swis
oMorris2.InputTest:	h.support
oMorris2.InputTest:	h.module
oMorris2.InputTest:	h.InputTest
oMorris2.InputTest:	h.display
oMorris2.InputTest:	h.msgs
oMorris2.InputTest:	h.usermode
oMorris2.InputTest:	C:h.kernel
oMorris2.Modem:	c.Modem
oMorris2.Modem:	C:h.kernel
oMorris2.Modem:	C:h.swis
oMorris2.Modem:	h.support
oMorris2.Modem:	h.module
oMorris2.Modem:	h.Modem
oMorris2.Modem:	h.display
oMorris2.Modem:	h.Msgs
oMorris2.Modem:	h.Dialler
oMorris2.Modem:	h.Serial
oMorris2.Modem:	h.types
oMorris2.Modem:	h.bufman
oMorris2.Modem:	h.types
oMorris2.Modem:	h.daytonahw
oMorris2.Modem:	C:h.kernel
oMorris2.Modem:	h.diallerb
oMorris2.Msgs:	c.Msgs
oMorris2.Msgs:	h.support
oMorris2.NVRAMTest:	c.NVRAMTest
oMorris2.NVRAMTest:	C:h.kernel
oMorris2.NVRAMTest:	C:h.swis
oMorris2.NVRAMTest:	h.support
oMorris2.NVRAMTest:	h.nvramtest
oMorris2.NVRAMTest:	h.display
oMorris2.ROMCheck:	c.ROMCheck
oMorris2.ROMCheck:	C:h.kernel
oMorris2.ROMCheck:	C:h.swis
oMorris2.ROMCheck:	h.support
oMorris2.ROMCheck:	h.ROMCheck
oMorris2.ROMCheck:	h.display
oMorris2.Sound:	c.Sound
oMorris2.Sound:	C:h.kernel
oMorris2.Sound:	C:h.swis
oMorris2.Sound:	h.support
oMorris2.Sound:	h.sound
oMorris2.Sound:	h.display
oMorris2.Sound:	h.msgs
oMorris2.Support:	c.Support
oMorris2.Support:	C:h.kernel
oMorris2.Support:	C:h.swis
oMorris2.Support:	h.module
oMorris2.Support:	h.support
oMorris2.Support:	C:h.led
oMorris2.Support:	h.display
oMorris2.Support:	h.Msgs
oMorris2.Support:	h.usermode
oMorris2.Support:	C:h.kernel
oMorris2.Support:	C:h.string
oMorris2.Support:	h.daytonahw
oMorris2.Support:	C:h.kernel
oMorris2.System:	c.System
oMorris2.System:	C:h.kernel
oMorris2.System:	C:h.swis
oMorris2.System:	h.support
oMorris2.System:	h.system
oMorris2.System:	h.display
oMorris2.System:	h.msgs
oMorris2.System:	h.CPUType
oMorris2.System:	h.IOControl
oMorris2.AppMain:	c.AppMain
oMorris2.AppMain:	C:h.kernel
oMorris2.AppMain:	C:h.swis
oMorris2.AppMain:	h.Support
oMorris2.AppMain:	h.UserDiag
oMorris2.Display:	c.Display
oMorris2.Display:	C:h.kernel
oMorris2.Display:	C:h.swis
oMorris2.Display:	h.support
oMorris2.Display:	h.module
oMorris2.Display:	h.display
oMorris2.Display:	h.msgs
oMorris2.UserDiag:	c.UserDiag
oMorris2.UserDiag:	C:h.kernel
oMorris2.UserDiag:	C:h.swis
oMorris2.UserDiag:	h.support
oMorris2.UserDiag:	C:h.Standby
oMorris2.UserDiag:	h.module
oMorris2.UserDiag:	h.UserDiag
oMorris2.UserDiag:	h.display
oMorris2.UserDiag:	h.ROMCheck
oMorris2.UserDiag:	h.DRAMTest
oMorris2.UserDiag:	h.NVRAMTest
oMorris2.UserDiag:	h.system
oMorris2.UserDiag:	h.Modem
oMorris2.UserDiag:	h.InputTest
oMorris2.UserDiag:	h.Sound
oMorris2.UserDiag:	h.msgs
oMorris2.UserDiag:	h.FlashCheck
oMorris2.UserDiag:	h.usermode
oMorris2.UserDiag:	C:h.kernel
oMorris2.FlashCheck:	c.FlashCheck
oMorris2.FlashCheck:	C:h.kernel
oMorris2.FlashCheck:	C:h.swis
oMorris2.FlashCheck:	h.support
oMorris2.FlashCheck:	h.FlashCheck
oMorris2.FlashCheck:	h.display
oNFDA.Display:	c.Display
oNFDA.Display:	C:h.kernel
oNFDA.Display:	C:h.swis
oNFDA.Display:	h.support
oNFDA.Display:	h.module
oNFDA.Display:	h.display
oNFDA.Display:	h.msgs
oNFDA.DRAMTest:	c.DRAMTest
oNFDA.DRAMTest:	C:h.kernel
oNFDA.DRAMTest:	C:h.swis
oNFDA.DRAMTest:	h.support
oNFDA.DRAMTest:	h.module
oNFDA.DRAMTest:	h.dramtest
oNFDA.DRAMTest:	h.display
oNFDA.DRAMTest:	h.msgs
oNFDA.DRAMTest:	h.MemSrcNCOS
oNFDA.InputTest:	c.InputTest
oNFDA.InputTest:	C:h.kernel
oNFDA.InputTest:	C:h.swis
oNFDA.InputTest:	h.support
oNFDA.InputTest:	h.module
oNFDA.InputTest:	h.InputTest
oNFDA.InputTest:	h.display
oNFDA.InputTest:	h.msgs
oNFDA.InputTest:	h.usermode
oNFDA.InputTest:	C:h.kernel
oNFDA.Modem:	c.Modem
oNFDA.Modem:	C:h.kernel
oNFDA.Modem:	C:h.swis
oNFDA.Modem:	h.support
oNFDA.Modem:	h.module
oNFDA.Modem:	h.Modem
oNFDA.Modem:	h.display
oNFDA.Modem:	h.Msgs
oNFDA.Modem:	h.Dialler
oNFDA.Modem:	h.Serial
oNFDA.Modem:	h.types
oNFDA.Modem:	h.bufman
oNFDA.Modem:	h.types
oNFDA.Modem:	h.daytonahw
oNFDA.Modem:	C:h.kernel
oNFDA.Modem:	h.diallerb
oNFDA.Msgs:	c.Msgs
oNFDA.Msgs:	h.support
oNFDA.NVRAMTest:	c.NVRAMTest
oNFDA.NVRAMTest:	C:h.kernel
oNFDA.NVRAMTest:	C:h.swis
oNFDA.NVRAMTest:	h.support
oNFDA.NVRAMTest:	h.nvramtest
oNFDA.NVRAMTest:	h.display
oNFDA.ROMCheck:	c.ROMCheck
oNFDA.ROMCheck:	C:h.kernel
oNFDA.ROMCheck:	C:h.swis
oNFDA.ROMCheck:	h.support
oNFDA.ROMCheck:	h.ROMCheck
oNFDA.ROMCheck:	h.display
oNFDA.Sound:	c.Sound
oNFDA.Sound:	C:h.kernel
oNFDA.Sound:	C:h.swis
oNFDA.Sound:	h.support
oNFDA.Sound:	h.sound
oNFDA.Sound:	h.display
oNFDA.Sound:	h.msgs
oNFDA.Support:	c.Support
oNFDA.Support:	C:h.kernel
oNFDA.Support:	C:h.swis
oNFDA.Support:	h.module
oNFDA.Support:	h.support
oNFDA.Support:	C:h.led
oNFDA.Support:	h.display
oNFDA.Support:	h.Msgs
oNFDA.Support:	h.usermode
oNFDA.Support:	C:h.kernel
oNFDA.Support:	C:h.string
oNFDA.Support:	h.daytonahw
oNFDA.Support:	C:h.kernel
oNFDA.System:	c.System
oNFDA.System:	C:h.kernel
oNFDA.System:	C:h.swis
oNFDA.System:	h.support
oNFDA.System:	h.system
oNFDA.System:	h.display
oNFDA.System:	h.msgs
oNFDA.System:	h.CPUType
oNFDA.System:	h.IOControl
oNFDA.System:	h.daytonahw
oNFDA.System:	C:h.kernel
oNFDA.UserDiag:	c.UserDiag
oNFDA.UserDiag:	C:h.kernel
oNFDA.UserDiag:	C:h.swis
oNFDA.UserDiag:	h.support
oNFDA.UserDiag:	C:h.Standby
oNFDA.UserDiag:	h.module
oNFDA.UserDiag:	h.UserDiag
oNFDA.UserDiag:	h.display
oNFDA.UserDiag:	h.ROMCheck
oNFDA.UserDiag:	h.DRAMTest
oNFDA.UserDiag:	h.NVRAMTest
oNFDA.UserDiag:	h.system
oNFDA.UserDiag:	h.Modem
oNFDA.UserDiag:	h.InputTest
oNFDA.UserDiag:	h.Sound
oNFDA.UserDiag:	h.msgs
oNFDA.UserDiag:	h.FlashCheck
oNFDA.UserDiag:	h.usermode
oNFDA.UserDiag:	C:h.kernel
oNFDA.CPUType: s.CPUType
oNFDA.CPUType: Hdr:ListOpts
oNFDA.CPUType: Hdr:Macros
oNFDA.CPUType: Hdr:MEMM.ARM600
oNFDA.IOControl: s.IOControl
oNFDA.IOControl: Hdr:ListOpts
oNFDA.IOControl: Hdr:Macros
oNFDA.IOControl: Hdr:System
oNFDA.IOControl: Hdr:SWIs
oNFDA.IOControl: Hdr:CPU.Generic26
oNFDA.IOControl: Hdr:IO.GenericIO
oNFDA.IOControl: Hdr:RISCOS
oNFDA.IOControl: Hdr:Machine.<Machine>
oNFDA.IOControl: Hdr:Machine.Machine
oNFDA.IOControl: Hdr:IO.IOMDL
oNFDA.MemSrcNCOS: s.MemSrcNCOS
oNFDA.MemSrcNCOS: Hdr:ListOpts
oNFDA.MemSrcNCOS: Hdr:Macros
oNFDA.MemSrcNCOS: Hdr:System
oNFDA.MemSrcNCOS: Hdr:SWIs
oNFDA.MemSrcNCOS: Hdr:CPU.Generic26
oNFDA.MemSrcNCOS: Hdr:IO.GenericIO
oNFDA.MemSrcNCOS: Hdr:RISCOS
oNFDA.ROMChksum: s.ROMChksum
oNFDA.ROMChksum: Hdr:ListOpts
oNFDA.ROMChksum: Hdr:Macros
oNFDA.ROMChksum: Hdr:System
oNFDA.ROMChksum: Hdr:SWIs
oNFDA.ROMChksum: Hdr:CPU.Generic26
oNFDA.ROMChksum: Hdr:IO.GenericIO
oNFDA.ROMChksum: Hdr:RISCOS
oNFDA.ROMChksum: Hdr:Machine.<Machine>
oNFDA.ROMChksum: Hdr:Machine.Machine
oNFDA.ROMChksum: Hdr:IO.IOMDL
oNFDA.AppMain:	c.AppMain
oNFDA.AppMain:	C:h.kernel
oNFDA.AppMain:	C:h.swis
oNFDA.AppMain:	h.Support
oNFDA.AppMain:	h.UserDiag
oNFDA.FlashCheck:	c.FlashCheck
oNFDA.FlashCheck:	C:h.kernel
oNFDA.FlashCheck:	C:h.swis
oNFDA.FlashCheck:	h.support
oNFDA.FlashCheck:	h.FlashCheck
oNFDA.FlashCheck:	h.display
oNFDA.Serial:	c.Serial
oNFDA.Serial:	C:h.kernel
oNFDA.Serial:	C:h.swis
oNFDA.Serial:	h.serial_lib
oNFDA.Serial:	C:h.kernel
oNFDA.Serial:	h.Serial
oNFDA.Serial:	h.DebugLib
oNFDA.bufman: s.bufman
oNFDA.bufman: h.CVars
oNFDA.bufman: h.SWINames
oNFDA.serial_lib:	c.serial_lib
oNFDA.serial_lib:	C:h.kernel
oNFDA.serial_lib:	C:h.swis
oNFDA.serial_lib:	h.serial_lib
oNFDA.serial_lib:	C:h.kernel
