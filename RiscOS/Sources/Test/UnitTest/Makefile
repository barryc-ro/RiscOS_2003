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
# 1999-02-01    SBF             Added STB3 targets, plus call to StripDepnd tool
# 1999-02-02    NCE             Added TestCard BASIC Prog
# 1999-02-04    SBF             Added wipe rule to remove o${BUILD} on clean all


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
STRIP   = -stripdepnd
CD	= dir

# BASIC Flags:
BasCrunchflags = 31



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
            ${OM}.CheckIC ${OM}.serflash ${OM}.ledtest
            
STB3      = ${BASEOBJS} ${Applic} ${OM}.serial_lib ${OM}.IRRemote ${OM}.MouseTest \
            ${OM}.Graphics 
            
OBJS      = ${STB3}


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
        ${WIPE} @.TestCard	${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${WIPE}	${APP}	${WFLAGS}
        ${WIPE} o${BUILD} ${WFLAGS}
	@echo ${COMPONENT}: cleaned all

install:
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${CP} @.TestCard ${APP}.TestCard ${CPFLAGS}
	@echo ${COMPONENT}: installed

dirs:
	${MKDIR}	o${BUILD}
	

#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} -o $@ ${OBJS} ${CLIB}
        BasCrunch $(BasCrunchflags) @.b.TestCard @.TestCard
        
           
        
# Dynamic dependencies:
oSTB3.Display:	c.Display
oSTB3.Display:	C:h.kernel
oSTB3.Display:	C:h.swis
oSTB3.Display:	h.support
oSTB3.Display:	h.BuildFlags
oSTB3.Display:	h.module
oSTB3.Display:	h.display
oSTB3.Display:	h.msgs
oSTB3.DRAMTest:	c.DRAMTest
oSTB3.DRAMTest:	C:h.kernel
oSTB3.DRAMTest:	C:h.swis
oSTB3.DRAMTest:	h.support
oSTB3.DRAMTest:	h.BuildFlags
oSTB3.DRAMTest:	h.module
oSTB3.DRAMTest:	h.dramtest
oSTB3.DRAMTest:	h.display
oSTB3.DRAMTest:	h.msgs
oSTB3.DRAMTest:	h.MemSrcNCOS
oSTB3.InputTest:	c.InputTest
oSTB3.InputTest:	C:h.kernel
oSTB3.InputTest:	C:h.swis
oSTB3.InputTest:	h.support
oSTB3.InputTest:	h.BuildFlags
oSTB3.InputTest:	h.module
oSTB3.InputTest:	h.InputTest
oSTB3.InputTest:	h.display
oSTB3.InputTest:	h.msgs
oSTB3.InputTest:	h.usermode
oSTB3.InputTest:	C:h.kernel
oSTB3.Modem:	c.Modem
oSTB3.Modem:	C:h.kernel
oSTB3.Modem:	C:h.swis
oSTB3.Modem:	h.support
oSTB3.Modem:	h.BuildFlags
oSTB3.Modem:	h.module
oSTB3.Modem:	h.Modem
oSTB3.Modem:	h.display
oSTB3.Modem:	h.Msgs
oSTB3.Modem:	h.Dialler
oSTB3.Msgs:	c.Msgs
oSTB3.Msgs:	h.support
oSTB3.Msgs:	h.BuildFlags
oSTB3.NVRAMTest:	c.NVRAMTest
oSTB3.NVRAMTest:	C:h.kernel
oSTB3.NVRAMTest:	C:h.swis
oSTB3.NVRAMTest:	h.support
oSTB3.NVRAMTest:	h.BuildFlags
oSTB3.NVRAMTest:	h.nvramtest
oSTB3.NVRAMTest:	h.display
oSTB3.ROMCheck:	c.ROMCheck
oSTB3.ROMCheck:	C:h.kernel
oSTB3.ROMCheck:	C:h.swis
oSTB3.ROMCheck:	h.support
oSTB3.ROMCheck:	h.BuildFlags
oSTB3.ROMCheck:	h.ROMCheck
oSTB3.ROMCheck:	h.display
oSTB3.Sound:	c.Sound
oSTB3.Sound:	C:h.kernel
oSTB3.Sound:	C:h.swis
oSTB3.Sound:	h.support
oSTB3.Sound:	h.BuildFlags
oSTB3.Sound:	h.sound
oSTB3.Sound:	h.display
oSTB3.Sound:	h.msgs
oSTB3.Support:	c.Support
oSTB3.Support:	C:h.kernel
oSTB3.Support:	C:h.swis
oSTB3.Support:	h.module
oSTB3.Support:	h.support
oSTB3.Support:	h.BuildFlags
oSTB3.Support:	C:h.led
oSTB3.Support:	h.display
oSTB3.Support:	h.Msgs
oSTB3.Support:	h.usermode
oSTB3.Support:	C:h.kernel
oSTB3.Support:	C:h.string
oSTB3.System:	c.System
oSTB3.System:	C:h.kernel
oSTB3.System:	C:h.swis
oSTB3.System:	h.support
oSTB3.System:	h.BuildFlags
oSTB3.System:	h.system
oSTB3.System:	h.display
oSTB3.System:	h.msgs
oSTB3.System:	h.CPUType
oSTB3.System:	h.IOControl
oSTB3.System:	h.daytonahw
oSTB3.System:	C:h.kernel
oSTB3.UserDiag:	c.UserDiag
oSTB3.UserDiag:	C:h.kernel
oSTB3.UserDiag:	C:h.swis
oSTB3.UserDiag:	h.support
oSTB3.UserDiag:	h.BuildFlags
oSTB3.UserDiag:	C:h.Standby
oSTB3.UserDiag:	h.module
oSTB3.UserDiag:	h.UserDiag
oSTB3.UserDiag:	h.display
oSTB3.UserDiag:	h.ROMCheck
oSTB3.UserDiag:	h.DRAMTest
oSTB3.UserDiag:	h.NVRAMTest
oSTB3.UserDiag:	h.system
oSTB3.UserDiag:	h.Modem
oSTB3.UserDiag:	h.InputTest
oSTB3.UserDiag:	h.Sound
oSTB3.UserDiag:	h.msgs
oSTB3.UserDiag:	h.FaultCodes
oSTB3.UserDiag:	h.SCardTest
oSTB3.UserDiag:	h.usermode
oSTB3.UserDiag:	C:h.kernel
oSTB3.UserDiag:	h.buildflags
oSTB3.UserDiag:	h.IRRemote
oSTB3.UserDiag:	h.MouseTest
oSTB3.CPUType: s.CPUType
oSTB3.CPUType: Hdr:ListOpts
oSTB3.CPUType: Hdr:Macros
oSTB3.CPUType: Hdr:MEMM.ARM600
oSTB3.IOControl: s.IOControl
oSTB3.IOControl: Hdr:ListOpts
oSTB3.IOControl: Hdr:Macros
oSTB3.IOControl: Hdr:System
oSTB3.IOControl: Hdr:SWIs
oSTB3.IOControl: Hdr:CPU.Generic26
oSTB3.IOControl: Hdr:IO.GenericIO
oSTB3.IOControl: Hdr:RISCOS
oSTB3.IOControl: Hdr:Machine.<Machine>
oSTB3.IOControl: Hdr:Machine.Machine
oSTB3.IOControl: Hdr:IO.IOMDL
oSTB3.MemSrcNCOS: s.MemSrcNCOS
oSTB3.MemSrcNCOS: Hdr:ListOpts
oSTB3.MemSrcNCOS: Hdr:Macros
oSTB3.MemSrcNCOS: Hdr:System
oSTB3.MemSrcNCOS: Hdr:SWIs
oSTB3.MemSrcNCOS: Hdr:CPU.Generic26
oSTB3.MemSrcNCOS: Hdr:IO.GenericIO
oSTB3.MemSrcNCOS: Hdr:RISCOS
oSTB3.ROMChksum: s.ROMChksum
oSTB3.ROMChksum: Hdr:ListOpts
oSTB3.ROMChksum: Hdr:Macros
oSTB3.ROMChksum: Hdr:System
oSTB3.ROMChksum: Hdr:SWIs
oSTB3.ROMChksum: Hdr:CPU.Generic26
oSTB3.ROMChksum: Hdr:IO.GenericIO
oSTB3.ROMChksum: Hdr:RISCOS
oSTB3.ROMChksum: Hdr:Machine.<Machine>
oSTB3.ROMChksum: Hdr:Machine.Machine
oSTB3.ROMChksum: Hdr:IO.IOMDL
oSTB3.AppMain:	c.AppMain
oSTB3.AppMain:	C:h.kernel
oSTB3.AppMain:	C:h.swis
oSTB3.AppMain:	h.Support
oSTB3.AppMain:	h.BuildFlags
oSTB3.AppMain:	h.UserDiag
oSTB3.serial_lib:	c.serial_lib
oSTB3.serial_lib:	C:h.kernel
oSTB3.serial_lib:	C:h.swis
oSTB3.serial_lib:	h.serial_lib
oSTB3.serial_lib:	C:h.kernel
oSTB3.IRRemote:	c.IRRemote
oSTB3.IRRemote:	C:h.kernel
oSTB3.IRRemote:	C:h.swis
oSTB3.IRRemote:	h.support
oSTB3.IRRemote:	h.BuildFlags
oSTB3.IRRemote:	h.module
oSTB3.IRRemote:	h.InputTest
oSTB3.IRRemote:	h.display
oSTB3.IRRemote:	h.msgs
oSTB3.IRRemote:	h.usermode
oSTB3.IRRemote:	C:h.kernel
oSTB3.IRRemote:	h.IRRemote
oSTB3.MouseTest:	c.MouseTest
oSTB3.MouseTest:	C:h.kernel
oSTB3.MouseTest:	C:h.swis
oSTB3.MouseTest:	h.support
oSTB3.MouseTest:	h.BuildFlags
oSTB3.MouseTest:	h.display
oSTB3.MouseTest:	h.msgs
oSTB3.MouseTest:	h.graphics
oSTB3.Graphics:	c.Graphics
oSTB3.Graphics:	C:h.kernel
oSTB3.Graphics:	C:h.swis
oSTB3.Graphics:	h.graphics
