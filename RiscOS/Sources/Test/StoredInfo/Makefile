# Makefile for StoredInfo
#

#
# Paths
#
EXP_HDR = <export$dir>

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS} -throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:

DFLAGS   =

#
# Program specific options:
#
COMPONENT = StoredInfo
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
OBJS      = header.o module.o storedinfo.o veneer.o \
            Display.o DRAMTest.o FaultCodes.o InputTest.o \
            Modem.o Msgs.o NVRAMTest.o ROMCheck.o \
            SCardTest.o Sound.o Support.o System.o UserDiag.o \
            CPUType.o IOControl.o MemSrcNCOS.o ROMChksum.o usermode.o
EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${RAMTARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic deo.module:	c.module
# Dynamic dependencies:
o.veneer: s.veneer
o.veneer: Hdr:ListOpts
o.veneer: Hdr:Macros
o.veneer: Hdr:System
o.veneer: Hdr:SWIs
o.veneer: Hdr:CPU.Generic26
o.veneer: Hdr:IO.GenericIO
o.veneer: Hdr:RISCOS
o.veneer: Hdr:ModHand
o.Display:	c.Display
o.Display:	C:h.kernel
o.Display:	C:h.swis
o.Display:	h.display
o.Display:	h.msgs
o.Display:	h.support
o.DRAMTest:	c.DRAMTest
o.DRAMTest:	C:h.kernel
o.DRAMTest:	C:h.swis
o.DRAMTest:	h.dramtest
o.DRAMTest:	h.display
o.DRAMTest:	h.support
o.DRAMTest:	h.msgs
o.DRAMTest:	h.MemSrcNCOS
o.InputTest:	c.InputTest
o.InputTest:	C:h.kernel
o.InputTest:	C:h.swis
o.InputTest:	h.InputTest
o.InputTest:	h.display
o.InputTest:	h.support
o.InputTest:	h.msgs
o.Modem:	c.Modem
o.Modem:	C:h.kernel
o.Modem:	C:h.swis
o.Modem:	h.Modem
o.Modem:	h.display
o.Modem:	h.support
o.Modem:	h.Msgs
o.Msgs:	c.Msgs
o.NVRAMTest:	c.NVRAMTest
o.NVRAMTest:	C:h.kernel
o.NVRAMTest:	C:h.swis
o.NVRAMTest:	h.nvramtest
o.NVRAMTest:	h.display
o.NVRAMTest:	h.support
o.ROMCheck:	c.ROMCheck
o.ROMCheck:	C:h.kernel
o.ROMCheck:	C:h.swis
o.ROMCheck:	h.ROMCheck
o.ROMCheck:	h.display
o.ROMCheck:	h.support
o.SCardTest:	c.SCardTest
o.SCardTest:	C:h.kernel
o.SCardTest:	C:h.swis
o.SCardTest:	h.SCardTest
o.SCardTest:	h.display
o.SCardTest:	h.support
o.SCardTest:	h.usermode
o.SCardTest:	C:h.kernel
o.SCardTest:	C:h.scinter
o.SCardTest:	h.Msgs
o.Sound:	c.Sound
o.Sound:	C:h.kernel
o.Sound:	C:h.swis
o.Sound:	h.sound
o.Sound:	h.display
o.Sound:	h.support
o.Sound:	h.msgs
o.Support:	c.Support
o.Support:	C:h.kernel
o.Support:	C:h.swis
o.Support:	h.support
o.Support:	C:h.led
o.Support:	h.display
o.Support:	h.Msgs
o.System:	c.System
o.System:	C:h.kernel
o.System:	C:h.swis
o.System:	h.system
o.System:	h.display
o.System:	h.support
o.System:	h.msgs
o.System:	h.CPUType
o.System:	h.IOControl
o.UserDiag:	c.UserDiag
o.UserDiag:	C:h.kernel
o.UserDiag:	C:h.swis
o.UserDiag:	h.UserDiag
o.UserDiag:	h.display
o.UserDiag:	h.support
o.UserDiag:	h.ROMCheck
o.UserDiag:	h.DRAMTest
o.UserDiag:	h.NVRAMTest
o.UserDiag:	h.FaultCodes
o.UserDiag:	h.system
o.UserDiag:	h.Modem
o.UserDiag:	h.InputTest
o.UserDiag:	h.Sound
o.UserDiag:	h.SCardTest
o.CPUType: s.CPUType
o.CPUType: Hdr:ListOpts
o.CPUType: Hdr:Macros
o.CPUType: Hdr:MEMM.ARM600
o.IOControl: s.IOControl
o.IOControl: Hdr:ListOpts
o.IOControl: Hdr:Macros
o.IOControl: Hdr:System
o.IOControl: Hdr:SWIs
o.IOControl: Hdr:CPU.Generic26
o.IOControl: Hdr:IO.GenericIO
o.IOControl: Hdr:RISCOS
o.IOControl: Hdr:Machine.<Machine>
o.IOControl: Hdr:IO.IOMDL
o.MemSrcNCOS: s.MemSrcNCOS
o.MemSrcNCOS: Hdr:ListOpts
o.MemSrcNCOS: Hdr:Macros
o.MemSrcNCOS: Hdr:System
o.MemSrcNCOS: Hdr:SWIs
o.MemSrcNCOS: Hdr:CPU.Generic26
o.MemSrcNCOS: Hdr:IO.GenericIO
o.MemSrcNCOS: Hdr:RISCOS
o.ROMChksum: s.ROMChksum
o.ROMChksum: Hdr:ListOpts
o.ROMChksum: Hdr:Macros
o.ROMChksum: Hdr:System
o.ROMChksum: Hdr:SWIs
o.ROMChksum: Hdr:CPU.Generic26
o.ROMChksum: Hdr:IO.GenericIO
o.ROMChksum: Hdr:RISCOS
o.ROMChksum: Hdr:Machine.<Machine>
o.ROMChksum: Hdr:IO.IOMDL
o.usermode: s.usermode
o.usermode: Hdr:ListOpts
o.usermode: Hdr:Macros
o.usermode: Hdr:System
o.usermode: Hdr:SWIs
o.usermode: Hdr:CPU.Generic26
o.usermode: Hdr:IO.GenericIO
o.usermode: Hdr:RISCOS
o.usermode: Hdr:EnvNumbers
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	h.storedinfo
o.storedinfo:	c.storedinfo
o.storedinfo:	C:h.kernel
o.storedinfo:	C:h.swis
o.storedinfo:	C:h.nvram
o.storedinfo:	C:h.standby
o.storedinfo:	h.storedinfo
o.storedinfo:	h.veneer
o.storedinfo:	h.error
o.FaultCodes:	c.FaultCodes
o.FaultCodes:	C:h.kernel
o.FaultCodes:	C:h.swis
o.FaultCodes:	h.FaultCodes
o.FaultCodes:	h.display
o.FaultCodes:	h.support
o.FaultCodes:	h.storedinfo
o.FaultCodes:	C:h.nvram
