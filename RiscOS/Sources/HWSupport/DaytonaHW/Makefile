# Makefile for DaytonaHW
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-07-18	BAL		Created
# 1998-03-27	RDW		MemCheckCC & LD added to CC & LD
#				${EXTRALIBS} added to LIBS
#				${EXTRACFLAGS} added to CFLAGS
#


# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H = <CExport$dir>.h


#
# Generic options:
#
MKDIR		= cdir
AS		= objasm
CC		= $(MemCheckCC) cc
CMHG		= cmhg
CP		= copy
LD		= $(MemCheckLD) link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE		= -wipe

ASMFLAGS	= -depend !Depend -Stamp -quit -throwback
CFLAGS		= -c -depend !Depend -strict -wp -zM -zps1 -ffah -throwback ${INCLUDES} ${EXTRACFLAGS}
CMHGFLAGS	= -depend !Depend -p -throwback ${DFLAGS}
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

DFLAGS		= 
DDFLAGS		= ${DFLAGS} -DTrace_TRACE -DDEBUGLIB


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM 		= RISC_OSLib:o.AbsSym


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:,<Lib$Dir>


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= DaytonaHW
RAMTARGET	= rm.DaytonaHW
ROMTARGET	= aof.DaytonaHW
DEBUGTARGET	= rm.DaytonaHWD
EXPORTS		= ${EXP_C_H}.${COMPONENT}

OBJS = \
 o.bufman \
 o.bufmanlo \
 o.buzzer \
 o.eeprom\
 o.error\
 o.fakeexch \
 o.fakeexchlo \
 o.interrupts \
 o.led \
 o.localfax\
 o.modem \
 o.module \
 o.modulehdr \
 o.pio_reg \
 o.watchdog \
 o.wimpmsg

OBJSD = \
 od.bufman \
 od.bufmanlo \
 od.buzzer \
 od.eeprom\
 od.error\
 od.fakeexch \
 od.fakeexchlo \
 od.interrupts \
 od.led \
 od.localfax\
 od.modem \
 od.module \
 o.modulehdr \
 od.pio_reg \
 od.watchdog \
 od.wimpmsg

OBJSSA = \
 o.bufman \
 o.bufmanlo \
 o.buzzer \
 o.eeprom\
 o.error\
 o.fakeexch \
 o.fakeexchlo \
 o.interrupts \
 o.led \
 o.localfax\
 o.modem \
 o.module \
 o.modulehdr \
 o.pio_reg \
 o.watchdog \
 o.wimpmsg

HDRS = h.modulehdr

LIBS = <Lib$Dir>.callx.o.callx ${EXTRALIBS}

LIBSD =\
 <Lib$Dir>.callx.o.callx\
 <Lib$Dir>.Trace.o.T_M_PL\
 <Lib$Dir>.DebugLib.o.debuglibzm\
 TCPIPLibs:o.inetlibzm\
 TCPIPLibs:o.socklib5zm\
 TCPIPLibs:o.unixlibzm\

# ------------------------------------------------------------------------------
# Rule patterns
#
.SUFFIXES:	.o .od .oSA .h .s .c .cmhg
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.oSA:;	${CC} ${CFLAGS} ${DFLAGS} -DSTANDALONE -o $@ $<
.c.od:;		${CC} ${CFLAGS} -fn ${DDFLAGS} -DSTANDALONE -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.s.od:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} $< -o $@
.cmhg.h:;	${CMHG} ${CMHGFLAGS} $< -d $@


# ------------------------------------------------------------------------------
# Main rules
#

ram: ${RAMTARGET}
	@echo ${COMPONENT}: all complete

rom: ${ROMTARGET}
	@echo ${COMPONENT}: rom module built

debug: ${DEBUGTARGET}
	@echo ${COMPONENT}: debug build complete

export: ${EXPORTS}
	@echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
	${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${RM} ${HDRS}
	${WIPE} o.* ${WFLAGS}
	${WIPE} oSA.* ${WFLAGS}
	${RM} linked.${COMPONENT}
	${RM} map.${COMPONENT}
	${RM} ${RAMTARGET}
	${RM} ${ROMTARGET}
	${WIPE} aof.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
	@echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

o.dirs:
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} o
	@${MKDIR} od
	@${MKDIR} oSA
	@${MKDIR} rm
	create o.dirs
	stamp o.dirs


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#


# Target 

${RAMTARGET}: ${OBJSSA} ${HDRS} ${LIBS} ${CLIB} o.dirs
	${LD} -rmf -symbols @.syms -o $@ ${OBJSSA} ${LIBS} ${CLIB}

${DEBUGTARGET}: ${OBJSD} ${HDRS} ${LIBSD} ${CLIB} o.dirs
	${LD} -rmf -symbols @.syms -o $@ ${OBJSD} ${LIBSD} ${CLIB}

o.res: Resources.<Locale>.Messages
	${RESGEN} resources_area o.res -via Res.List

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}


# ROM Target (re-linked at ROM Image build time)

${ROMTARGET}: ${OBJS} ${HDRS} ${LIBS} ${ROMCSTUBS} o.dirs
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

# Final link for the ROM Image (using given base address)

rom_link:
	${MKDIR} linked
	${MKDIR} map
	${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Dynamic dependencies:
