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
 ${NUMCONVERT}zm


LIBS = \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \
 ${NUMCONVERT}


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

INCLUDES	= -Itbox:,C:

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
od.datetime:	h.Windows
od.datetime:	h.structs
od.datetime:	C:DebugLib.h.DebugLib
od.datetime:	h.functions
od.datetime:	tbox:h.toolbox
od.datetime:	C:h.time
od.datetime:	h.misc
od.datetime:	C:h.swis
od.datetime:	C:h.kernel
od.datetime:	h.datetime
od.datetime:	h.main
od.datetime:	h.functions
od.datetime:	C:h.time
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
od.fontsetup:	h.Windows
od.fontsetup:	h.structs
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
od.load:	h.Windows
od.load:	h.structs
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
od.load:	h.main
od.load:	h.misc
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
od.main:	h.Windows
od.main:	h.structs
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
od.messhandle:	h.Windows
od.messhandle:	h.structs
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
od.misc:	h.Windows
od.misc:	h.structs
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
od.openwin:	h.Windows
od.openwin:	h.structs
od.openwin:	C:DebugLib.h.DebugLib
od.openwin:	h.functions
od.openwin:	tbox:h.toolbox
od.openwin:	h.openwin
od.openwin:	h.main
od.openwin:	h.functions
od.runes:	c.runes
od.runes:	h.main
od.runes:	tbox:h.toolbox
od.runes:	C:h.kernel
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
od.runes:	h.Windows
od.runes:	h.structs
od.runes:	C:DebugLib.h.DebugLib
od.runes:	h.functions
od.runes:	tbox:h.toolbox
od.runes:	h.datetime
od.runes:	h.main
od.runes:	h.functions
od.runes:	C:h.time
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
od.runes:	h.main
od.runes:	h.misc
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
od.save:	h.Windows
od.save:	h.structs
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
od.save:	h.main
od.save:	h.misc
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
od.scheduler:	h.Windows
od.scheduler:	h.structs
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
od.ui:	h.Windows
od.ui:	h.structs
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
od.ui:	h.main
od.ui:	h.misc
