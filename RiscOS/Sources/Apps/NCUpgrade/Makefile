# Makefile for Funai2 Maintenence App
# ===================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 14-Jul-1998   SR      Created
# 14-Aug-1998   JPB	Tided up, and made MemCheck builds possible

# -----------------------------------------------------------------------------
# Paths
#
EXP_C_H = <Cexport$dir>.h

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Upgrade
APP		= !Upgrade
MODULE		= rm.!Upgrade
ROM_MODULE	= aof.Upgrade
LDIR		= Resources.${LOCALE}
EXPORTS		= ${EXP_C_H}.${COMPONENT}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = ${RESDIR}.${COMPONENT}
RESAPP          = <Resource$Dir>.Apps.${APP}

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= cc #${MemCheckCC} cc
CMHG		= cmhg
CP		= copy
LD		= link #${MemCheckLD} link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= -depend !Depend -nocache -stamp -quit ${THROWBACK}
CFLAGS		= -depend !Depend -ffah ${INCLUDES} -wp ${THROWBACK}
CPFLAGS		= ~cfr~v~n
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS		= 
DDFLAGS		= -fn -DDEBUGLIB
DZFLAGS		= 
DDZFLAGS	= -fn -DDEBUGLIB


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>

CLIB		= C:o.stubs
CPPLIB		= CPPLib:o.c++lib
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
TOOLACTIONLIB   = <CExport$Dir>.o.Toolaction

ZLIB		= ${LIBDIR}.zlib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
#DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglib
#MEMCHECKLIB	= MemCheck:o.MC

INETLIB		= TCPIPLibs:o.inetlibzm
SOCKLIB		= TCPIPLibs:o.socklibzm
UNIXLIB		= TCPIPLibs:o.unixlibzm

LIBSM = \
 ${MSGLIB}zm \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${TOOLACTIONLIB} \
 ${MEMCHECKLIB} \


LIBS = \
 ${MSGLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${TOOLACTIONLIB} \
 ${CLIB} \
 ${ZLIB}.o.zlib \
 ${SOCKLIB} \
 ${INETLIB} \
 ${MEMCHECKLIB} \


LIBSD = \
 ${MSGLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${TOOLACTIONLIB} \
 ${CLIB} \
 ${ZLIB}.o.zlib \
 ${DEBUGLIB} \
 ${SOCKLIB} \
 ${INETLIB} \
 ${MEMCHECKLIB} \


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,TCPIPLibs:,TCPIPLibs:sys,TCPIPLibs:net,${ZLIB}

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${TARGET} \
 ${UNIXLIB} \
 ${INETLIB} \
 ${SOCKLIB} \

# Include these in the ROM module
RESFILES = \
 ${LDIR}.!Sprites \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.main \
 o.strings \
 o.ui_menu \
 o.ui_misc \
 o.dial \
 o.error \
 o.ftp \
 o.flashstubs \
 o.nvram \
 o.ppp \
 o.progrmng \
 o.ui_main 

OBJSZ = \
 oz.main \
 oz.strings \
 oz.ui_main \
 oz.ui_menu \
 oz.ui_misc \
 oz.dial \
 oz.error \
 oz.flashstubs \
 oz.ftp \
 oz.nvram \
 oz.ppp \
 oz.progrmng \

#OBJSZ =

OBJSD = \
 od.main \
 od.strings \
 od.ui_main \
 od.ui_menu \
 od.ui_misc \
 od.dial \
 od.error \
 od.flashstubs \
 od.ftp \
 od.nvram \
 od.ppp \
 od.progrmng \
  
#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.oz:;		@echo
		@echo Compiling $<
		@echo ${DZFLAGS}
		@${CC} ${CFLAGS} ${DZFLAGS} ${EXTRAFLAGS} -c -zM -zps1 -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.odz:;	@echo
		@echo Compiling $<
		@echo ${DDZFLAGS}
		@${CC} ${CFLAGS} ${DDZFLAGS} ${EXTRAFLAGS} -c -zM -zps1 -DROM -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${AFLAGS}
		@${AS} ${AFLAGS} $< $@



# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)
	
export: ${EXPORTS}
	@echo ${COMPONENT}: export complete

install_common: ${FILES}
	@echo
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}

install: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
	-Access ${INSTDIR}.!Boot	lr/r
	-Access ${INSTDIR}.!Run		lr/r
	-Access ${INSTDIR}.!RunImage	lr/r
	-Access ${INSTDIR}.Messages	lr/r
	-Access ${INSTDIR}.Res		lr/r
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${LDIR}.!RunT		${INSTDIR}.!RunT	${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImageT	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Debug application installed ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}.User
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESDIR}.!Sprites	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	@echo starting
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	map		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	od		${WFLAGS}
	${WIPE}	oz		${WFLAGS}
	${WIPE}	odz		${WFLAGS}
	${WIPE}	rm		${WFLAGS}
	${WIPE} syms		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	o.dirs
	@echo
	@echo ${COMPONENT}: Cleaned

o.dirs:
	@${MKDIR} abs
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} o
	@${MKDIR} od
	@${MKDIR} oz
	@${MKDIR} odz
	@${MKDIR} rm
	@${MKDIR} syms
	@${MKDIR} Targets
	@${MKDIR} Targets.!Upgrade
	create o.dirs
	stamp  o.dirs


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
	${WIPE}	Targets		${WFLAGS}
	@echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

rom_link:
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
	awk -f awk.AppName ${LDIR}.Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
	@echo
	${LD} -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
	@echo
	${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.!GUID: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
