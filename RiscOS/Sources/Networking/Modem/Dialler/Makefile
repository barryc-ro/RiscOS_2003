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
LIBS		= <Lib$Dir>.svcprint.o.svcprint <Lib$Dir>.Trace.o.T_M_PL


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
.SUFFIXES:	.o .od .o_old .od_old .h .c .cmhg
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}		-c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}		-c -zM -zps1 -fn -o $@ $<
.c.o_old:;	${CC} ${CFLAGS} ${DFLAGS_OLD}		-c -zM -zps1 -o $@ $<
.c.od_old:;	${CC} ${CFLAGS} ${DDFLAGS_OLD}		-c -zM -zps1 -fn -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}		-o $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS}		-o $@ $<
.cmhg.o_old:;	${CMHG} ${CMHGFLAGS} ${DFLAGS_OLD}	-o $@ $<
.cmhg.od_old:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS_OLD}	-o $@ $<

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
o_old.chat:	c.chat
o_old.chat:	C:h.kernel
o_old.chat:	C:h.swis
o_old.chat:	OS:h.osargs
o_old.chat:	OS:h.types
o_old.chat:	OS:h.os
o_old.chat:	OS:h.fileswitch
o_old.chat:	OS:h.osgbpb
o_old.chat:	h.bool
o_old.chat:	OS:h.types
o_old.chat:	h.chat
o_old.chat:	C:h.kernel
o_old.chat:	h.dialler
o_old.chat:	h.error
o_old.chat:	C:h.kernel
o_old.chat:	h.log
o_old.chat:	h.flags
o_old.chat:	h.handlers
o_old.chat:	C:h.kernel
o_old.chat:	h.header
o_old.chat:	h.modhdr
o_old.chat:	h.list
o_old.chat:	h.log
o_old.chat:	h.module
o_old.chat:	C:h.kernel
o_old.chat:	h.script
o_old.chat:	C:h.kernel
o_old.chat:	h.bool
o_old.chat:	h.serial
o_old.chat:	C:h.kernel
o_old.chat:	h.status
o_old.chat:	C:h.kernel
o_old.chat:	h.timeout
o_old.chat:	C:h.kernel
o_old.chat:	h.utils
o_old.chat:	C:h.kernel
o_old.chat:	h.driver
o_old.dialler:	c.dialler
o_old.dialler:	C:h.kernel
o_old.dialler:	C:h.swis
o_old.dialler:	OS:h.osfind
o_old.dialler:	OS:h.types
o_old.dialler:	OS:h.os
o_old.dialler:	OS:h.fileswitch
o_old.dialler:	h.bool
o_old.dialler:	OS:h.types
o_old.dialler:	h.chat
o_old.dialler:	C:h.kernel
o_old.dialler:	h.dialler
o_old.dialler:	h.error
o_old.dialler:	C:h.kernel
o_old.dialler:	h.log
o_old.dialler:	h.flags
o_old.dialler:	h.handlers
o_old.dialler:	C:h.kernel
o_old.dialler:	h.header
o_old.dialler:	h.modhdr
o_old.dialler:	h.ioctl
o_old.dialler:	C:h.kernel
o_old.dialler:	h.log
o_old.dialler:	h.module
o_old.dialler:	C:h.kernel
o_old.dialler:	h.script
o_old.dialler:	C:h.kernel
o_old.dialler:	h.bool
o_old.dialler:	h.serial
o_old.dialler:	C:h.kernel
o_old.dialler:	h.status
o_old.dialler:	C:h.kernel
o_old.dialler:	h.timeout
o_old.dialler:	C:h.kernel
o_old.dialler:	h.utils
o_old.dialler:	C:h.kernel
o_old.dialler:	h.driver
o_old.error:	c.error
o_old.error:	C:h.kernel
o_old.error:	h.error
o_old.error:	C:h.kernel
o_old.error:	h.log
o_old.handlers:	c.handlers
o_old.handlers:	C:h.kernel
o_old.handlers:	C:h.swis
o_old.handlers:	OS:h.os
o_old.handlers:	OS:h.types
o_old.handlers:	OS:h.osbyte
o_old.handlers:	OS:h.osserialop
o_old.handlers:	h.bool
o_old.handlers:	OS:h.types
o_old.handlers:	h.chat
o_old.handlers:	C:h.kernel
o_old.handlers:	h.dialler
o_old.handlers:	h.error
o_old.handlers:	C:h.kernel
o_old.handlers:	h.log
o_old.handlers:	h.flags
o_old.handlers:	h.handlers
o_old.handlers:	C:h.kernel
o_old.handlers:	h.header
o_old.handlers:	h.modhdr
o_old.handlers:	h.log
o_old.handlers:	h.module
o_old.handlers:	C:h.kernel
o_old.handlers:	h.script
o_old.handlers:	C:h.kernel
o_old.handlers:	h.bool
o_old.handlers:	h.serial
o_old.handlers:	C:h.kernel
o_old.handlers:	h.status
o_old.handlers:	C:h.kernel
o_old.handlers:	h.timeout
o_old.handlers:	C:h.kernel
o_old.handlers:	h.driver
o_old.list:	c.list
o_old.list:	h.list
o_old.log:	c.log
o_old.log:	C:h.swis
o_old.log:	C:h.kernel
o_old.log:	h.flags
o_old.log:	h.log
o_old.module:	c.module
o_old.module:	C:h.kernel
o_old.module:	C:h.swis
o_old.module:	h.chat
o_old.module:	C:h.kernel
o_old.module:	h.dialler
o_old.module:	h.error
o_old.module:	C:h.kernel
o_old.module:	h.log
o_old.module:	h.flags
o_old.module:	h.handlers
o_old.module:	C:h.kernel
o_old.module:	h.header
o_old.module:	h.modhdr
o_old.module:	h.log
o_old.module:	h.module
o_old.module:	C:h.kernel
o_old.module:	h.script
o_old.module:	C:h.kernel
o_old.module:	h.bool
o_old.module:	OS:h.types
o_old.module:	h.serial
o_old.module:	C:h.kernel
o_old.module:	h.status
o_old.module:	C:h.kernel
o_old.module:	h.swidefs
o_old.module:	h.header
o_old.module:	h.timeout
o_old.module:	C:h.kernel
o_old.module:	h.driver
o_old.serial:	c.serial
o_old.serial:	C:h.kernel
o_old.serial:	C:h.swis
o_old.serial:	OS:h.osargs
o_old.serial:	OS:h.types
o_old.serial:	OS:h.os
o_old.serial:	OS:h.fileswitch
o_old.serial:	OS:h.osgbpb
o_old.serial:	OS:h.osfile
o_old.serial:	OS:h.osfind
o_old.serial:	h.bool
o_old.serial:	OS:h.types
o_old.serial:	h.dialler
o_old.serial:	h.error
o_old.serial:	C:h.kernel
o_old.serial:	h.log
o_old.serial:	h.flags
o_old.serial:	h.header
o_old.serial:	h.modhdr
o_old.serial:	h.log
o_old.serial:	h.module
o_old.serial:	C:h.kernel
o_old.serial:	h.script
o_old.serial:	C:h.kernel
o_old.serial:	h.bool
o_old.serial:	h.serial
o_old.serial:	C:h.kernel
o_old.serial:	h.utils
o_old.serial:	C:h.kernel
o_old.serial:	h.driver
o_old.status:	c.status
o_old.status:	C:h.kernel
o_old.status:	C:h.swis
o_old.status:	h.dialler
o_old.status:	h.flags
o_old.status:	h.log
o_old.status:	h.script
o_old.status:	C:h.kernel
o_old.status:	h.bool
o_old.status:	OS:h.types
o_old.status:	h.status
o_old.status:	C:h.kernel
o_old.status:	h.utils
o_old.status:	C:h.kernel
o_old.timeout:	c.timeout
o_old.timeout:	C:h.kernel
o_old.timeout:	C:h.swis
o_old.timeout:	h.bool
o_old.timeout:	OS:h.types
o_old.timeout:	h.chat
o_old.timeout:	C:h.kernel
o_old.timeout:	h.dialler
o_old.timeout:	h.error
o_old.timeout:	C:h.kernel
o_old.timeout:	h.log
o_old.timeout:	h.flags
o_old.timeout:	h.handlers
o_old.timeout:	C:h.kernel
o_old.timeout:	h.header
o_old.timeout:	h.modhdr
o_old.timeout:	h.log
o_old.timeout:	h.module
o_old.timeout:	C:h.kernel
o_old.timeout:	h.status
o_old.timeout:	C:h.kernel
o_old.timeout:	h.swidefs
o_old.timeout:	h.header
o_old.timeout:	h.timeout
o_old.timeout:	C:h.kernel
o_old.utils:	c.utils
o_old.utils:	C:h.kernel
o_old.utils:	C:h.swis
o_old.utils:	OS:h.fileswitch
o_old.utils:	OS:h.types
o_old.utils:	OS:h.os
o_old.utils:	OS:h.osfile
o_old.utils:	h.error
o_old.utils:	C:h.kernel
o_old.utils:	h.log
o_old.utils:	h.handlers
o_old.utils:	C:h.kernel
o_old.utils:	h.ioctl
o_old.utils:	C:h.kernel
o_old.utils:	h.log
o_old.utils:	h.utils
o_old.utils:	C:h.kernel
o.error:	c.error
o.error:	C:h.kernel
o.error:	h.error
o.error:	C:h.kernel
o.error:	h.log
o.list:	c.list
o.list:	h.list
o.log:	c.log
o.log:	C:h.swis
o.log:	C:h.kernel
o.log:	h.flags
o.log:	h.log
o.status:	c.status
o.status:	C:h.kernel
o.status:	C:h.swis
o.status:	h.dialler
o.status:	h.flags
o.status:	h.log
o.status:	h.script
o.status:	C:h.kernel
o.status:	h.bool
o.status:	OS:h.types
o.status:	h.status
o.status:	C:h.kernel
o.status:	h.utils
o.status:	C:h.kernel
o.utils:	c.utils
o.utils:	C:h.kernel
o.utils:	C:h.swis
o.utils:	OS:h.fileswitch
o.utils:	OS:h.types
o.utils:	OS:h.os
o.utils:	OS:h.osfile
o.utils:	h.error
o.utils:	C:h.kernel
o.utils:	h.log
o.utils:	h.handlers
o.utils:	C:h.kernel
o.utils:	h.ioctl
o.utils:	C:h.kernel
o.utils:	h.log
o.utils:	h.utils
o.utils:	C:h.kernel
o.chat:	c.chat
o.chat:	C:h.kernel
o.chat:	C:h.swis
o.chat:	OS:h.osargs
o.chat:	OS:h.types
o.chat:	OS:h.os
o.chat:	OS:h.fileswitch
o.chat:	OS:h.osgbpb
o.chat:	h.bool
o.chat:	OS:h.types
o.chat:	h.chat
o.chat:	C:h.kernel
o.chat:	h.dialler
o.chat:	h.error
o.chat:	C:h.kernel
o.chat:	h.log
o.chat:	h.flags
o.chat:	h.handlers
o.chat:	C:h.kernel
o.chat:	h.header
o.chat:	h.modhdrb
o.chat:	h.list
o.chat:	h.log
o.chat:	h.module
o.chat:	C:h.kernel
o.chat:	h.script
o.chat:	C:h.kernel
o.chat:	h.bool
o.chat:	h.serial
o.chat:	C:h.kernel
o.chat:	h.status
o.chat:	C:h.kernel
o.chat:	h.timeout
o.chat:	C:h.kernel
o.chat:	h.utils
o.chat:	C:h.kernel
o.chat:	h.driver
o.dialler:	c.dialler
o.dialler:	C:h.kernel
o.dialler:	C:h.swis
o.dialler:	OS:h.osfind
o.dialler:	OS:h.types
o.dialler:	OS:h.os
o.dialler:	OS:h.fileswitch
o.dialler:	h.bool
o.dialler:	OS:h.types
o.dialler:	h.chat
o.dialler:	C:h.kernel
o.dialler:	h.dialler
o.dialler:	h.error
o.dialler:	C:h.kernel
o.dialler:	h.log
o.dialler:	h.flags
o.dialler:	h.handlers
o.dialler:	C:h.kernel
o.dialler:	h.header
o.dialler:	h.modhdrb
o.dialler:	h.ioctl
o.dialler:	C:h.kernel
o.dialler:	h.log
o.dialler:	h.module
o.dialler:	C:h.kernel
o.dialler:	h.script
o.dialler:	C:h.kernel
o.dialler:	h.bool
o.dialler:	h.serial
o.dialler:	C:h.kernel
o.dialler:	h.status
o.dialler:	C:h.kernel
o.dialler:	h.timeout
o.dialler:	C:h.kernel
o.dialler:	h.utils
o.dialler:	C:h.kernel
o.dialler:	h.driver
o.handlers:	c.handlers
o.handlers:	C:h.kernel
o.handlers:	C:h.swis
o.handlers:	OS:h.os
o.handlers:	OS:h.types
o.handlers:	OS:h.osbyte
o.handlers:	OS:h.osserialop
o.handlers:	h.bool
o.handlers:	OS:h.types
o.handlers:	h.chat
o.handlers:	C:h.kernel
o.handlers:	h.dialler
o.handlers:	h.error
o.handlers:	C:h.kernel
o.handlers:	h.log
o.handlers:	h.flags
o.handlers:	h.handlers
o.handlers:	C:h.kernel
o.handlers:	h.header
o.handlers:	h.modhdrb
o.handlers:	h.log
o.handlers:	h.module
o.handlers:	C:h.kernel
o.handlers:	h.script
o.handlers:	C:h.kernel
o.handlers:	h.bool
o.handlers:	h.serial
o.handlers:	C:h.kernel
o.handlers:	h.status
o.handlers:	C:h.kernel
o.handlers:	h.timeout
o.handlers:	C:h.kernel
o.handlers:	h.driver
o.modhdr:	cmhg.modhdr
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.chat
o.module:	C:h.kernel
o.module:	h.dialler
o.module:	h.error
o.module:	C:h.kernel
o.module:	h.log
o.module:	h.flags
o.module:	h.handlers
o.module:	C:h.kernel
o.module:	h.header
o.module:	h.modhdrb
o.module:	h.log
o.module:	h.module
o.module:	C:h.kernel
o.module:	h.script
o.module:	C:h.kernel
o.module:	h.bool
o.module:	OS:h.types
o.module:	h.serial
o.module:	C:h.kernel
o.module:	h.status
o.module:	C:h.kernel
o.module:	h.swidefs
o.module:	h.header
o.module:	h.timeout
o.module:	C:h.kernel
o.module:	h.driver
o.serial:	c.serial
o.serial:	C:h.kernel
o.serial:	C:h.swis
o.serial:	OS:h.osargs
o.serial:	OS:h.types
o.serial:	OS:h.os
o.serial:	OS:h.fileswitch
o.serial:	OS:h.osgbpb
o.serial:	OS:h.osfile
o.serial:	OS:h.osfind
o.serial:	h.bool
o.serial:	OS:h.types
o.serial:	h.dialler
o.serial:	h.error
o.serial:	C:h.kernel
o.serial:	h.log
o.serial:	h.flags
o.serial:	h.header
o.serial:	h.modhdrb
o.serial:	h.log
o.serial:	h.module
o.serial:	C:h.kernel
o.serial:	h.script
o.serial:	C:h.kernel
o.serial:	h.bool
o.serial:	h.serial
o.serial:	C:h.kernel
o.serial:	h.utils
o.serial:	C:h.kernel
o.serial:	h.driver
o.timeout:	c.timeout
o.timeout:	C:h.kernel
o.timeout:	C:h.swis
o.timeout:	h.bool
o.timeout:	OS:h.types
o.timeout:	h.chat
o.timeout:	C:h.kernel
o.timeout:	h.dialler
o.timeout:	h.error
o.timeout:	C:h.kernel
o.timeout:	h.log
o.timeout:	h.flags
o.timeout:	h.handlers
o.timeout:	C:h.kernel
o.timeout:	h.header
o.timeout:	h.modhdrb
o.timeout:	h.log
o.timeout:	h.module
o.timeout:	C:h.kernel
o.timeout:	h.status
o.timeout:	C:h.kernel
o.timeout:	h.swidefs
o.timeout:	h.header
o.timeout:	h.timeout
o.timeout:	C:h.kernel
o.modhdr:	cmhg.modhdr
