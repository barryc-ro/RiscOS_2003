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

INETLIB		= TCPIPLibs:o.inetlibzm
SOCKLIB		= TCPIPLibs:o.socklibzm
UNIXLIB		= TCPIPLibs:o.unixlibzm

LIBSM = \
 ${MSGLIB}zm \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${TOOLACTIONLIB} \
 ${ZLIB}.o.zlibzm \
 ${SOCKLIB} \
 ${INETLIB} \
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
oz.ModuleWrap: s.ModuleWrap
oz.ModuleWrap: s.AppName
oz.ModuleWrap: Hdr:ListOpts
oz.ModuleWrap: Hdr:Macros
oz.ModuleWrap: Hdr:System
oz.ModuleWrap: Hdr:SWIs
oz.ModuleWrap: Hdr:CPU.Generic26
oz.ModuleWrap: Hdr:IO.GenericIO
oz.ModuleWrap: Hdr:RISCOS
oz.ModuleWrap: Hdr:ModHand
oz.ModuleWrap: Hdr:EnvNumbers
oz.ModuleWrap: Hdr:Proc
oz.ModuleWrap: Hdr:FSNumbers
oz.ModuleWrap: Hdr:NewErrors
oz.ModuleWrap: Hdr:Services
oz.ModuleWrap: Hdr:ResourceFS
oz.ModuleWrap: Hdr:MsgTrans
oz.ModuleWrap: Hdr:Variables
oz.ModuleWrap: Hdr:UpCall
oz.ModuleWrap: s.ResFiles
oz.main:	c.main
oz.main:	C:h.swis
oz.main:	C:h.kernel
oz.main:	C:h.kernel
oz.main:	tbox:h.event
oz.main:	tbox:h.wimp
oz.main:	tbox:h.toolbox
oz.main:	C:debuglib.h.DebugLib
oz.main:	h.ui
oz.main:	h.errors
oz.main:	h.main
oz.main:	C:h.kernel
oz.main:	h.ui
oz.main:	tbox:h.toolbox
oz.main:	h.nvram
oz.main:	h.dial
oz.main:	h.ppp
oz.main:	h.ftp
oz.main:	h.progrmng
oz.main:	<Lib$Dir>.zlib.h.zlib
oz.main:	<Lib$Dir>.zlib.h.zconf
oz.strings:	c.strings
oz.strings:	h.strings
oz.ui_main:	c.ui_main
oz.ui_main:	h.ui_include
oz.ui_main:	C:h.swis
oz.ui_main:	C:h.kernel
oz.ui_main:	h.dial
oz.ui_main:	h.ui_tbox
oz.ui_main:	tbox:h.event
oz.ui_main:	tbox:h.wimp
oz.ui_main:	tbox:h.toolbox
oz.ui_main:	tbox:h.toolbox
oz.ui_main:	tbox:h.window
oz.ui_main:	tbox:h.gadgets
oz.ui_main:	tbox:h.gadgets
oz.ui_main:	tbox:h.menu
oz.ui_main:	tbox:h.proginfo
oz.ui_main:	tbox:h.iconbar
oz.ui_main:	tbox:h.quit
oz.ui_main:	tbox:h.wimplib
oz.ui_main:	C:msglib.h.MsgLib
oz.ui_main:	C:debuglib.h.DebugLib
oz.ui_main:	h.ui_menu
oz.ui_main:	h.ui_misc
oz.ui_main:	h.ui_main
oz.ui_main:	h.ui_types
oz.ui_main:	h.strings
oz.ui_main:	h.ui_globals
oz.ui_menu:	c.ui_menu
oz.ui_menu:	h.ui_include
oz.ui_menu:	C:h.swis
oz.ui_menu:	C:h.kernel
oz.ui_menu:	h.ui_tbox
oz.ui_menu:	tbox:h.event
oz.ui_menu:	tbox:h.wimp
oz.ui_menu:	tbox:h.toolbox
oz.ui_menu:	tbox:h.toolbox
oz.ui_menu:	tbox:h.window
oz.ui_menu:	tbox:h.gadgets
oz.ui_menu:	tbox:h.gadgets
oz.ui_menu:	tbox:h.menu
oz.ui_menu:	tbox:h.proginfo
oz.ui_menu:	tbox:h.iconbar
oz.ui_menu:	tbox:h.quit
oz.ui_menu:	tbox:h.wimplib
oz.ui_menu:	C:msglib.h.MsgLib
oz.ui_menu:	C:debuglib.h.DebugLib
oz.ui_menu:	h.ui_menu
oz.ui_menu:	h.ui_globals
oz.ui_misc:	c.ui_misc
oz.ui_misc:	h.ui_include
oz.ui_misc:	C:h.swis
oz.ui_misc:	C:h.kernel
oz.ui_misc:	h.ui_tbox
oz.ui_misc:	tbox:h.event
oz.ui_misc:	tbox:h.wimp
oz.ui_misc:	tbox:h.toolbox
oz.ui_misc:	tbox:h.toolbox
oz.ui_misc:	tbox:h.window
oz.ui_misc:	tbox:h.gadgets
oz.ui_misc:	tbox:h.gadgets
oz.ui_misc:	tbox:h.menu
oz.ui_misc:	tbox:h.proginfo
oz.ui_misc:	tbox:h.iconbar
oz.ui_misc:	tbox:h.quit
oz.ui_misc:	tbox:h.wimplib
oz.ui_misc:	C:msglib.h.MsgLib
oz.ui_misc:	C:debuglib.h.DebugLib
oz.ui_misc:	h.ui_misc
oz.dial:	c.dial
oz.dial:	C:h.kernel
oz.dial:	C:h.swis
oz.dial:	h.ui
oz.dial:	h.errors
oz.dial:	h.main
oz.dial:	C:h.kernel
oz.dial:	h.ui
oz.dial:	tbox:h.toolbox
oz.dial:	tbox:h.wimp
oz.dial:	h.dial
oz.dial:	C:h.dialler
oz.error:	c.error
oz.error:	C:h.swis
oz.error:	C:h.kernel
oz.error:	C:h.signal
oz.error:	C:h.kernel
oz.error:	h.ui
oz.error:	h.errors
oz.error:	h.main
oz.error:	C:h.kernel
oz.error:	h.ui
oz.error:	tbox:h.toolbox
oz.error:	tbox:h.wimp
oz.error:	<Lib$Dir>.zlib.h.zlib
oz.error:	<Lib$Dir>.zlib.h.zconf
oz.error:	h.ftp
oz.error:	h.progrmng
oz.error:	<Lib$Dir>.zlib.h.zlib
oz.error:	h.ppp
oz.error:	h.dial
oz.error:	h.flash
oz.flashstubs:	c.flashstubs
oz.flashstubs:	C:h.kernel
oz.flashstubs:	h.ui
oz.flashstubs:	h.flashstubs
oz.flashstubs:	C:h.kernel
oz.flashstubs:	h.main
oz.flashstubs:	C:h.kernel
oz.flashstubs:	h.ui
oz.flashstubs:	tbox:h.toolbox
oz.flashstubs:	tbox:h.wimp
oz.ftp:	c.ftp
oz.ftp:	C:h.kernel
oz.ftp:	h.ui
oz.ftp:	h.main
oz.ftp:	C:h.kernel
oz.ftp:	h.ui
oz.ftp:	tbox:h.toolbox
oz.ftp:	tbox:h.wimp
oz.ftp:	h.errors
oz.ftp:	h.ftp
oz.ftp:	h.progrmng
oz.ftp:	<Lib$Dir>.zlib.h.zlib
oz.ftp:	<Lib$Dir>.zlib.h.zconf
oz.ftp:	<Lib$Dir>.zlib.h.zlib
oz.ftp:	h.ppp
oz.nvram:	c.nvram
oz.nvram:	C:h.kernel
oz.nvram:	h.ui
oz.nvram:	h.errors
oz.nvram:	h.main
oz.nvram:	C:h.kernel
oz.nvram:	h.ui
oz.nvram:	tbox:h.toolbox
oz.nvram:	tbox:h.wimp
oz.nvram:	h.nvram
oz.nvram:	TCPIPLibs:h.inetlib
oz.nvram:	TCPIPLibs:sys.h.types
oz.nvram:	TCPIPLibs:sys.h.cdefs
oz.nvram:	TCPIPLibs:machine.h.endian
oz.nvram:	TCPIPLibs:machine.h.ansi
oz.nvram:	TCPIPLibs:machine.h.types
oz.nvram:	TCPIPLibs:netinet.h.in
oz.ppp:	c.ppp
oz.ppp:	C:h.kernel
oz.ppp:	C:h.swis
oz.ppp:	h.ppp
oz.ppp:	TCPIPLibs:sys.h.types
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	TCPIPLibs:machine.h.endian
oz.ppp:	TCPIPLibs:machine.h.ansi
oz.ppp:	TCPIPLibs:machine.h.types
oz.ppp:	TCPIPLibs:sys.h.dcistructs
oz.ppp:	C:h.errno
oz.ppp:	TCPIPLibs:sys.h.socket
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	TCPIPLibs:sys.h.ioctl
oz.ppp:	TCPIPLibs:sys.h.ttycom
oz.ppp:	TCPIPLibs:sys.h.ioccom
oz.ppp:	TCPIPLibs:sys.h.ioccom
oz.ppp:	TCPIPLibs:sys.h.filio
oz.ppp:	TCPIPLibs:sys.h.ioccom
oz.ppp:	TCPIPLibs:sys.h.sockio
oz.ppp:	TCPIPLibs:sys.h.ioccom
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	TCPIPLibs:net.h.if
oz.ppp:	TCPIPLibs:sys.h.socket
oz.ppp:	TCPIPLibs:sys.h.time
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	TCPIPLibs:net.h.if_arp
oz.ppp:	TCPIPLibs:sys.h.socket
oz.ppp:	TCPIPLibs:h.socklib
oz.ppp:	TCPIPLibs:sys.h.types
oz.ppp:	TCPIPLibs:sys.h.socket
oz.ppp:	TCPIPLibs:sys.h.stat
oz.ppp:	TCPIPLibs:sys.h.time
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	TCPIPLibs:sys.h.time
oz.ppp:	TCPIPLibs:sys.h.uio
oz.ppp:	TCPIPLibs:sys.h.cdefs
oz.ppp:	C:h.kernel
oz.ppp:	h.ui
oz.ppp:	h.errors
oz.ppp:	h.main
oz.ppp:	C:h.kernel
oz.ppp:	h.ui
oz.ppp:	tbox:h.toolbox
oz.ppp:	tbox:h.wimp
oz.progrmng:	c.progrmng
oz.progrmng:	h.main
oz.progrmng:	C:h.kernel
oz.progrmng:	h.ui
oz.progrmng:	tbox:h.toolbox
oz.progrmng:	tbox:h.wimp
oz.progrmng:	h.ftp
oz.progrmng:	C:h.kernel
oz.progrmng:	h.errors
oz.progrmng:	h.progrmng
oz.progrmng:	<Lib$Dir>.zlib.h.zlib
oz.progrmng:	<Lib$Dir>.zlib.h.zconf
oz.progrmng:	<Lib$Dir>.zlib.h.zlib
oz.progrmng:	C:h.swis
oz.progrmng:	h.flash
