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
# 1998-10-16	BAL	Removed -zps1 from suffix rules for module-wrapped
#                       builds.  Module-wrapped apps. run in User mode and
#                       so they *can* extend the stack if necessary.

# -----------------------------------------------------------------------------
# Paths
#
EXP_C_H = <Cexport$dir>.h

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= NCUpgrade
APP		= !NCUpgrade
MODULE		= rm.!NCUpgrade
ROM_MODULE	= aof.NCUpgrade
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
CC		= ${MemCheckCC} cc
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
DDFLAGS		= -fn -DDEBUGLIB -DDEBUG
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
DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglib
#MEMCHECKLIB	= ${LIBDIR}.MemCheck.o.MC

INETLIB		= TCPIPLibs:o.inetlib
SOCKLIB		= TCPIPLibs:o.socklib
UNIXLIB		= TCPIPLibs:o.unixlib

LIBSM = \
 ${MSGLIB}zm \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${TOOLACTIONLIB} \
 ${ZLIB}.o.zlibzm \
 ${SOCKLIB}zm \
 ${INETLIB}zm \


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


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,TCPIPLibs:

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
 o.ui_main \
 o.leds \

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
 oz.leds \

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
 od.leds \
  
#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.oz:;		${CC} ${CFLAGS} ${DZFLAGS} ${EXTRAFLAGS} -c -zM -DROM -o $@ $<

.c.od:;		${CC} ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.odz:;	${CC} ${CFLAGS} ${DDZFLAGS} ${EXTRAFLAGS} -c -zM -DROM -o $@ $<

.s.o:;		${AS} ${AFLAGS} $< $@



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
	

install: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	${CP} ${LDIR}.Res		${INSTDIR}.Res		${CPFLAGS}
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
	${CP} ${LDIR}.ResT		${INSTDIR}.Res		${CPFLAGS}
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
	${CP} ${LDIR}.ROM.!Boot		${RESAPP}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESAPP}.!Sprites	${CPFLAGS}
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
	@${MKDIR} Targets.${APP}
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

#s.AppName: ${LDIR}.Messages
#	awk -f awk.AppName ${LDIR}.Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${EXTRALIBS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS} ${EXTRALIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} ${EXTRALIBS} o.dirs
	@echo
	${LD} -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD} ${EXTRALIBS}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${CLIB} o.dirs
	@echo
	${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} ${EXTRALIBS} o.dirs
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${ROMCSTUBS}

rm.!GUID: odz.ModuleWrap ${OBJSDZ} ${LIBSM} ${EXTRALIBS} o.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM} ${EXTRALIBS}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
