# Makefile for PCIManager
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 04 Feb 1998	WT		Created

#
# Paths
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
CD	= dir
CHMOD	= access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:

#
# Program specific options:
#
COMPONENT = PCIManager
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS = o.PCI o.ModuleCore o.SwiHandler o.Hardware o.Debug o.MemCtrl o.Test o.StarCmds o.IRQEnable

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@

#
# Build a relocatable module:
#
all: ${RMTARGET}
	@echo ${COMPONENT}: all complete
	
#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
        ${RM} ${RMTARGET}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
        
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

#
# Relocatable module target
#
${RMTARGET}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
o.Debug:	c.Debug
o.Debug:	C:h.swis
o.Debug:	C:h.kernel
o.Debug:	C:h.kernel
o.Debug:	h.Debug
o.IRQEnable: s.IRQEnable
o.Test:	c.Test
o.Test:	C:h.kernel
o.Test:	h._Private
o.Test:	h._Public
o.Test:	h.Debug
o.Test:	h.MemCtrl
o.Test:	h.Test
o.Test:	C:h.kernel
o.Test:	h._Private
o.StarCmds:	c.StarCmds
o.StarCmds:	C:h.kernel
o.StarCmds:	C:h.swis
o.StarCmds:	h._Private
o.StarCmds:	h._Public
o.StarCmds:	h.Debug
o.StarCmds:	h.StarCmds
o.ModuleCore:	c.ModuleCore
o.ModuleCore:	C:h.kernel
o.ModuleCore:	C:h.swis
o.ModuleCore:	h._Private
o.ModuleCore:	h._Public
o.ModuleCore:	h.Debug
o.ModuleCore:	h.Hardware
o.ModuleCore:	C:h.kernel
o.ModuleCore:	h._Private
o.ModuleCore:	h.MemCtrl
o.ModuleCore:	h.ModuleCore
o.SwiHandler:	c.SwiHandler
o.SwiHandler:	C:h.kernel
o.SwiHandler:	C:h.swis
o.SwiHandler:	h._Private
o.SwiHandler:	h._Public
o.SwiHandler:	h.Debug
o.SwiHandler:	h.Hardware
o.SwiHandler:	C:h.kernel
o.SwiHandler:	h._Private
o.SwiHandler:	h.SWIHandler
o.Hardware:	c.Hardware
o.Hardware:	C:h.kernel
o.Hardware:	C:h.swis
o.Hardware:	h._Private
o.Hardware:	h._Public
o.Hardware:	h.Debug
o.Hardware:	h.Hardware
o.Hardware:	C:h.kernel
o.Hardware:	h._Private
o.MemCtrl:	c.MemCtrl
o.MemCtrl:	C:h.kernel
o.MemCtrl:	C:h.swis
o.MemCtrl:	h._Private
o.MemCtrl:	h._Public
o.MemCtrl:	h.Debug
o.MemCtrl:	h.Hardware
o.MemCtrl:	C:h.kernel
o.MemCtrl:	h._Private
o.MemCtrl:	h.MemCtrl
