# Makefile for SCTransport
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-May-96  RWB 	  New for SCTransport
# 25-Jul-96  RWB	  Changed install rules
# 11-Sep-96  RWB	  Corrected a major flaw in ROM build

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
DEFMOD  = DefMod
AR	= LibFile

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
#DFLAGS  = -DARM -D_DEBUG
DFLAGS  = -DARM -DUSE_TINY
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

# Program specific options:
#
COMPONENT = SCTrans
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.sys		\
o.com		\
o.db		\
o.data		\
o.api		\
o.atr		\
o.t0_sm		\
o.chan		\
o.irqs		\
o.serial	\
o.header

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} $< $@ 
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
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
# Target 
#
rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} 

o.header: cmhg.header
        $(CMHG) cmhg.header $@ h.header
        	
#
# Dynamic dependencies:
o.conn:	c.conn
o.conn:	h.arm
o.conn:	h.sci_api
o.conn:	h.sci_int
o.sys:	c.sys
o.sys:	C:h.kernel
o.sys:	C:h.swis
o.sys:	h.module
o.sys:	C:h.kernel
o.sys:	h.header
o.sys:	h.arm
o.sys:	h.sci_api
o.sys:	h.sci_int
o.sys:	h.atr
o.sys:	h.t0_sm
o.sys:	h.irqs
o.sys:	C:h.serial
o.sys:	h.serial_ven
o.com:	c.com
o.com:	C:h.kernel
o.com:	C:h.swis
o.com:	h.module
o.com:	C:h.kernel
o.com:	h.arm
o.com:	h.sci_api
o.com:	h.sci_int
o.com:	C:h.serial
o.com:	h.serial_ven
o.db:	c.db
o.data:	c.data
o.data:	h.arm
o.data:	h.sci_api
o.data:	h.sci_int
o.api:	c.api
o.api:	h.arm
o.api:	h.module
o.api:	C:h.kernel
o.api:	h.sci_api
o.api:	h.sci_int
o.atr:	c.atr
o.atr:	h.arm
o.atr:	h.sci_api
o.atr:	h.sci_int
o.atr:	h.atr
o.t0_sm:	c.t0_sm
o.t0_sm:	h.arm
o.t0_sm:	h.sci_api
o.t0_sm:	h.sci_int
o.t0_sm:	h.t0_sm
o.chan:	c.chan
o.chan:	h.arm
o.chan:	h.sci_api
o.chan:	h.sci_int
o.irqs: s.irqs
o.irqs: Hdr:ListOpts
o.irqs: Hdr:Macros
o.irqs: Hdr:System
o.irqs: Hdr:SWIs
o.irqs: Hdr:CPU.Generic26
o.irqs: Hdr:IO.GenericIO
o.irqs: Hdr:RISCOS
o.irqs: Hdr:ModHand
o.serial:	c.serial
o.serial:	C:h.kernel
o.serial:	C:h.swis
o.serial:	C:h.serial
o.serial:	h.serial_ven
o.serial:	h.arm
o.serial:	h.sci_api
o.serial:	h.sci_int
o.serial:	h.module
o.serial:	C:h.kernel
o.serial:	h.irqs
