# Makefile for Dialler
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1997-06-23	BAL	Created
# 1998-02-12	BAL	Build Dialler and DiallerB from the same sources
#

# ------------------------------------------------------------------------------
# Paths
#
EXP_HDR	= <export$dir>
EXP_C_H	= <Cexport$dir>.h
EXP_C_O	= <Cexport$dir>.o


# ------------------------------------------------------------------------------
# Generic options
#
MKDIR	= cdir
AS	= objasm
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
MODSQZ	= modsqz
WIPE	= -wipe

ASMFLAGS	= -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS		= -depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -strict -ffah 
CMHGFLAGS	= -depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -p
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~vr

DFLAGS_COMMON	= -D${SYSTEM}
DFLAGS		= ${DFLAGS_COMMON} -DNDEBUG
DFLAGS_OLD	= ${DFLAGS_COMMON} -DNDEBUG -DOLD_DIALLER
DDFLAGS		= ${DFLAGS_COMMON} -DDEBUG -DDEBUG_TML -DTrace_TRACE
DDFLAGS_OLD	= ${DFLAGS_COMMON} -DDEBUG -DDEBUG_TML -DTrace_TRACE -DOLD_DIALLER


# ------------------------------------------------------------------------------
# Libraries
#
CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_AbsSym
LIBS		= #<Lib$Dir>.svcprint.o.svcprint <Lib$Dir>.Trace.o.T_M_PL


# ------------------------------------------------------------------------------
# Include files
#
INCLUDES	= -ITCPIPLibs,C:


# ------------------------------------------------------------------------------
# Program specific options
#
COMPONENT	= Dialler
RAMTARGET	= rm.Dialler
ROMTARGET	= aof.Dialler
EXPORTS		= ${EXP_C_H}.${COMPONENT}

OBJS =\
 o.chat\
 o.dialler\
 o.error\
 o.handlers\
 o.interrupts\
 o.list\
 o.log\
 o.modhdr\
 o.module\
 o.serial\
 o.status\
 o.timeout\
 o.utils

OBJSD =\
 od.chat\
 od.dialler\
 od.error\
 od.handlers\
 od.interrupts\
 od.list\
 od.log\
 od.module\
 od.modhdr\
 od.serial\
 od.status\
 od.timeout\
 od.utils

OLDOBJS =\
 o_old.chat\
 o_old.dialler\
 o_old.error\
 o_old.handlers\
 o.interrupts\
 o_old.list\
 o_old.log\
 o_old.modhdr\
 o_old.module\
 o_old.serial\
 o_old.status\
 o_old.timeout\
 o_old.utils

OLDOBJSD =\
 od_old.chat\
 od_old.dialler\
 od_old.error\
 od_old.handlers\
 od.interrupts\
 od_old.list\
 od_old.log\
 od_old.modhdr\
 od_old.module\
 od_old.serial\
 od_old.status\
 od_old.timeout\
 od_old.utils

HDR	= h.modhdrb

OLDHDR	= h.modhdr

# ------------------------------------------------------------------------------
# Rule patterns
#
.SUFFIXES:	.o .od .o_old .od_old .h .c .cmhg .s
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}		-c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}		-c -zM -zps1 -fn -o $@ $<
.c.o_old:;	${CC} ${CFLAGS} ${DFLAGS_OLD}		-c -zM -zps1 -o $@ $<
.c.od_old:;	${CC} ${CFLAGS} ${DDFLAGS_OLD}		-c -zM -zps1 -fn -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}		-o $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS}		-o $@ $<
.cmhg.o_old:;	${CMHG} ${CMHGFLAGS} ${DFLAGS_OLD}	-o $@ $<
.cmhg.od_old:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS_OLD}	-o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.s.od:;		$(AS) ${ASMFLAGS} -o $@ $<

h.modhdrb: cmhg.modhdr
	${CMHG} ${CMHGFLAGS} ${DFLAGS} -d $@ cmhg.modhdr

h.modhdr: cmhg.modhdr
	${CMHG} ${CMHGFLAGS} ${DFLAGS} -DOLD_DIALLER -d $@ cmhg.modhdr


# ------------------------------------------------------------------------------
# Rules
#
all: ram_ ram_old

ram: ram_${TYPE}
	@echo ${COMPONENT}: ram module build

rom: rom_${TYPE}
	@echo ${COMPONENT}: rom module built

debug: rm.DiallerD
	@echo ${COMPONENT}: all complete

export: ${EXPORTS}
	@echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
	${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

resources:
	@echo ${COMPONENT}: resource files copied

clean:
	${RM} ${HDR}
	${WIPE} aof.* ${WFLAGS}
	${WIPE} linked.* ${WFLAGS}
	${WIPE} map.* ${WFLAGS}
	${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	${WIPE} o_old.* ${WFLAGS}
	${WIPE} od_old.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
	${RM} ${HDR}
	${RM} ${OLDHDR}
	${RM} ${RAMTARGET}
	${RM} ${RAMTARGET}B
	${RM} ${ROMTARGET}
	${RM} ${ROMTARGET}B
	@echo ${COMPONENT}: cleaned

#
# RAM Target 
#
ram_old: ${OLDOBJS} ${OLDHDR} ${LIBS} ${CLIB}
	${LD} -rmf -o ${RAMTARGET} ${OLDOBJS} ${LIBS} ${CLIB}
	${MODSQZ} ${RAMTARGET}

ram_ ram_new: ${OBJS} ${HDR} ${LIBS} ${CLIB}
	${LD} -rmf -o ${RAMTARGET}B ${OBJS} ${LIBS} ${CLIB}
	${MODSQZ} ${RAMTARGET}B

#
# ROM Target (re-linked at ROM Image build time)
#
rom_old: ${OLDOBJS} ${OLDHDR} ${LIBS} ${ROMCSTUBS}
	${LD} -o ${ROMTARGET} -aof ${OLDOBJS} ${LIBS} ${ROMCSTUBS}

rom_ rom_new: ${OBJS} ${HDR} ${LIBS} ${ROMCSTUBS}
	${LD} -o ${ROMTARGET}B -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link: rom_link_${TYPE}

rom_link_old:
	${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

rom_link_ rom_link_new:
	${LD} -o linked.${COMPONENT}B -map -bin -base ${ADDRESS} ${ROMTARGET}B ${ABSSYM} > map.${COMPONENT}B
	truncate map.${COMPONENT}B linked.${COMPONENT}B
	${CP} linked.${COMPONENT}B ${LINKDIR}.${COMPONENT}B ${CPFLAGS}
	@echo ${COMPONENT}B: rom_link complete

${EXP_C_H}.${COMPONENT}: h.${COMPONENT}
	${CP} h.${COMPONENT} $@ ${CPFLAGS}

rm.DiallerD: ${OBJSD} ${HDR} ${CLIB}
	${LD} -rmf -o $@ ${OBJSD} ${LIBS} ${CLIB}

# Dynamic dependencies:
