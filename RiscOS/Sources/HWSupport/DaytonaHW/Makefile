# Makefile for DaytonaHW
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-07-18	BAL		Created
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
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE		= -wipe

ASMFLAGS	= -depend !Depend -Stamp -quit -throwback
CFLAGS		= -c -depend !Depend -strict -wp -zM -zps1 -ffah -throwback ${INCLUDES} ${DFLAGS}
CMHGFLAGS	= -depend !Depend -p -throwback ${DFLAGS}
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

DFLAGS		= #-DDEBUG #-DTEST


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
EXPORTS		= ${EXP_C_H}.${COMPONENT}

OBJS = \
 o.bufman \
 o.bufmanlo \
 o.buzzer \
 o.callfax \
 o.eeprom\
 o.error\
 o.fakeexch \
 o.fakeexchlo \
 o.faxreceivr \
 o.interrupts \
 o.led \
 o.modem \
 o.module \
 o.modulehdr \
 o.pio_reg \
 o.watchdog \
 o.wimpmsg

OBJSSA = \
 o.bufman \
 o.bufmanlo \
 o.buzzer \
 o.callfax \
 o.eeprom\
 o.error\
 o.fakeexch \
 o.fakeexchlo \
 o.faxreceivr \
 o.interrupts \
 o.led \
 o.modem \
 o.module \
 o.modulehdr \
 o.pio_reg \
 o.watchdog \
 o.wimpmsg

HDRS = h.modulehdr

LIBS = <Lib$Dir>.callx.o.callx


# ------------------------------------------------------------------------------
# Rule patterns
#
.SUFFIXES:	.o .oSA .h .s .c .cmhg
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.c.oSA:;	${CC} ${CFLAGS} -DSTANDALONE -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} $< -o $@
.cmhg.h:;	${CMHG} ${CMHGFLAGS} $< -d $@


# ------------------------------------------------------------------------------
# Main rules
#

ram: ${RAMTARGET}
	@echo ${COMPONENT}: all complete

rom: ${ROMTARGET}
	@echo ${COMPONENT}: rom module built

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

dirs:
	${MKDIR} aof
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} o
	${MKDIR} oSA
	${MKDIR} rm


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#


# Target 

${RAMTARGET}: ${OBJSSA} ${HDRS} ${CLIB} dirs
	${LD} -rmf -symbols @.syms -o $@ ${OBJSSA} ${LIBS} ${CLIB}


#	${MODSQZ} $@

o.res: Resources.<Locale>.Messages
	${RESGEN} resources_area o.res -via Res.List


# ROM Target (re-linked at ROM Image build time)

${ROMTARGET}: ${OBJS} ${HDRS} ${ROMCSTUBS} dirs
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}


${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

# Final link for the ROM Image (using given base address)

rom_link:
	${MKDIR} linked
	${MKDIR} map
	${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Dynamic dependencies:
