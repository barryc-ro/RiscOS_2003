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
# 07 May 1997 RWB	  Now supports remote debug library

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
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

DFLAGS = -DUSE_TINY ${DEBUG}
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = SCTrans
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.sys		\
o.com		\
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
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
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
rm.${COMPONENT}: ${OBJS} ${CLIB} ${REMOTEDB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} ${REMOTEDB}
        Access rm.${COMPONENT} RW/R

#
# Dynamic dependencies:
o.data:	c.data
o.data:	h.sci_api
o.data:	h.module
o.data:	C:h.kernel
o.data:	C:debug.h.remote
o.data:	h.sci_int
o.chan:	c.chan
o.chan:	h.sci_api
o.chan:	h.module
o.chan:	C:h.kernel
o.chan:	C:debug.h.remote
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
o.serial:	h.sci_api
o.serial:	h.module
o.serial:	C:h.kernel
o.serial:	C:debug.h.remote
o.serial:	h.sci_int
o.serial:	h.module
o.serial:	h.irqs
o.com:	c.com
o.com:	C:h.kernel
o.com:	C:h.swis
o.com:	h.module
o.com:	C:h.kernel
o.com:	C:debug.h.remote
o.com:	h.sci_api
o.com:	h.module
o.com:	h.sci_int
o.com:	C:h.serial
o.com:	h.serial_ven
o.api:	c.api
o.api:	h.module
o.api:	C:h.kernel
o.api:	C:debug.h.remote
o.api:	h.sci_api
o.api:	h.module
o.api:	h.sci_int
o.sys:	c.sys
o.sys:	C:h.kernel
o.sys:	C:h.swis
o.sys:	h.module
o.sys:	C:h.kernel
o.sys:	C:debug.h.remote
o.sys:	h.header
o.sys:	h.sci_api
o.sys:	h.module
o.sys:	h.sci_int
o.sys:	h.atr
o.sys:	h.t0_sm
o.sys:	h.irqs
o.sys:	C:h.serial
o.sys:	h.serial_ven
o.atr:	c.atr
o.atr:	h.sci_api
o.atr:	h.module
o.atr:	C:h.kernel
o.atr:	C:debug.h.remote
o.atr:	h.sci_int
o.atr:	h.atr
o.t0_sm:	c.t0_sm
o.t0_sm:	h.sci_api
o.t0_sm:	h.module
o.t0_sm:	C:h.kernel
o.t0_sm:	C:debug.h.remote
o.t0_sm:	h.sci_int
o.t0_sm:	h.t0_sm
