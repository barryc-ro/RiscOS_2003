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
DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglib
MEMCHECKLIB	= MemCheck:o.MC

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
od.main:	c.main
od.main:	C:h.string
od.main:	C:h.stdio
od.main:	C:h.stdlib
od.main:	C:h.swis
od.main:	C:h.kernel
od.main:	C:h.kernel
od.main:	tbox:h.event
od.main:	tbox:h.wimp
od.main:	tbox:h.toolbox
od.main:	C:debuglib.h.DebugLib
od.main:	MemCheck:h.MemCheck
od.main:	C:h.stddef
od.main:	C:h.stdio
od.main:	C:h.stdarg
od.main:	h.ui
od.main:	h.errors
od.main:	h.main
od.main:	C:h.kernel
od.main:	h.ui
od.main:	tbox:h.toolbox
od.main:	C:h.stdio
od.main:	h.nvram
od.main:	h.dial
od.main:	h.ppp
od.main:	h.ftp
od.main:	h.progrmng
od.main:	<Lib$Dir>.zlib.h.zlib
od.main:	<Lib$Dir>.zlib.h.zconf
od.strings:	c.strings
od.strings:	C:h.string
od.strings:	C:h.stdlib
od.strings:	C:h.ctype
od.strings:	C:h.stdio
od.strings:	h.strings
od.ui_main:	c.ui_main
od.ui_main:	C:h.time
od.ui_main:	h.ui_include
od.ui_main:	C:h.stdio
od.ui_main:	C:h.stdlib
od.ui_main:	C:h.string
od.ui_main:	C:h.swis
od.ui_main:	C:h.kernel
od.ui_main:	h.ui_tbox
od.ui_main:	tbox:h.event
od.ui_main:	tbox:h.wimp
od.ui_main:	tbox:h.toolbox
od.ui_main:	tbox:h.toolbox
od.ui_main:	tbox:h.window
od.ui_main:	tbox:h.gadgets
od.ui_main:	tbox:h.gadgets
od.ui_main:	tbox:h.menu
od.ui_main:	tbox:h.proginfo
od.ui_main:	tbox:h.iconbar
od.ui_main:	tbox:h.quit
od.ui_main:	tbox:h.wimplib
od.ui_main:	C:msglib.h.MsgLib
od.ui_main:	C:debuglib.h.DebugLib
od.ui_main:	h.ui_menu
od.ui_main:	h.ui_misc
od.ui_main:	h.ui_main
od.ui_main:	h.ui_types
od.ui_main:	h.strings
od.ui_main:	h.ui_globals
od.ui_menu:	c.ui_menu
od.ui_menu:	h.ui_include
od.ui_menu:	C:h.stdio
od.ui_menu:	C:h.stdlib
od.ui_menu:	C:h.string
od.ui_menu:	C:h.swis
od.ui_menu:	C:h.kernel
od.ui_menu:	h.ui_tbox
od.ui_menu:	tbox:h.event
od.ui_menu:	tbox:h.wimp
od.ui_menu:	tbox:h.toolbox
od.ui_menu:	tbox:h.toolbox
od.ui_menu:	tbox:h.window
od.ui_menu:	tbox:h.gadgets
od.ui_menu:	tbox:h.gadgets
od.ui_menu:	tbox:h.menu
od.ui_menu:	tbox:h.proginfo
od.ui_menu:	tbox:h.iconbar
od.ui_menu:	tbox:h.quit
od.ui_menu:	tbox:h.wimplib
od.ui_menu:	C:msglib.h.MsgLib
od.ui_menu:	C:debuglib.h.DebugLib
od.ui_menu:	h.ui_menu
od.ui_menu:	h.ui_globals
od.ui_misc:	c.ui_misc
od.ui_misc:	h.ui_include
od.ui_misc:	C:h.stdio
od.ui_misc:	C:h.stdlib
od.ui_misc:	C:h.string
od.ui_misc:	C:h.swis
od.ui_misc:	C:h.kernel
od.ui_misc:	h.ui_tbox
od.ui_misc:	tbox:h.event
od.ui_misc:	tbox:h.wimp
od.ui_misc:	tbox:h.toolbox
od.ui_misc:	tbox:h.toolbox
od.ui_misc:	tbox:h.window
od.ui_misc:	tbox:h.gadgets
od.ui_misc:	tbox:h.gadgets
od.ui_misc:	tbox:h.menu
od.ui_misc:	tbox:h.proginfo
od.ui_misc:	tbox:h.iconbar
od.ui_misc:	tbox:h.quit
od.ui_misc:	tbox:h.wimplib
od.ui_misc:	C:msglib.h.MsgLib
od.ui_misc:	C:debuglib.h.DebugLib
od.ui_misc:	h.ui_misc
od.dial:	c.dial
od.dial:	C:h.stdio
od.dial:	C:h.string
od.dial:	C:h.stdlib
od.dial:	C:h.time
od.dial:	C:h.kernel
od.dial:	C:h.swis
od.dial:	h.ui
od.dial:	h.errors
od.dial:	h.main
od.dial:	C:h.kernel
od.dial:	h.ui
od.dial:	tbox:h.toolbox
od.dial:	tbox:h.wimp
od.dial:	C:h.stdio
od.dial:	h.dial
od.dial:	h.dialler
od.error:	c.error
od.error:	C:h.stdio
od.error:	C:h.stdlib
od.error:	C:h.string
od.error:	C:h.time
od.error:	C:h.swis
od.error:	C:h.kernel
od.error:	C:h.signal
od.error:	C:h.kernel
od.error:	h.ui
od.error:	h.errors
od.error:	h.main
od.error:	C:h.kernel
od.error:	h.ui
od.error:	tbox:h.toolbox
od.error:	tbox:h.wimp
od.error:	C:h.stdio
od.error:	<Lib$Dir>.zlib.h.zlib
od.error:	<Lib$Dir>.zlib.h.zconf
od.error:	h.ftp
od.error:	h.progrmng
od.error:	<Lib$Dir>.zlib.h.zlib
od.error:	h.ppp
od.error:	h.dial
od.error:	h.flashstubs
od.error:	C:h.kernel
od.flashstubs:	c.flashstubs
od.flashstubs:	C:h.kernel
od.flashstubs:	h.ui
od.flashstubs:	h.flashstubs
od.flashstubs:	C:h.kernel
od.flashstubs:	h.main
od.flashstubs:	C:h.kernel
od.flashstubs:	h.ui
od.flashstubs:	tbox:h.toolbox
od.flashstubs:	tbox:h.wimp
od.flashstubs:	C:h.stdio
od.ftp:	c.ftp
od.ftp:	C:h.stdio
od.ftp:	C:h.stdlib
od.ftp:	C:h.time
od.ftp:	C:h.string
od.ftp:	C:h.ctype
od.ftp:	C:h.kernel
od.ftp:	h.ui
od.ftp:	h.main
od.ftp:	C:h.kernel
od.ftp:	h.ui
od.ftp:	tbox:h.toolbox
od.ftp:	tbox:h.wimp
od.ftp:	C:h.stdio
od.ftp:	h.errors
od.ftp:	h.ftp
od.ftp:	h.progrmng
od.ftp:	<Lib$Dir>.zlib.h.zlib
od.ftp:	<Lib$Dir>.zlib.h.zconf
od.ftp:	<Lib$Dir>.zlib.h.zlib
od.ftp:	h.ppp
od.ftp:	C:h.swis
od.nvram:	c.nvram
od.nvram:	C:h.stdio
od.nvram:	C:h.stdlib
od.nvram:	C:h.string
od.nvram:	C:h.math
od.nvram:	C:h.ctype
od.nvram:	C:h.kernel
od.nvram:	h.ui
od.nvram:	h.errors
od.nvram:	h.main
od.nvram:	C:h.kernel
od.nvram:	h.ui
od.nvram:	tbox:h.toolbox
od.nvram:	tbox:h.wimp
od.nvram:	C:h.stdio
od.nvram:	h.nvram
od.nvram:	TCPIPLibs:h.inetlib
od.nvram:	TCPIPLibs:sys.h.types
od.nvram:	TCPIPLibs:sys.h.cdefs
od.nvram:	TCPIPLibs:machine.h.endian
od.nvram:	TCPIPLibs:machine.h.ansi
od.nvram:	TCPIPLibs:machine.h.types
od.nvram:	C:h.time
od.nvram:	TCPIPLibs:netinet.h.in
od.ppp:	c.ppp
od.ppp:	C:h.stdlib
od.ppp:	C:h.time
od.ppp:	C:h.string
od.ppp:	C:h.stdio
od.ppp:	C:h.kernel
od.ppp:	C:h.swis
od.ppp:	h.ppp
od.ppp:	h.ui
od.ppp:	h.errors
od.ppp:	h.main
od.ppp:	C:h.kernel
od.ppp:	h.ui
od.ppp:	tbox:h.toolbox
od.ppp:	tbox:h.wimp
od.ppp:	C:h.stdio
od.progrmng:	c.progrmng
od.progrmng:	C:h.stdio
od.progrmng:	C:h.stdlib
od.progrmng:	C:h.time
od.progrmng:	h.main
od.progrmng:	C:h.kernel
od.progrmng:	h.ui
od.progrmng:	tbox:h.toolbox
od.progrmng:	tbox:h.wimp
od.progrmng:	C:h.stdio
od.progrmng:	h.ftp
od.progrmng:	C:h.kernel
od.progrmng:	h.errors
od.progrmng:	h.progrmng
od.progrmng:	<Lib$Dir>.zlib.h.zlib
od.progrmng:	<Lib$Dir>.zlib.h.zconf
od.progrmng:	<Lib$Dir>.zlib.h.zlib
od.progrmng:	C:h.swis
od.progrmng:	h.flashstubs
od.progrmng:	C:h.kernel
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	C:h.kernel
o.main:	tbox:h.event
o.main:	tbox:h.wimp
o.main:	tbox:h.toolbox
o.main:	C:debuglib.h.DebugLib
o.main:	h.ui
o.main:	h.errors
o.main:	h.main
o.main:	C:h.kernel
o.main:	h.ui
o.main:	tbox:h.toolbox
o.main:	h.nvram
o.main:	h.dial
o.main:	h.ppp
o.main:	h.ftp
o.main:	h.progrmng
o.main:	<Lib$Dir>.zlib.h.zlib
o.main:	<Lib$Dir>.zlib.h.zconf
o.strings:	c.strings
o.strings:	h.strings
o.ui_menu:	c.ui_menu
o.ui_menu:	h.ui_include
o.ui_menu:	C:h.swis
o.ui_menu:	C:h.kernel
o.ui_menu:	h.ui_tbox
o.ui_menu:	tbox:h.event
o.ui_menu:	tbox:h.wimp
o.ui_menu:	tbox:h.toolbox
o.ui_menu:	tbox:h.toolbox
o.ui_menu:	tbox:h.window
o.ui_menu:	tbox:h.gadgets
o.ui_menu:	tbox:h.gadgets
o.ui_menu:	tbox:h.menu
o.ui_menu:	tbox:h.proginfo
o.ui_menu:	tbox:h.iconbar
o.ui_menu:	tbox:h.quit
o.ui_menu:	tbox:h.wimplib
o.ui_menu:	C:msglib.h.MsgLib
o.ui_menu:	C:debuglib.h.DebugLib
o.ui_menu:	h.ui_menu
o.ui_menu:	h.ui_globals
o.ui_misc:	c.ui_misc
o.ui_misc:	h.ui_include
o.ui_misc:	C:h.swis
o.ui_misc:	C:h.kernel
o.ui_misc:	h.ui_tbox
o.ui_misc:	tbox:h.event
o.ui_misc:	tbox:h.wimp
o.ui_misc:	tbox:h.toolbox
o.ui_misc:	tbox:h.toolbox
o.ui_misc:	tbox:h.window
o.ui_misc:	tbox:h.gadgets
o.ui_misc:	tbox:h.gadgets
o.ui_misc:	tbox:h.menu
o.ui_misc:	tbox:h.proginfo
o.ui_misc:	tbox:h.iconbar
o.ui_misc:	tbox:h.quit
o.ui_misc:	tbox:h.wimplib
o.ui_misc:	C:msglib.h.MsgLib
o.ui_misc:	C:debuglib.h.DebugLib
o.ui_misc:	h.ui_misc
o.dial:	c.dial
o.dial:	C:h.kernel
o.dial:	C:h.swis
o.dial:	h.ui
o.dial:	h.errors
o.dial:	h.main
o.dial:	C:h.kernel
o.dial:	h.ui
o.dial:	tbox:h.toolbox
o.dial:	tbox:h.wimp
o.dial:	h.dial
o.dial:	h.dialler
o.error:	c.error
o.error:	C:h.swis
o.error:	C:h.kernel
o.error:	C:h.signal
o.error:	C:h.kernel
o.error:	h.ui
o.error:	h.errors
o.error:	h.main
o.error:	C:h.kernel
o.error:	h.ui
o.error:	tbox:h.toolbox
o.error:	tbox:h.wimp
o.error:	<Lib$Dir>.zlib.h.zlib
o.error:	<Lib$Dir>.zlib.h.zconf
o.error:	h.ftp
o.error:	h.progrmng
o.error:	<Lib$Dir>.zlib.h.zlib
o.error:	h.ppp
o.error:	h.dial
o.error:	h.flash
o.ftp:	c.ftp
o.ftp:	C:h.kernel
o.ftp:	h.ui
o.ftp:	h.main
o.ftp:	C:h.kernel
o.ftp:	h.ui
o.ftp:	tbox:h.toolbox
o.ftp:	tbox:h.wimp
o.ftp:	h.errors
o.ftp:	h.ftp
o.ftp:	h.progrmng
o.ftp:	<Lib$Dir>.zlib.h.zlib
o.ftp:	<Lib$Dir>.zlib.h.zconf
o.ftp:	<Lib$Dir>.zlib.h.zlib
o.ftp:	h.ppp
o.flashstubs:	c.flashstubs
o.flashstubs:	C:h.kernel
o.flashstubs:	h.ui
o.flashstubs:	h.flashstubs
o.flashstubs:	C:h.kernel
o.flashstubs:	h.main
o.flashstubs:	C:h.kernel
o.flashstubs:	h.ui
o.flashstubs:	tbox:h.toolbox
o.flashstubs:	tbox:h.wimp
o.nvram:	c.nvram
o.nvram:	C:h.kernel
o.nvram:	h.ui
o.nvram:	h.errors
o.nvram:	h.main
o.nvram:	C:h.kernel
o.nvram:	h.ui
o.nvram:	tbox:h.toolbox
o.nvram:	tbox:h.wimp
o.nvram:	h.nvram
o.nvram:	TCPIPLibs:h.inetlib
o.nvram:	TCPIPLibs:sys.h.types
o.nvram:	TCPIPLibs:sys.h.cdefs
o.nvram:	TCPIPLibs:machine.h.endian
o.nvram:	TCPIPLibs:machine.h.ansi
o.nvram:	TCPIPLibs:machine.h.types
o.nvram:	TCPIPLibs:netinet.h.in
o.progrmng:	c.progrmng
o.progrmng:	h.main
o.progrmng:	C:h.kernel
o.progrmng:	h.ui
o.progrmng:	tbox:h.toolbox
o.progrmng:	tbox:h.wimp
o.progrmng:	h.ftp
o.progrmng:	C:h.kernel
o.progrmng:	h.errors
o.progrmng:	h.progrmng
o.progrmng:	<Lib$Dir>.zlib.h.zlib
o.progrmng:	<Lib$Dir>.zlib.h.zconf
o.progrmng:	<Lib$Dir>.zlib.h.zlib
o.progrmng:	C:h.swis
o.progrmng:	h.flash
o.ui_main:	c.ui_main
o.ui_main:	h.ui_include
o.ui_main:	C:h.swis
o.ui_main:	C:h.kernel
o.ui_main:	h.ui_tbox
o.ui_main:	tbox:h.event
o.ui_main:	tbox:h.wimp
o.ui_main:	tbox:h.toolbox
o.ui_main:	tbox:h.toolbox
o.ui_main:	tbox:h.window
o.ui_main:	tbox:h.gadgets
o.ui_main:	tbox:h.gadgets
o.ui_main:	tbox:h.menu
o.ui_main:	tbox:h.proginfo
o.ui_main:	tbox:h.iconbar
o.ui_main:	tbox:h.quit
o.ui_main:	tbox:h.wimplib
o.ui_main:	C:msglib.h.MsgLib
o.ui_main:	C:debuglib.h.DebugLib
o.ui_main:	h.ui_menu
o.ui_main:	h.ui_misc
o.ui_main:	h.ui_main
o.ui_main:	h.ui_types
o.ui_main:	h.strings
o.ui_main:	h.ui_globals
o.ppp:	c.ppp
o.ppp:	C:h.kernel
o.ppp:	C:h.swis
o.ppp:	h.ppp
o.ppp:	TCPIPLibs:sys.h.types
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	TCPIPLibs:machine.h.endian
o.ppp:	TCPIPLibs:machine.h.ansi
o.ppp:	TCPIPLibs:machine.h.types
o.ppp:	TCPIPLibs:sys.h.dcistructs
o.ppp:	C:h.errno
o.ppp:	TCPIPLibs:sys.h.socket
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	TCPIPLibs:sys.h.ioctl
o.ppp:	TCPIPLibs:sys.h.ttycom
o.ppp:	TCPIPLibs:sys.h.ioccom
o.ppp:	TCPIPLibs:sys.h.ioccom
o.ppp:	TCPIPLibs:sys.h.filio
o.ppp:	TCPIPLibs:sys.h.ioccom
o.ppp:	TCPIPLibs:sys.h.sockio
o.ppp:	TCPIPLibs:sys.h.ioccom
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	TCPIPLibs:net.h.if
o.ppp:	TCPIPLibs:sys.h.socket
o.ppp:	TCPIPLibs:sys.h.time
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	TCPIPLibs:net.h.if_arp
o.ppp:	TCPIPLibs:sys.h.socket
o.ppp:	TCPIPLibs:h.socklib
o.ppp:	TCPIPLibs:sys.h.types
o.ppp:	TCPIPLibs:sys.h.socket
o.ppp:	TCPIPLibs:sys.h.stat
o.ppp:	TCPIPLibs:sys.h.time
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	TCPIPLibs:sys.h.time
o.ppp:	TCPIPLibs:sys.h.uio
o.ppp:	TCPIPLibs:sys.h.cdefs
o.ppp:	C:h.kernel
o.ppp:	h.ui
o.ppp:	h.errors
o.ppp:	h.main
o.ppp:	C:h.kernel
o.ppp:	h.ui
o.ppp:	tbox:h.toolbox
o.ppp:	tbox:h.wimp
