# Makefile for Daytona Setup
# ==========================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1997-10-06	BAL	Created

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= DaytSetup
APP		= !DaytSetup
MODULE		= rm.!DaySetup
ROM_MODULE	= aof.DaytSetup
LDIR		= Resources.${LOCALE}

# ----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = ${RESDIR}.${COMPONENT}
RESAPP          = <Resource$Dir>.Apps.${APP}


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS		= ${THROWBACK} -depend !Depend -ffah ${INCLUDES} -wp
CPFLAGS		= ~cfr~v~n
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS		= -DFINAL=1
DDFLAGS		= -DFINAL=0 -DDEBUGLIB -fn
DZFLAGS		= 
DDZFLAGS	= ${DZFLAGS} -fn


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>

CLIB		= CLib:o.stubs
CPPLIB		= CPPLib:o.c++lib
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

SOCKLIB		= TCPIPLibs:o.socklib5

EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
TEXTGADGETLIB	= tbox:o.textgadget

ACORNSMTPLIB	= <CExport$Dir>.o.AcornSMTP
ACORNPOP3LIB	= <CExport$Dir>.o.AcornPOP3

UILIB		= ${LIBDIR}.netfax.UILib.o.UILib
ERRORLIB	= ${LIBDIR}.ErrorLib.o.ErrorLib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
NUMCONVERT      = ${LIBDIR}.NumConvert.o.numconv
DEBUGLIB	= ${LIBDIR}.DebugLib.o.DebugLib


LIBSM = \
 ${UILIB}zm \
 ${MSGLIB}zm \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${NUMCONVERT}zm \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.unixlib \
 TCPIPLibs:o.inetlib \


LIBS = \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \
 ${NUMCONVERT} \
 ${LIBDIR}.TCPIPLibs.o.socklib5 \
 ${LIBDIR}.TCPIPLibs.o.unixlib \
 ${LIBDIR}.TCPIPLibs.o.inetlib \


LIBSD = \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \
 ${NUMCONVERT} \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.unixlib \
 TCPIPLibs:o.inetlib \


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,TCPIPLIBS:

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 ${LDIR}.!Sprites \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.datetime\
 o.fontsetup\
 o.load\
 o.main\
 o.messhandle\
 o.misc\
 o.openwin\
 o.runes\
 o.save\
 o.scheduler\
 o.ui\
 o.cancel\


OBJSZ = \
 oz.datetime\
 oz.fontsetup\
 oz.load\
 oz.main\
 oz.messhandle\
 oz.misc\
 oz.openwin\
 oz.runes\
 oz.save\
 oz.scheduler\
 oz.ui\
 oz.cancel\


OBJSD =\
 od.datetime\
 od.fontsetup\
 od.load\
 od.main\
 od.messhandle\
 od.misc\
 od.openwin\
 od.runes\
 od.save\
 od.scheduler\
 od.ui\
 od.cancel\


#OBJSZ =
#OBJSD =
#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<

.c.oz:;		@echo
		@echo Compiling $<
		@echo ${DZFLAGS}
		@${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -zps1 -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CFLAGS} ${DDFLAGS}       -c -o $@ $<

.c.odz:;	@echo
		@echo Compiling $<
		@echo ${DDZFLAGS}
		@${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -zps1 -DROM -o $@ $<

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

install_common: ${FILES}
	@echo
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}

install: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}


installd: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
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
	@${MKDIR} Targets.!DaytSetup
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

rm.!DaytSetD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:


od.datetime:	c.datetime
od.datetime:	h.main
od.datetime:	tbox:h.toolbox
od.datetime:	C:h.kernel
od.datetime:	tbox:h.wimp
od.datetime:	tbox:h.window
od.datetime:	tbox:h.gadgets
od.datetime:	tbox:h.menu
od.datetime:	tbox:h.event
od.datetime:	tbox:h.wimp
od.datetime:	tbox:h.wimplib
od.datetime:	tbox:h.gadgets
od.datetime:	tbox:h.iconbar
od.datetime:	C:h.kernel
od.datetime:	C:h.swis
od.datetime:	C:ErrorLib.h.ErrorLib
od.datetime:	C:MsgLib.h.MsgLib
od.datetime:	C:NetFax.UILib.h.UILib
od.datetime:	C:TCPIPLibs.sys.h.errno
od.datetime:	C:h.kernel
od.datetime:	C:TCPIPLibs.h.inetlib
od.datetime:	C:TCPIPLibs.sys.h.types
od.datetime:	TCPIPLIBS:sys.h.cdefs
od.datetime:	TCPIPLIBS:machine.h.endian
od.datetime:	TCPIPLIBS:machine.h.ansi
od.datetime:	TCPIPLIBS:machine.h.types
od.datetime:	C:TCPIPLibs.netinet.h.in
od.datetime:	C:TCPIPLibs.h.unixlib
od.datetime:	h.Windows
od.datetime:	C:DebugLib.h.DebugLib
od.datetime:	h.functions
od.datetime:	tbox:h.toolbox
od.datetime:	h.misc
od.datetime:	C:h.swis
od.datetime:	C:h.kernel
od.datetime:	h.datetime
od.datetime:	h.main
od.datetime:	h.functions
od.datetime:	h.misc
od.datetime:	C:h.swis
od.datetime:	C:h.kernel
od.fontsetup:	c.fontsetup
od.fontsetup:	h.main
od.fontsetup:	tbox:h.toolbox
od.fontsetup:	C:h.kernel
od.fontsetup:	tbox:h.wimp
od.fontsetup:	tbox:h.window
od.fontsetup:	tbox:h.gadgets
od.fontsetup:	tbox:h.menu
od.fontsetup:	tbox:h.event
od.fontsetup:	tbox:h.wimp
od.fontsetup:	tbox:h.wimplib
od.fontsetup:	tbox:h.gadgets
od.fontsetup:	tbox:h.iconbar
od.fontsetup:	C:h.kernel
od.fontsetup:	C:h.swis
od.fontsetup:	C:ErrorLib.h.ErrorLib
od.fontsetup:	C:MsgLib.h.MsgLib
od.fontsetup:	C:NetFax.UILib.h.UILib
od.fontsetup:	C:TCPIPLibs.sys.h.errno
od.fontsetup:	C:h.kernel
od.fontsetup:	C:TCPIPLibs.h.inetlib
od.fontsetup:	C:TCPIPLibs.sys.h.types
od.fontsetup:	TCPIPLIBS:sys.h.cdefs
od.fontsetup:	TCPIPLIBS:machine.h.endian
od.fontsetup:	TCPIPLIBS:machine.h.ansi
od.fontsetup:	TCPIPLIBS:machine.h.types
od.fontsetup:	C:TCPIPLibs.netinet.h.in
od.fontsetup:	C:TCPIPLibs.h.unixlib
od.fontsetup:	h.Windows
od.fontsetup:	C:DebugLib.h.DebugLib
od.fontsetup:	h.functions
od.fontsetup:	tbox:h.toolbox
od.load:	c.load
od.load:	h.main
od.load:	tbox:h.toolbox
od.load:	C:h.kernel
od.load:	tbox:h.wimp
od.load:	tbox:h.window
od.load:	tbox:h.gadgets
od.load:	tbox:h.menu
od.load:	tbox:h.event
od.load:	tbox:h.wimp
od.load:	tbox:h.wimplib
od.load:	tbox:h.gadgets
od.load:	tbox:h.iconbar
od.load:	C:h.kernel
od.load:	C:h.swis
od.load:	C:ErrorLib.h.ErrorLib
od.load:	C:MsgLib.h.MsgLib
od.load:	C:NetFax.UILib.h.UILib
od.load:	C:TCPIPLibs.sys.h.errno
od.load:	C:h.kernel
od.load:	C:TCPIPLibs.h.inetlib
od.load:	C:TCPIPLibs.sys.h.types
od.load:	TCPIPLIBS:sys.h.cdefs
od.load:	TCPIPLIBS:machine.h.endian
od.load:	TCPIPLIBS:machine.h.ansi
od.load:	TCPIPLIBS:machine.h.types
od.load:	C:TCPIPLibs.netinet.h.in
od.load:	C:TCPIPLibs.h.unixlib
od.load:	h.Windows
od.load:	C:DebugLib.h.DebugLib
od.load:	h.functions
od.load:	tbox:h.toolbox
od.load:	C:h.swis
od.load:	C:h.kernel
od.load:	h.misc
od.load:	h.messhandle
od.load:	tbox:h.toolbox
od.load:	h.scheduler
od.load:	h.main
od.load:	h.misc
od.load:	h.ui
od.load:	h.main
od.load:	h.functions
od.load:	C:h.kernel
od.load:	h.misc
od.load:	h.scheduler
od.load:	C:numconvert.h.numconvert
od.load:	h.save
od.load:	h.main
od.load:	h.functions
od.load:	h.messhandle
od.load:	h.misc
od.load:	h.load
od.load:	h.main
od.load:	h.functions
od.load:	C:h.swis
od.load:	C:h.kernel
od.load:	h.misc
od.load:	h.messhandle
od.main:	c.main
od.main:	h.main
od.main:	tbox:h.toolbox
od.main:	C:h.kernel
od.main:	tbox:h.wimp
od.main:	tbox:h.window
od.main:	tbox:h.gadgets
od.main:	tbox:h.menu
od.main:	tbox:h.event
od.main:	tbox:h.wimp
od.main:	tbox:h.wimplib
od.main:	tbox:h.gadgets
od.main:	tbox:h.iconbar
od.main:	C:h.kernel
od.main:	C:h.swis
od.main:	C:ErrorLib.h.ErrorLib
od.main:	C:MsgLib.h.MsgLib
od.main:	C:NetFax.UILib.h.UILib
od.main:	C:TCPIPLibs.sys.h.errno
od.main:	C:h.kernel
od.main:	C:TCPIPLibs.h.inetlib
od.main:	C:TCPIPLibs.sys.h.types
od.main:	TCPIPLIBS:sys.h.cdefs
od.main:	TCPIPLIBS:machine.h.endian
od.main:	TCPIPLIBS:machine.h.ansi
od.main:	TCPIPLIBS:machine.h.types
od.main:	C:TCPIPLibs.netinet.h.in
od.main:	C:TCPIPLibs.h.unixlib
od.main:	h.Windows
od.main:	C:DebugLib.h.DebugLib
od.main:	h.functions
od.main:	tbox:h.toolbox
od.main:	h.memhandle
od.main:	h.messhandle
od.main:	tbox:h.toolbox
od.main:	h.scheduler
od.main:	h.main
od.main:	h.misc
od.main:	tbox:h.proginfo
od.main:	h.load
od.main:	h.main
od.main:	h.functions
od.main:	C:h.swis
od.main:	C:h.kernel
od.main:	h.misc
od.main:	h.messhandle
od.messhandle:	c.messhandle
od.messhandle:	h.messhandle
od.messhandle:	tbox:h.toolbox
od.messhandle:	C:h.kernel
od.messhandle:	tbox:h.wimp
od.messhandle:	h.scheduler
od.messhandle:	h.main
od.messhandle:	tbox:h.toolbox
od.messhandle:	tbox:h.window
od.messhandle:	tbox:h.gadgets
od.messhandle:	tbox:h.menu
od.messhandle:	tbox:h.event
od.messhandle:	tbox:h.wimp
od.messhandle:	tbox:h.wimplib
od.messhandle:	tbox:h.gadgets
od.messhandle:	tbox:h.iconbar
od.messhandle:	C:h.kernel
od.messhandle:	C:h.swis
od.messhandle:	C:ErrorLib.h.ErrorLib
od.messhandle:	C:MsgLib.h.MsgLib
od.messhandle:	C:NetFax.UILib.h.UILib
od.messhandle:	C:TCPIPLibs.sys.h.errno
od.messhandle:	C:h.kernel
od.messhandle:	C:TCPIPLibs.h.inetlib
od.messhandle:	C:TCPIPLibs.sys.h.types
od.messhandle:	TCPIPLIBS:sys.h.cdefs
od.messhandle:	TCPIPLIBS:machine.h.endian
od.messhandle:	TCPIPLIBS:machine.h.ansi
od.messhandle:	TCPIPLIBS:machine.h.types
od.messhandle:	C:TCPIPLibs.netinet.h.in
od.messhandle:	C:TCPIPLibs.h.unixlib
od.messhandle:	h.Windows
od.messhandle:	C:DebugLib.h.DebugLib
od.messhandle:	h.misc
od.messhandle:	h.main
od.misc:	c.misc
od.misc:	h.main
od.misc:	tbox:h.toolbox
od.misc:	C:h.kernel
od.misc:	tbox:h.wimp
od.misc:	tbox:h.window
od.misc:	tbox:h.gadgets
od.misc:	tbox:h.menu
od.misc:	tbox:h.event
od.misc:	tbox:h.wimp
od.misc:	tbox:h.wimplib
od.misc:	tbox:h.gadgets
od.misc:	tbox:h.iconbar
od.misc:	C:h.kernel
od.misc:	C:h.swis
od.misc:	C:ErrorLib.h.ErrorLib
od.misc:	C:MsgLib.h.MsgLib
od.misc:	C:NetFax.UILib.h.UILib
od.misc:	C:TCPIPLibs.sys.h.errno
od.misc:	C:h.kernel
od.misc:	C:TCPIPLibs.h.inetlib
od.misc:	C:TCPIPLibs.sys.h.types
od.misc:	TCPIPLIBS:sys.h.cdefs
od.misc:	TCPIPLIBS:machine.h.endian
od.misc:	TCPIPLIBS:machine.h.ansi
od.misc:	TCPIPLIBS:machine.h.types
od.misc:	C:TCPIPLibs.netinet.h.in
od.misc:	C:TCPIPLibs.h.unixlib
od.misc:	h.Windows
od.misc:	C:DebugLib.h.DebugLib
od.misc:	h.misc
od.openwin:	c.openwin
od.openwin:	h.main
od.openwin:	tbox:h.toolbox
od.openwin:	C:h.kernel
od.openwin:	tbox:h.wimp
od.openwin:	tbox:h.window
od.openwin:	tbox:h.gadgets
od.openwin:	tbox:h.menu
od.openwin:	tbox:h.event
od.openwin:	tbox:h.wimp
od.openwin:	tbox:h.wimplib
od.openwin:	tbox:h.gadgets
od.openwin:	tbox:h.iconbar
od.openwin:	C:h.kernel
od.openwin:	C:h.swis
od.openwin:	C:ErrorLib.h.ErrorLib
od.openwin:	C:MsgLib.h.MsgLib
od.openwin:	C:NetFax.UILib.h.UILib
od.openwin:	C:TCPIPLibs.sys.h.errno
od.openwin:	C:h.kernel
od.openwin:	C:TCPIPLibs.h.inetlib
od.openwin:	C:TCPIPLibs.sys.h.types
od.openwin:	TCPIPLIBS:sys.h.cdefs
od.openwin:	TCPIPLIBS:machine.h.endian
od.openwin:	TCPIPLIBS:machine.h.ansi
od.openwin:	TCPIPLIBS:machine.h.types
od.openwin:	C:TCPIPLibs.netinet.h.in
od.openwin:	C:TCPIPLibs.h.unixlib
od.openwin:	h.Windows
od.openwin:	C:DebugLib.h.DebugLib
od.openwin:	h.functions
od.openwin:	tbox:h.toolbox
od.openwin:	h.openwin
od.openwin:	h.main
od.openwin:	h.functions
od.runes:	c.runes
od.runes:	TCPIPLIBS:sys.h.errno
od.runes:	C:h.kernel
od.runes:	TCPIPLIBS:h.inetlib
od.runes:	TCPIPLIBS:sys.h.types
od.runes:	TCPIPLIBS:sys.h.cdefs
od.runes:	TCPIPLIBS:machine.h.endian
od.runes:	TCPIPLIBS:machine.h.ansi
od.runes:	TCPIPLIBS:machine.h.types
od.runes:	TCPIPLIBS:netinet.h.in
od.runes:	TCPIPLIBS:h.unixlib
od.runes:	h.main
od.runes:	tbox:h.toolbox
od.runes:	tbox:h.wimp
od.runes:	tbox:h.window
od.runes:	tbox:h.gadgets
od.runes:	tbox:h.menu
od.runes:	tbox:h.event
od.runes:	tbox:h.wimp
od.runes:	tbox:h.wimplib
od.runes:	tbox:h.gadgets
od.runes:	tbox:h.iconbar
od.runes:	C:h.kernel
od.runes:	C:h.swis
od.runes:	C:ErrorLib.h.ErrorLib
od.runes:	C:MsgLib.h.MsgLib
od.runes:	C:NetFax.UILib.h.UILib
od.runes:	C:TCPIPLibs.sys.h.errno
od.runes:	C:TCPIPLibs.h.inetlib
od.runes:	C:TCPIPLibs.sys.h.types
od.runes:	C:TCPIPLibs.netinet.h.in
od.runes:	C:TCPIPLibs.h.unixlib
od.runes:	h.Windows
od.runes:	C:DebugLib.h.DebugLib
od.runes:	h.functions
od.runes:	tbox:h.toolbox
od.runes:	h.datetime
od.runes:	h.main
od.runes:	h.functions
od.runes:	h.misc
od.runes:	C:h.swis
od.runes:	C:h.kernel
od.runes:	h.scheduler
od.runes:	h.main
od.runes:	h.misc
od.runes:	h.messhandle
od.runes:	tbox:h.toolbox
od.runes:	h.ui
od.runes:	h.main
od.runes:	h.functions
od.runes:	C:h.kernel
od.runes:	h.misc
od.runes:	h.scheduler
od.runes:	h.load
od.runes:	h.main
od.runes:	h.functions
od.runes:	C:h.swis
od.runes:	C:h.kernel
od.runes:	h.misc
od.runes:	h.messhandle
od.runes:	h.save
od.runes:	h.main
od.runes:	h.functions
od.runes:	h.messhandle
od.runes:	h.misc
od.runes:	h.cancel
od.runes:	h.load
od.runes:	h.misc
od.save:	c.save
od.save:	h.main
od.save:	tbox:h.toolbox
od.save:	C:h.kernel
od.save:	tbox:h.wimp
od.save:	tbox:h.window
od.save:	tbox:h.gadgets
od.save:	tbox:h.menu
od.save:	tbox:h.event
od.save:	tbox:h.wimp
od.save:	tbox:h.wimplib
od.save:	tbox:h.gadgets
od.save:	tbox:h.iconbar
od.save:	C:h.kernel
od.save:	C:h.swis
od.save:	C:ErrorLib.h.ErrorLib
od.save:	C:MsgLib.h.MsgLib
od.save:	C:NetFax.UILib.h.UILib
od.save:	C:TCPIPLibs.sys.h.errno
od.save:	C:h.kernel
od.save:	C:TCPIPLibs.h.inetlib
od.save:	C:TCPIPLibs.sys.h.types
od.save:	TCPIPLIBS:sys.h.cdefs
od.save:	TCPIPLIBS:machine.h.endian
od.save:	TCPIPLIBS:machine.h.ansi
od.save:	TCPIPLIBS:machine.h.types
od.save:	C:TCPIPLibs.netinet.h.in
od.save:	C:TCPIPLibs.h.unixlib
od.save:	h.Windows
od.save:	C:DebugLib.h.DebugLib
od.save:	h.functions
od.save:	tbox:h.toolbox
od.save:	h.messhandle
od.save:	tbox:h.toolbox
od.save:	h.misc
od.save:	h.scheduler
od.save:	h.main
od.save:	h.misc
od.save:	C:numconvert.h.numconvert
od.save:	h.ui
od.save:	h.main
od.save:	h.functions
od.save:	C:h.kernel
od.save:	h.misc
od.save:	h.scheduler
od.save:	h.save
od.save:	h.main
od.save:	h.functions
od.save:	h.messhandle
od.save:	h.misc
od.save:	h.load
od.save:	h.main
od.save:	h.functions
od.save:	C:h.swis
od.save:	C:h.kernel
od.save:	h.misc
od.save:	h.messhandle
od.scheduler:	c.scheduler
od.scheduler:	h.main
od.scheduler:	tbox:h.toolbox
od.scheduler:	C:h.kernel
od.scheduler:	tbox:h.wimp
od.scheduler:	tbox:h.window
od.scheduler:	tbox:h.gadgets
od.scheduler:	tbox:h.menu
od.scheduler:	tbox:h.event
od.scheduler:	tbox:h.wimp
od.scheduler:	tbox:h.wimplib
od.scheduler:	tbox:h.gadgets
od.scheduler:	tbox:h.iconbar
od.scheduler:	C:h.kernel
od.scheduler:	C:h.swis
od.scheduler:	C:ErrorLib.h.ErrorLib
od.scheduler:	C:MsgLib.h.MsgLib
od.scheduler:	C:NetFax.UILib.h.UILib
od.scheduler:	C:TCPIPLibs.sys.h.errno
od.scheduler:	C:h.kernel
od.scheduler:	C:TCPIPLibs.h.inetlib
od.scheduler:	C:TCPIPLibs.sys.h.types
od.scheduler:	TCPIPLIBS:sys.h.cdefs
od.scheduler:	TCPIPLIBS:machine.h.endian
od.scheduler:	TCPIPLIBS:machine.h.ansi
od.scheduler:	TCPIPLIBS:machine.h.types
od.scheduler:	C:TCPIPLibs.netinet.h.in
od.scheduler:	C:TCPIPLibs.h.unixlib
od.scheduler:	h.Windows
od.scheduler:	C:DebugLib.h.DebugLib
od.scheduler:	h.misc
od.scheduler:	h.scheduler
od.scheduler:	h.main
od.scheduler:	h.misc
od.ui:	c.ui
od.ui:	h.main
od.ui:	tbox:h.toolbox
od.ui:	C:h.kernel
od.ui:	tbox:h.wimp
od.ui:	tbox:h.window
od.ui:	tbox:h.gadgets
od.ui:	tbox:h.menu
od.ui:	tbox:h.event
od.ui:	tbox:h.wimp
od.ui:	tbox:h.wimplib
od.ui:	tbox:h.gadgets
od.ui:	tbox:h.iconbar
od.ui:	C:h.kernel
od.ui:	C:h.swis
od.ui:	C:ErrorLib.h.ErrorLib
od.ui:	C:MsgLib.h.MsgLib
od.ui:	C:NetFax.UILib.h.UILib
od.ui:	C:TCPIPLibs.sys.h.errno
od.ui:	C:h.kernel
od.ui:	C:TCPIPLibs.h.inetlib
od.ui:	C:TCPIPLibs.sys.h.types
od.ui:	TCPIPLIBS:sys.h.cdefs
od.ui:	TCPIPLIBS:machine.h.endian
od.ui:	TCPIPLIBS:machine.h.ansi
od.ui:	TCPIPLIBS:machine.h.types
od.ui:	C:TCPIPLibs.netinet.h.in
od.ui:	C:TCPIPLibs.h.unixlib
od.ui:	h.Windows
od.ui:	C:DebugLib.h.DebugLib
od.ui:	h.ui
od.ui:	h.main
od.ui:	h.functions
od.ui:	tbox:h.toolbox
od.ui:	C:h.kernel
od.ui:	h.misc
od.ui:	h.scheduler
od.ui:	h.main
od.ui:	h.misc
od.ui:	h.functions
od.ui:	C:h.kernel
od.ui:	h.misc
od.ui:	h.load
od.ui:	h.main
od.ui:	h.functions
od.ui:	C:h.swis
od.ui:	C:h.kernel
od.ui:	h.misc
od.ui:	h.messhandle
od.ui:	tbox:h.toolbox
od.ui:	h.scheduler
od.cancel:	c.cancel
od.cancel:	C:h.kernel
od.cancel:	h.datetime
od.cancel:	h.main
od.cancel:	tbox:h.toolbox
od.cancel:	tbox:h.wimp
od.cancel:	tbox:h.window
od.cancel:	tbox:h.gadgets
od.cancel:	tbox:h.menu
od.cancel:	tbox:h.event
od.cancel:	tbox:h.wimp
od.cancel:	tbox:h.wimplib
od.cancel:	tbox:h.gadgets
od.cancel:	tbox:h.iconbar
od.cancel:	C:h.kernel
od.cancel:	C:h.swis
od.cancel:	C:ErrorLib.h.ErrorLib
od.cancel:	C:MsgLib.h.MsgLib
od.cancel:	C:NetFax.UILib.h.UILib
od.cancel:	C:TCPIPLibs.sys.h.errno
od.cancel:	C:h.kernel
od.cancel:	C:TCPIPLibs.h.inetlib
od.cancel:	C:TCPIPLibs.sys.h.types
od.cancel:	TCPIPLIBS:sys.h.cdefs
od.cancel:	TCPIPLIBS:machine.h.endian
od.cancel:	TCPIPLIBS:machine.h.ansi
od.cancel:	TCPIPLIBS:machine.h.types
od.cancel:	C:TCPIPLibs.netinet.h.in
od.cancel:	C:TCPIPLibs.h.unixlib
od.cancel:	h.Windows
od.cancel:	C:DebugLib.h.DebugLib
od.cancel:	h.functions
od.cancel:	tbox:h.toolbox
od.cancel:	h.misc
od.cancel:	C:h.swis
od.cancel:	C:h.kernel
od.cancel:	C:h.swis
od.cancel:	h.cancel
od.cancel:	h.load
od.cancel:	h.main
od.cancel:	h.functions
od.cancel:	C:h.swis
od.cancel:	C:h.kernel
od.cancel:	h.misc
od.cancel:	h.messhandle
od.cancel:	tbox:h.toolbox
od.cancel:	h.misc
od.cancel:	h.load
od.cancel:	h.misc
o.datetime:	c.datetime
o.datetime:	h.main
o.datetime:	tbox:h.toolbox
o.datetime:	C:h.kernel
o.datetime:	tbox:h.wimp
o.datetime:	tbox:h.window
o.datetime:	tbox:h.gadgets
o.datetime:	tbox:h.menu
o.datetime:	tbox:h.event
o.datetime:	tbox:h.wimp
o.datetime:	tbox:h.wimplib
o.datetime:	tbox:h.gadgets
o.datetime:	tbox:h.iconbar
o.datetime:	C:h.kernel
o.datetime:	C:h.swis
o.datetime:	C:ErrorLib.h.ErrorLib
o.datetime:	C:MsgLib.h.MsgLib
o.datetime:	C:NetFax.UILib.h.UILib
o.datetime:	C:TCPIPLibs.sys.h.errno
o.datetime:	C:h.kernel
o.datetime:	C:TCPIPLibs.h.inetlib
o.datetime:	C:TCPIPLibs.sys.h.types
o.datetime:	TCPIPLIBS:sys.h.cdefs
o.datetime:	TCPIPLIBS:machine.h.endian
o.datetime:	TCPIPLIBS:machine.h.ansi
o.datetime:	TCPIPLIBS:machine.h.types
o.datetime:	C:TCPIPLibs.netinet.h.in
o.datetime:	C:TCPIPLibs.h.unixlib
o.datetime:	h.Windows
o.datetime:	C:DebugLib.h.DebugLib
o.datetime:	h.functions
o.datetime:	tbox:h.toolbox
o.datetime:	h.misc
o.datetime:	C:h.swis
o.datetime:	C:h.kernel
o.datetime:	h.datetime
o.datetime:	h.main
o.datetime:	h.functions
o.datetime:	h.misc
o.datetime:	C:h.swis
o.datetime:	C:h.kernel
o.fontsetup:	c.fontsetup
o.fontsetup:	h.main
o.fontsetup:	tbox:h.toolbox
o.fontsetup:	C:h.kernel
o.fontsetup:	tbox:h.wimp
o.fontsetup:	tbox:h.window
o.fontsetup:	tbox:h.gadgets
o.fontsetup:	tbox:h.menu
o.fontsetup:	tbox:h.event
o.fontsetup:	tbox:h.wimp
o.fontsetup:	tbox:h.wimplib
o.fontsetup:	tbox:h.gadgets
o.fontsetup:	tbox:h.iconbar
o.fontsetup:	C:h.kernel
o.fontsetup:	C:h.swis
o.fontsetup:	C:ErrorLib.h.ErrorLib
o.fontsetup:	C:MsgLib.h.MsgLib
o.fontsetup:	C:NetFax.UILib.h.UILib
o.fontsetup:	C:TCPIPLibs.sys.h.errno
o.fontsetup:	C:h.kernel
o.fontsetup:	C:TCPIPLibs.h.inetlib
o.fontsetup:	C:TCPIPLibs.sys.h.types
o.fontsetup:	TCPIPLIBS:sys.h.cdefs
o.fontsetup:	TCPIPLIBS:machine.h.endian
o.fontsetup:	TCPIPLIBS:machine.h.ansi
o.fontsetup:	TCPIPLIBS:machine.h.types
o.fontsetup:	C:TCPIPLibs.netinet.h.in
o.fontsetup:	C:TCPIPLibs.h.unixlib
o.fontsetup:	h.Windows
o.fontsetup:	C:DebugLib.h.DebugLib
o.fontsetup:	h.functions
o.fontsetup:	tbox:h.toolbox
o.load:	c.load
o.load:	h.main
o.load:	tbox:h.toolbox
o.load:	C:h.kernel
o.load:	tbox:h.wimp
o.load:	tbox:h.window
o.load:	tbox:h.gadgets
o.load:	tbox:h.menu
o.load:	tbox:h.event
o.load:	tbox:h.wimp
o.load:	tbox:h.wimplib
o.load:	tbox:h.gadgets
o.load:	tbox:h.iconbar
o.load:	C:h.kernel
o.load:	C:h.swis
o.load:	C:ErrorLib.h.ErrorLib
o.load:	C:MsgLib.h.MsgLib
o.load:	C:NetFax.UILib.h.UILib
o.load:	C:TCPIPLibs.sys.h.errno
o.load:	C:h.kernel
o.load:	C:TCPIPLibs.h.inetlib
o.load:	C:TCPIPLibs.sys.h.types
o.load:	TCPIPLIBS:sys.h.cdefs
o.load:	TCPIPLIBS:machine.h.endian
o.load:	TCPIPLIBS:machine.h.ansi
o.load:	TCPIPLIBS:machine.h.types
o.load:	C:TCPIPLibs.netinet.h.in
o.load:	C:TCPIPLibs.h.unixlib
o.load:	h.Windows
o.load:	C:DebugLib.h.DebugLib
o.load:	h.functions
o.load:	tbox:h.toolbox
o.load:	C:h.swis
o.load:	C:h.kernel
o.load:	h.misc
o.load:	h.messhandle
o.load:	tbox:h.toolbox
o.load:	h.scheduler
o.load:	h.main
o.load:	h.misc
o.load:	h.ui
o.load:	h.main
o.load:	h.functions
o.load:	C:h.kernel
o.load:	h.misc
o.load:	h.scheduler
o.load:	C:numconvert.h.numconvert
o.load:	h.save
o.load:	h.main
o.load:	h.functions
o.load:	h.messhandle
o.load:	h.misc
o.load:	h.load
o.load:	h.main
o.load:	h.functions
o.load:	C:h.swis
o.load:	C:h.kernel
o.load:	h.misc
o.load:	h.messhandle
o.main:	c.main
o.main:	h.main
o.main:	tbox:h.toolbox
o.main:	C:h.kernel
o.main:	tbox:h.wimp
o.main:	tbox:h.window
o.main:	tbox:h.gadgets
o.main:	tbox:h.menu
o.main:	tbox:h.event
o.main:	tbox:h.wimp
o.main:	tbox:h.wimplib
o.main:	tbox:h.gadgets
o.main:	tbox:h.iconbar
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	C:ErrorLib.h.ErrorLib
o.main:	C:MsgLib.h.MsgLib
o.main:	C:NetFax.UILib.h.UILib
o.main:	C:TCPIPLibs.sys.h.errno
o.main:	C:h.kernel
o.main:	C:TCPIPLibs.h.inetlib
o.main:	C:TCPIPLibs.sys.h.types
o.main:	TCPIPLIBS:sys.h.cdefs
o.main:	TCPIPLIBS:machine.h.endian
o.main:	TCPIPLIBS:machine.h.ansi
o.main:	TCPIPLIBS:machine.h.types
o.main:	C:TCPIPLibs.netinet.h.in
o.main:	C:TCPIPLibs.h.unixlib
o.main:	h.Windows
o.main:	C:DebugLib.h.DebugLib
o.main:	h.functions
o.main:	tbox:h.toolbox
o.main:	h.memhandle
o.main:	h.messhandle
o.main:	tbox:h.toolbox
o.main:	h.scheduler
o.main:	h.main
o.main:	h.misc
o.main:	tbox:h.proginfo
o.main:	h.load
o.main:	h.main
o.main:	h.functions
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	h.misc
o.main:	h.messhandle
o.messhandle:	c.messhandle
o.messhandle:	h.messhandle
o.messhandle:	tbox:h.toolbox
o.messhandle:	C:h.kernel
o.messhandle:	tbox:h.wimp
o.messhandle:	h.scheduler
o.messhandle:	h.main
o.messhandle:	tbox:h.toolbox
o.messhandle:	tbox:h.window
o.messhandle:	tbox:h.gadgets
o.messhandle:	tbox:h.menu
o.messhandle:	tbox:h.event
o.messhandle:	tbox:h.wimp
o.messhandle:	tbox:h.wimplib
o.messhandle:	tbox:h.gadgets
o.messhandle:	tbox:h.iconbar
o.messhandle:	C:h.kernel
o.messhandle:	C:h.swis
o.messhandle:	C:ErrorLib.h.ErrorLib
o.messhandle:	C:MsgLib.h.MsgLib
o.messhandle:	C:NetFax.UILib.h.UILib
o.messhandle:	C:TCPIPLibs.sys.h.errno
o.messhandle:	C:h.kernel
o.messhandle:	C:TCPIPLibs.h.inetlib
o.messhandle:	C:TCPIPLibs.sys.h.types
o.messhandle:	TCPIPLIBS:sys.h.cdefs
o.messhandle:	TCPIPLIBS:machine.h.endian
o.messhandle:	TCPIPLIBS:machine.h.ansi
o.messhandle:	TCPIPLIBS:machine.h.types
o.messhandle:	C:TCPIPLibs.netinet.h.in
o.messhandle:	C:TCPIPLibs.h.unixlib
o.messhandle:	h.Windows
o.messhandle:	C:DebugLib.h.DebugLib
o.messhandle:	h.misc
o.messhandle:	h.main
o.misc:	c.misc
o.misc:	h.main
o.misc:	tbox:h.toolbox
o.misc:	C:h.kernel
o.misc:	tbox:h.wimp
o.misc:	tbox:h.window
o.misc:	tbox:h.gadgets
o.misc:	tbox:h.menu
o.misc:	tbox:h.event
o.misc:	tbox:h.wimp
o.misc:	tbox:h.wimplib
o.misc:	tbox:h.gadgets
o.misc:	tbox:h.iconbar
o.misc:	C:h.kernel
o.misc:	C:h.swis
o.misc:	C:ErrorLib.h.ErrorLib
o.misc:	C:MsgLib.h.MsgLib
o.misc:	C:NetFax.UILib.h.UILib
o.misc:	C:TCPIPLibs.sys.h.errno
o.misc:	C:h.kernel
o.misc:	C:TCPIPLibs.h.inetlib
o.misc:	C:TCPIPLibs.sys.h.types
o.misc:	TCPIPLIBS:sys.h.cdefs
o.misc:	TCPIPLIBS:machine.h.endian
o.misc:	TCPIPLIBS:machine.h.ansi
o.misc:	TCPIPLIBS:machine.h.types
o.misc:	C:TCPIPLibs.netinet.h.in
o.misc:	C:TCPIPLibs.h.unixlib
o.misc:	h.Windows
o.misc:	C:DebugLib.h.DebugLib
o.misc:	h.misc
o.openwin:	c.openwin
o.openwin:	h.main
o.openwin:	tbox:h.toolbox
o.openwin:	C:h.kernel
o.openwin:	tbox:h.wimp
o.openwin:	tbox:h.window
o.openwin:	tbox:h.gadgets
o.openwin:	tbox:h.menu
o.openwin:	tbox:h.event
o.openwin:	tbox:h.wimp
o.openwin:	tbox:h.wimplib
o.openwin:	tbox:h.gadgets
o.openwin:	tbox:h.iconbar
o.openwin:	C:h.kernel
o.openwin:	C:h.swis
o.openwin:	C:ErrorLib.h.ErrorLib
o.openwin:	C:MsgLib.h.MsgLib
o.openwin:	C:NetFax.UILib.h.UILib
o.openwin:	C:TCPIPLibs.sys.h.errno
o.openwin:	C:h.kernel
o.openwin:	C:TCPIPLibs.h.inetlib
o.openwin:	C:TCPIPLibs.sys.h.types
o.openwin:	TCPIPLIBS:sys.h.cdefs
o.openwin:	TCPIPLIBS:machine.h.endian
o.openwin:	TCPIPLIBS:machine.h.ansi
o.openwin:	TCPIPLIBS:machine.h.types
o.openwin:	C:TCPIPLibs.netinet.h.in
o.openwin:	C:TCPIPLibs.h.unixlib
o.openwin:	h.Windows
o.openwin:	C:DebugLib.h.DebugLib
o.openwin:	h.functions
o.openwin:	tbox:h.toolbox
o.openwin:	h.openwin
o.openwin:	h.main
o.openwin:	h.functions
o.runes:	c.runes
o.runes:	TCPIPLIBS:sys.h.errno
o.runes:	C:h.kernel
o.runes:	TCPIPLIBS:h.inetlib
o.runes:	TCPIPLIBS:sys.h.types
o.runes:	TCPIPLIBS:sys.h.cdefs
o.runes:	TCPIPLIBS:machine.h.endian
o.runes:	TCPIPLIBS:machine.h.ansi
o.runes:	TCPIPLIBS:machine.h.types
o.runes:	TCPIPLIBS:netinet.h.in
o.runes:	TCPIPLIBS:h.unixlib
o.runes:	h.main
o.runes:	tbox:h.toolbox
o.runes:	tbox:h.wimp
o.runes:	tbox:h.window
o.runes:	tbox:h.gadgets
o.runes:	tbox:h.menu
o.runes:	tbox:h.event
o.runes:	tbox:h.wimp
o.runes:	tbox:h.wimplib
o.runes:	tbox:h.gadgets
o.runes:	tbox:h.iconbar
o.runes:	C:h.kernel
o.runes:	C:h.swis
o.runes:	C:ErrorLib.h.ErrorLib
o.runes:	C:MsgLib.h.MsgLib
o.runes:	C:NetFax.UILib.h.UILib
o.runes:	C:TCPIPLibs.sys.h.errno
o.runes:	C:TCPIPLibs.h.inetlib
o.runes:	C:TCPIPLibs.sys.h.types
o.runes:	C:TCPIPLibs.netinet.h.in
o.runes:	C:TCPIPLibs.h.unixlib
o.runes:	h.Windows
o.runes:	C:DebugLib.h.DebugLib
o.runes:	h.functions
o.runes:	tbox:h.toolbox
o.runes:	h.datetime
o.runes:	h.main
o.runes:	h.functions
o.runes:	h.misc
o.runes:	C:h.swis
o.runes:	C:h.kernel
o.runes:	h.scheduler
o.runes:	h.main
o.runes:	h.misc
o.runes:	h.messhandle
o.runes:	tbox:h.toolbox
o.runes:	h.ui
o.runes:	h.main
o.runes:	h.functions
o.runes:	C:h.kernel
o.runes:	h.misc
o.runes:	h.scheduler
o.runes:	h.load
o.runes:	h.main
o.runes:	h.functions
o.runes:	C:h.swis
o.runes:	C:h.kernel
o.runes:	h.misc
o.runes:	h.messhandle
o.runes:	h.save
o.runes:	h.main
o.runes:	h.functions
o.runes:	h.messhandle
o.runes:	h.misc
o.runes:	h.cancel
o.runes:	h.load
o.runes:	h.misc
o.save:	c.save
o.save:	h.main
o.save:	tbox:h.toolbox
o.save:	C:h.kernel
o.save:	tbox:h.wimp
o.save:	tbox:h.window
o.save:	tbox:h.gadgets
o.save:	tbox:h.menu
o.save:	tbox:h.event
o.save:	tbox:h.wimp
o.save:	tbox:h.wimplib
o.save:	tbox:h.gadgets
o.save:	tbox:h.iconbar
o.save:	C:h.kernel
o.save:	C:h.swis
o.save:	C:ErrorLib.h.ErrorLib
o.save:	C:MsgLib.h.MsgLib
o.save:	C:NetFax.UILib.h.UILib
o.save:	C:TCPIPLibs.sys.h.errno
o.save:	C:h.kernel
o.save:	C:TCPIPLibs.h.inetlib
o.save:	C:TCPIPLibs.sys.h.types
o.save:	TCPIPLIBS:sys.h.cdefs
o.save:	TCPIPLIBS:machine.h.endian
o.save:	TCPIPLIBS:machine.h.ansi
o.save:	TCPIPLIBS:machine.h.types
o.save:	C:TCPIPLibs.netinet.h.in
o.save:	C:TCPIPLibs.h.unixlib
o.save:	h.Windows
o.save:	C:DebugLib.h.DebugLib
o.save:	h.functions
o.save:	tbox:h.toolbox
o.save:	h.messhandle
o.save:	tbox:h.toolbox
o.save:	h.misc
o.save:	h.scheduler
o.save:	h.main
o.save:	h.misc
o.save:	C:numconvert.h.numconvert
o.save:	h.ui
o.save:	h.main
o.save:	h.functions
o.save:	C:h.kernel
o.save:	h.misc
o.save:	h.scheduler
o.save:	h.save
o.save:	h.main
o.save:	h.functions
o.save:	h.messhandle
o.save:	h.misc
o.save:	h.load
o.save:	h.main
o.save:	h.functions
o.save:	C:h.swis
o.save:	C:h.kernel
o.save:	h.misc
o.save:	h.messhandle
o.scheduler:	c.scheduler
o.scheduler:	h.main
o.scheduler:	tbox:h.toolbox
o.scheduler:	C:h.kernel
o.scheduler:	tbox:h.wimp
o.scheduler:	tbox:h.window
o.scheduler:	tbox:h.gadgets
o.scheduler:	tbox:h.menu
o.scheduler:	tbox:h.event
o.scheduler:	tbox:h.wimp
o.scheduler:	tbox:h.wimplib
o.scheduler:	tbox:h.gadgets
o.scheduler:	tbox:h.iconbar
o.scheduler:	C:h.kernel
o.scheduler:	C:h.swis
o.scheduler:	C:ErrorLib.h.ErrorLib
o.scheduler:	C:MsgLib.h.MsgLib
o.scheduler:	C:NetFax.UILib.h.UILib
o.scheduler:	C:TCPIPLibs.sys.h.errno
o.scheduler:	C:h.kernel
o.scheduler:	C:TCPIPLibs.h.inetlib
o.scheduler:	C:TCPIPLibs.sys.h.types
o.scheduler:	TCPIPLIBS:sys.h.cdefs
o.scheduler:	TCPIPLIBS:machine.h.endian
o.scheduler:	TCPIPLIBS:machine.h.ansi
o.scheduler:	TCPIPLIBS:machine.h.types
o.scheduler:	C:TCPIPLibs.netinet.h.in
o.scheduler:	C:TCPIPLibs.h.unixlib
o.scheduler:	h.Windows
o.scheduler:	C:DebugLib.h.DebugLib
o.scheduler:	h.misc
o.scheduler:	h.scheduler
o.scheduler:	h.main
o.scheduler:	h.misc
o.ui:	c.ui
o.ui:	h.main
o.ui:	tbox:h.toolbox
o.ui:	C:h.kernel
o.ui:	tbox:h.wimp
o.ui:	tbox:h.window
o.ui:	tbox:h.gadgets
o.ui:	tbox:h.menu
o.ui:	tbox:h.event
o.ui:	tbox:h.wimp
o.ui:	tbox:h.wimplib
o.ui:	tbox:h.gadgets
o.ui:	tbox:h.iconbar
o.ui:	C:h.kernel
o.ui:	C:h.swis
o.ui:	C:ErrorLib.h.ErrorLib
o.ui:	C:MsgLib.h.MsgLib
o.ui:	C:NetFax.UILib.h.UILib
o.ui:	C:TCPIPLibs.sys.h.errno
o.ui:	C:h.kernel
o.ui:	C:TCPIPLibs.h.inetlib
o.ui:	C:TCPIPLibs.sys.h.types
o.ui:	TCPIPLIBS:sys.h.cdefs
o.ui:	TCPIPLIBS:machine.h.endian
o.ui:	TCPIPLIBS:machine.h.ansi
o.ui:	TCPIPLIBS:machine.h.types
o.ui:	C:TCPIPLibs.netinet.h.in
o.ui:	C:TCPIPLibs.h.unixlib
o.ui:	h.Windows
o.ui:	C:DebugLib.h.DebugLib
o.ui:	h.ui
o.ui:	h.main
o.ui:	h.functions
o.ui:	tbox:h.toolbox
o.ui:	C:h.kernel
o.ui:	h.misc
o.ui:	h.scheduler
o.ui:	h.main
o.ui:	h.misc
o.ui:	h.functions
o.ui:	C:h.kernel
o.ui:	h.misc
o.ui:	h.load
o.ui:	h.main
o.ui:	h.functions
o.ui:	C:h.swis
o.ui:	C:h.kernel
o.ui:	h.misc
o.ui:	h.messhandle
o.ui:	tbox:h.toolbox
o.ui:	h.scheduler
o.cancel:	c.cancel
o.cancel:	C:h.kernel
o.cancel:	h.datetime
o.cancel:	h.main
o.cancel:	tbox:h.toolbox
o.cancel:	tbox:h.wimp
o.cancel:	tbox:h.window
o.cancel:	tbox:h.gadgets
o.cancel:	tbox:h.menu
o.cancel:	tbox:h.event
o.cancel:	tbox:h.wimp
o.cancel:	tbox:h.wimplib
o.cancel:	tbox:h.gadgets
o.cancel:	tbox:h.iconbar
o.cancel:	C:h.kernel
o.cancel:	C:h.swis
o.cancel:	C:ErrorLib.h.ErrorLib
o.cancel:	C:MsgLib.h.MsgLib
o.cancel:	C:NetFax.UILib.h.UILib
o.cancel:	C:TCPIPLibs.sys.h.errno
o.cancel:	C:h.kernel
o.cancel:	C:TCPIPLibs.h.inetlib
o.cancel:	C:TCPIPLibs.sys.h.types
o.cancel:	TCPIPLIBS:sys.h.cdefs
o.cancel:	TCPIPLIBS:machine.h.endian
o.cancel:	TCPIPLIBS:machine.h.ansi
o.cancel:	TCPIPLIBS:machine.h.types
o.cancel:	C:TCPIPLibs.netinet.h.in
o.cancel:	C:TCPIPLibs.h.unixlib
o.cancel:	h.Windows
o.cancel:	C:DebugLib.h.DebugLib
o.cancel:	h.functions
o.cancel:	tbox:h.toolbox
o.cancel:	h.misc
o.cancel:	C:h.swis
o.cancel:	C:h.kernel
o.cancel:	C:h.swis
o.cancel:	h.cancel
o.cancel:	h.load
o.cancel:	h.main
o.cancel:	h.functions
o.cancel:	C:h.swis
o.cancel:	C:h.kernel
o.cancel:	h.misc
o.cancel:	h.messhandle
o.cancel:	tbox:h.toolbox
o.cancel:	h.misc
o.cancel:	h.load
o.cancel:	h.misc
