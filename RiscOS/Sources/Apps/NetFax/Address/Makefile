# Makefile for Daytona Address Book
# =================================
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

COMPONENT	= DayAdd
APP		= !DayAdd
MODULE		= rm.!DayAdd
ROM_MODULE	= aof.DayAdd
LDIR		= Resources.${LOCALE}

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
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS		= ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wp
CPFLAGS		= ~cfr~v~n
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS		= 
DDFLAGS		= ${DFLAGS} -fn
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

EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
TEXTGADGETLIB	= tbox:o.textgadget

UILIB		= ${LIBDIR}.UILib.o.UILib
ERRORLIB	= ${LIBDIR}.ErrorLib.o.ErrorLib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
FLASHLIB	= ${LIBDIR}.FlashLib.o.FlashLib


LIBSM = \
 ${FLASHLIB} \
 ${UILIB}zm \
 ${MSGLIB}zm \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \


LIBS = \
 ${FLASHLIB} \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \


LIBSD = \
 ${FLASHLIB} \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,${LIBDIR}.FlashLib,${LIBDIR}.UILib,${LIBDIR}.MsgLib,${LIBDIR}.ErrorLib,${LIBDIR}.DebugLib

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
 o.addedit \
 o.error \
 o.flash \
 o.fontsetup \
 o.main \
 o.menuevents \
 o.memhandle\
 o.misc \
 o.miscwimp \
 o.numconvert \
 o.runes \
 o.search \
 o.strings \


OBJSZ = \
 oz.addedit \
 oz.error \
 oz.flash \
 oz.fontsetup \
 oz.main \
 oz.menuevents \
 oz.misc \
 oz.miscwimp \
 oz.memhandle\
 oz.numconvert \
 oz.runes \
 oz.search \
 oz.strings \


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
		@${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

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
	-Access ${INSTDIR}.!Boot	lr/r
	-Access ${INSTDIR}.!Run		lr/r
	-Access ${INSTDIR}.!RunImage	lr/r
	-Access ${INSTDIR}.Messages	lr/r
	-Access ${INSTDIR}.Res		lr/r
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}
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
	@${MKDIR} Targets.!DayAdd
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
	${LD} -debug -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
	@echo
	${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.!DayAddD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
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
oz.addedit:	c.addedit
oz.addedit:	h.include
oz.addedit:	C:h.swis
oz.addedit:	C:h.kernel
oz.addedit:	h.tbox
oz.addedit:	tbox:h.event
oz.addedit:	tbox:h.wimp
oz.addedit:	tbox:h.toolbox
oz.addedit:	tbox:h.toolbox
oz.addedit:	tbox:h.window
oz.addedit:	tbox:h.gadgets
oz.addedit:	tbox:h.gadgets
oz.addedit:	tbox:h.menu
oz.addedit:	tbox:h.proginfo
oz.addedit:	tbox:h.iconbar
oz.addedit:	tbox:h.quit
oz.addedit:	tbox:h.saveas
oz.addedit:	tbox:h.wimplib
oz.addedit:	<Lib$Dir>.MsgLib.h.MsgLib
oz.addedit:	<Lib$Dir>.DebugLib.h.DebugLib
oz.addedit:	<Lib$Dir>.UILib.h.UILib
oz.addedit:	h.state
oz.addedit:	h.flash
oz.addedit:	h.misc
oz.addedit:	h.miscwimp
oz.addedit:	h.runes
oz.addedit:	h.error
oz.addedit:	h.numconvert
oz.addedit:	h.strings
oz.addedit:	h.addedit
oz.addedit:	h.globals
oz.error:	c.error
oz.error:	h.include
oz.error:	C:h.swis
oz.error:	C:h.kernel
oz.error:	h.tbox
oz.error:	tbox:h.event
oz.error:	tbox:h.wimp
oz.error:	tbox:h.toolbox
oz.error:	tbox:h.toolbox
oz.error:	tbox:h.window
oz.error:	tbox:h.gadgets
oz.error:	tbox:h.gadgets
oz.error:	tbox:h.menu
oz.error:	tbox:h.proginfo
oz.error:	tbox:h.iconbar
oz.error:	tbox:h.quit
oz.error:	tbox:h.saveas
oz.error:	tbox:h.wimplib
oz.error:	<Lib$Dir>.MsgLib.h.MsgLib
oz.error:	<Lib$Dir>.DebugLib.h.DebugLib
oz.error:	<Lib$Dir>.UILib.h.UILib
oz.error:	h.state
oz.error:	h.miscwimp
oz.error:	h.runes
oz.error:	h.error
oz.error:	h.globals
oz.flash:	c.flash
oz.flash:	h.include
oz.flash:	C:h.swis
oz.flash:	C:h.kernel
oz.flash:	h.tbox
oz.flash:	tbox:h.event
oz.flash:	tbox:h.wimp
oz.flash:	tbox:h.toolbox
oz.flash:	tbox:h.toolbox
oz.flash:	tbox:h.window
oz.flash:	tbox:h.gadgets
oz.flash:	tbox:h.gadgets
oz.flash:	tbox:h.menu
oz.flash:	tbox:h.proginfo
oz.flash:	tbox:h.iconbar
oz.flash:	tbox:h.quit
oz.flash:	tbox:h.saveas
oz.flash:	tbox:h.wimplib
oz.flash:	<Lib$Dir>.MsgLib.h.MsgLib
oz.flash:	<Lib$Dir>.DebugLib.h.DebugLib
oz.flash:	<Lib$Dir>.UILib.h.UILib
oz.flash:	h.state
oz.flash:	h.misc
oz.flash:	tbox:^.flashlib.h.flashlib
oz.flash:	h.globals
oz.flash:	h.error
oz.flash:	h.flash
oz.fontsetup:	c.fontsetup
oz.fontsetup:	h.include
oz.fontsetup:	C:h.swis
oz.fontsetup:	C:h.kernel
oz.fontsetup:	h.tbox
oz.fontsetup:	tbox:h.event
oz.fontsetup:	tbox:h.wimp
oz.fontsetup:	tbox:h.toolbox
oz.fontsetup:	tbox:h.toolbox
oz.fontsetup:	tbox:h.window
oz.fontsetup:	tbox:h.gadgets
oz.fontsetup:	tbox:h.gadgets
oz.fontsetup:	tbox:h.menu
oz.fontsetup:	tbox:h.proginfo
oz.fontsetup:	tbox:h.iconbar
oz.fontsetup:	tbox:h.quit
oz.fontsetup:	tbox:h.saveas
oz.fontsetup:	tbox:h.wimplib
oz.fontsetup:	<Lib$Dir>.MsgLib.h.MsgLib
oz.fontsetup:	<Lib$Dir>.DebugLib.h.DebugLib
oz.fontsetup:	<Lib$Dir>.UILib.h.UILib
oz.fontsetup:	h.state
oz.fontsetup:	h.fontsetup
oz.fontsetup:	h.globals
oz.main:	c.main
oz.main:	h.include
oz.main:	C:h.swis
oz.main:	C:h.kernel
oz.main:	h.tbox
oz.main:	tbox:h.event
oz.main:	tbox:h.wimp
oz.main:	tbox:h.toolbox
oz.main:	tbox:h.toolbox
oz.main:	tbox:h.window
oz.main:	tbox:h.gadgets
oz.main:	tbox:h.gadgets
oz.main:	tbox:h.menu
oz.main:	tbox:h.proginfo
oz.main:	tbox:h.iconbar
oz.main:	tbox:h.quit
oz.main:	tbox:h.saveas
oz.main:	tbox:h.wimplib
oz.main:	<Lib$Dir>.MsgLib.h.MsgLib
oz.main:	<Lib$Dir>.DebugLib.h.DebugLib
oz.main:	<Lib$Dir>.UILib.h.UILib
oz.main:	h.state
oz.main:	h.menuevents
oz.main:	h.runes
oz.main:	h.fontsetup
oz.main:	h.flash
oz.main:	h.error
oz.main:	h.search
oz.main:	h.globals
oz.menuevents:	c.menuevents
oz.menuevents:	h.include
oz.menuevents:	C:h.swis
oz.menuevents:	C:h.kernel
oz.menuevents:	h.tbox
oz.menuevents:	tbox:h.event
oz.menuevents:	tbox:h.wimp
oz.menuevents:	tbox:h.toolbox
oz.menuevents:	tbox:h.toolbox
oz.menuevents:	tbox:h.window
oz.menuevents:	tbox:h.gadgets
oz.menuevents:	tbox:h.gadgets
oz.menuevents:	tbox:h.menu
oz.menuevents:	tbox:h.proginfo
oz.menuevents:	tbox:h.iconbar
oz.menuevents:	tbox:h.quit
oz.menuevents:	tbox:h.saveas
oz.menuevents:	tbox:h.wimplib
oz.menuevents:	<Lib$Dir>.MsgLib.h.MsgLib
oz.menuevents:	<Lib$Dir>.DebugLib.h.DebugLib
oz.menuevents:	<Lib$Dir>.UILib.h.UILib
oz.menuevents:	h.state
oz.menuevents:	h.error
oz.menuevents:	h.menuevents
oz.menuevents:	h.runes
oz.menuevents:	h.search
oz.menuevents:	h.globals
oz.misc:	c.misc
oz.misc:	h.include
oz.misc:	C:h.swis
oz.misc:	C:h.kernel
oz.misc:	<Lib$Dir>.DebugLib.h.DebugLib
oz.misc:	h.numconvert
oz.misc:	h.strings
oz.misc:	h.misc
oz.miscwimp:	c.miscwimp
oz.miscwimp:	h.include
oz.miscwimp:	C:h.swis
oz.miscwimp:	C:h.kernel
oz.miscwimp:	h.tbox
oz.miscwimp:	tbox:h.event
oz.miscwimp:	tbox:h.wimp
oz.miscwimp:	tbox:h.toolbox
oz.miscwimp:	tbox:h.toolbox
oz.miscwimp:	tbox:h.window
oz.miscwimp:	tbox:h.gadgets
oz.miscwimp:	tbox:h.gadgets
oz.miscwimp:	tbox:h.menu
oz.miscwimp:	tbox:h.proginfo
oz.miscwimp:	tbox:h.iconbar
oz.miscwimp:	tbox:h.quit
oz.miscwimp:	tbox:h.saveas
oz.miscwimp:	tbox:h.wimplib
oz.miscwimp:	<Lib$Dir>.MsgLib.h.MsgLib
oz.miscwimp:	<Lib$Dir>.DebugLib.h.DebugLib
oz.miscwimp:	<Lib$Dir>.UILib.h.UILib
oz.miscwimp:	h.state
oz.miscwimp:	h.error
oz.miscwimp:	h.globals
oz.miscwimp:	h.miscwimp
oz.numconvert:	c.numconvert
oz.numconvert:	h.numconvert
oz.runes:	c.runes
oz.runes:	h.include
oz.runes:	C:h.swis
oz.runes:	C:h.kernel
oz.runes:	h.tbox
oz.runes:	tbox:h.event
oz.runes:	tbox:h.wimp
oz.runes:	tbox:h.toolbox
oz.runes:	tbox:h.toolbox
oz.runes:	tbox:h.window
oz.runes:	tbox:h.gadgets
oz.runes:	tbox:h.gadgets
oz.runes:	tbox:h.menu
oz.runes:	tbox:h.proginfo
oz.runes:	tbox:h.iconbar
oz.runes:	tbox:h.quit
oz.runes:	tbox:h.saveas
oz.runes:	tbox:h.wimplib
oz.runes:	<Lib$Dir>.MsgLib.h.MsgLib
oz.runes:	<Lib$Dir>.DebugLib.h.DebugLib
oz.runes:	<Lib$Dir>.UILib.h.UILib
oz.runes:	h.state
oz.runes:	h.error
oz.runes:	h.runes
oz.runes:	h.miscwimp
oz.runes:	h.addedit
oz.runes:	h.misc
oz.runes:	h.numconvert
oz.runes:	h.flash
oz.runes:	h.search
oz.runes:	h.globals
oz.search:	c.search
oz.search:	h.include
oz.search:	C:h.swis
oz.search:	C:h.kernel
oz.search:	h.tbox
oz.search:	tbox:h.event
oz.search:	tbox:h.wimp
oz.search:	tbox:h.toolbox
oz.search:	tbox:h.toolbox
oz.search:	tbox:h.window
oz.search:	tbox:h.gadgets
oz.search:	tbox:h.gadgets
oz.search:	tbox:h.menu
oz.search:	tbox:h.proginfo
oz.search:	tbox:h.iconbar
oz.search:	tbox:h.quit
oz.search:	tbox:h.saveas
oz.search:	tbox:h.wimplib
oz.search:	<Lib$Dir>.MsgLib.h.MsgLib
oz.search:	<Lib$Dir>.DebugLib.h.DebugLib
oz.search:	<Lib$Dir>.UILib.h.UILib
oz.search:	h.state
oz.search:	C:h.ctype
oz.search:	C:h.time
oz.search:	h.globals
oz.search:	h.misc
oz.search:	h.miscwimp
oz.search:	h.numconvert
oz.search:	h.strings
oz.search:	h.flash
oz.search:	h.runes
oz.search:	h.error
oz.search:	h.search
oz.strings:	c.strings
oz.strings:	h.include
oz.strings:	C:h.swis
oz.strings:	C:h.kernel
oz.strings:	h.tbox
oz.strings:	tbox:h.event
oz.strings:	tbox:h.wimp
oz.strings:	tbox:h.toolbox
oz.strings:	tbox:h.toolbox
oz.strings:	tbox:h.window
oz.strings:	tbox:h.gadgets
oz.strings:	tbox:h.gadgets
oz.strings:	tbox:h.menu
oz.strings:	tbox:h.proginfo
oz.strings:	tbox:h.iconbar
oz.strings:	tbox:h.quit
oz.strings:	tbox:h.saveas
oz.strings:	tbox:h.wimplib
oz.strings:	<Lib$Dir>.MsgLib.h.MsgLib
oz.strings:	<Lib$Dir>.DebugLib.h.DebugLib
oz.strings:	<Lib$Dir>.UILib.h.UILib
oz.strings:	h.state
oz.strings:	h.strings
oz.strings:	h.misc
oz.strings:	C:h.ctype
o.addedit:	c.addedit
o.addedit:	h.include
o.addedit:	C:h.swis
o.addedit:	C:h.kernel
o.addedit:	h.tbox
o.addedit:	tbox:h.event
o.addedit:	tbox:h.wimp
o.addedit:	tbox:h.toolbox
o.addedit:	tbox:h.toolbox
o.addedit:	tbox:h.window
o.addedit:	tbox:h.gadgets
o.addedit:	tbox:h.gadgets
o.addedit:	tbox:h.menu
o.addedit:	tbox:h.proginfo
o.addedit:	tbox:h.iconbar
o.addedit:	tbox:h.quit
o.addedit:	tbox:h.saveas
o.addedit:	tbox:h.wimplib
o.addedit:	<Lib$Dir>.MsgLib.h.MsgLib
o.addedit:	<Lib$Dir>.DebugLib.h.DebugLib
o.addedit:	<Lib$Dir>.UILib.h.UILib
o.addedit:	h.state
o.addedit:	h.flash
o.addedit:	h.misc
o.addedit:	h.miscwimp
o.addedit:	h.runes
o.addedit:	h.error
o.addedit:	h.numconvert
o.addedit:	h.strings
o.addedit:	h.addedit
o.addedit:	h.globals
o.error:	c.error
o.error:	h.include
o.error:	C:h.swis
o.error:	C:h.kernel
o.error:	h.tbox
o.error:	tbox:h.event
o.error:	tbox:h.wimp
o.error:	tbox:h.toolbox
o.error:	tbox:h.toolbox
o.error:	tbox:h.window
o.error:	tbox:h.gadgets
o.error:	tbox:h.gadgets
o.error:	tbox:h.menu
o.error:	tbox:h.proginfo
o.error:	tbox:h.iconbar
o.error:	tbox:h.quit
o.error:	tbox:h.saveas
o.error:	tbox:h.wimplib
o.error:	<Lib$Dir>.MsgLib.h.MsgLib
o.error:	<Lib$Dir>.DebugLib.h.DebugLib
o.error:	<Lib$Dir>.UILib.h.UILib
o.error:	h.state
o.error:	h.miscwimp
o.error:	h.runes
o.error:	h.error
o.error:	h.globals
o.flash:	c.flash
o.flash:	h.include
o.flash:	C:h.swis
o.flash:	C:h.kernel
o.flash:	h.tbox
o.flash:	tbox:h.event
o.flash:	tbox:h.wimp
o.flash:	tbox:h.toolbox
o.flash:	tbox:h.toolbox
o.flash:	tbox:h.window
o.flash:	tbox:h.gadgets
o.flash:	tbox:h.gadgets
o.flash:	tbox:h.menu
o.flash:	tbox:h.proginfo
o.flash:	tbox:h.iconbar
o.flash:	tbox:h.quit
o.flash:	tbox:h.saveas
o.flash:	tbox:h.wimplib
o.flash:	<Lib$Dir>.MsgLib.h.MsgLib
o.flash:	<Lib$Dir>.DebugLib.h.DebugLib
o.flash:	<Lib$Dir>.UILib.h.UILib
o.flash:	h.state
o.flash:	h.misc
o.flash:	tbox:^.flashlib.h.flashlib
o.flash:	h.globals
o.flash:	h.error
o.flash:	h.flash
o.fontsetup:	c.fontsetup
o.fontsetup:	h.include
o.fontsetup:	C:h.swis
o.fontsetup:	C:h.kernel
o.fontsetup:	h.tbox
o.fontsetup:	tbox:h.event
o.fontsetup:	tbox:h.wimp
o.fontsetup:	tbox:h.toolbox
o.fontsetup:	tbox:h.toolbox
o.fontsetup:	tbox:h.window
o.fontsetup:	tbox:h.gadgets
o.fontsetup:	tbox:h.gadgets
o.fontsetup:	tbox:h.menu
o.fontsetup:	tbox:h.proginfo
o.fontsetup:	tbox:h.iconbar
o.fontsetup:	tbox:h.quit
o.fontsetup:	tbox:h.saveas
o.fontsetup:	tbox:h.wimplib
o.fontsetup:	<Lib$Dir>.MsgLib.h.MsgLib
o.fontsetup:	<Lib$Dir>.DebugLib.h.DebugLib
o.fontsetup:	<Lib$Dir>.UILib.h.UILib
o.fontsetup:	h.state
o.fontsetup:	h.fontsetup
o.fontsetup:	h.globals
o.main:	c.main
o.main:	h.include
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	h.tbox
o.main:	tbox:h.event
o.main:	tbox:h.wimp
o.main:	tbox:h.toolbox
o.main:	tbox:h.toolbox
o.main:	tbox:h.window
o.main:	tbox:h.gadgets
o.main:	tbox:h.gadgets
o.main:	tbox:h.menu
o.main:	tbox:h.proginfo
o.main:	tbox:h.iconbar
o.main:	tbox:h.quit
o.main:	tbox:h.saveas
o.main:	tbox:h.wimplib
o.main:	<Lib$Dir>.MsgLib.h.MsgLib
o.main:	<Lib$Dir>.DebugLib.h.DebugLib
o.main:	<Lib$Dir>.UILib.h.UILib
o.main:	h.state
o.main:	h.menuevents
o.main:	h.runes
o.main:	h.fontsetup
o.main:	h.flash
o.main:	h.error
o.main:	h.search
o.main:	h.addrbookIF
o.main:	h.memhandle
o.main:	h.globals
o.menuevents:	c.menuevents
o.menuevents:	h.include
o.menuevents:	C:h.swis
o.menuevents:	C:h.kernel
o.menuevents:	h.tbox
o.menuevents:	tbox:h.event
o.menuevents:	tbox:h.wimp
o.menuevents:	tbox:h.toolbox
o.menuevents:	tbox:h.toolbox
o.menuevents:	tbox:h.window
o.menuevents:	tbox:h.gadgets
o.menuevents:	tbox:h.gadgets
o.menuevents:	tbox:h.menu
o.menuevents:	tbox:h.proginfo
o.menuevents:	tbox:h.iconbar
o.menuevents:	tbox:h.quit
o.menuevents:	tbox:h.saveas
o.menuevents:	tbox:h.wimplib
o.menuevents:	<Lib$Dir>.MsgLib.h.MsgLib
o.menuevents:	<Lib$Dir>.DebugLib.h.DebugLib
o.menuevents:	<Lib$Dir>.UILib.h.UILib
o.menuevents:	h.state
o.menuevents:	h.error
o.menuevents:	h.menuevents
o.menuevents:	h.runes
o.menuevents:	h.search
o.menuevents:	h.addrbookIF
o.menuevents:	h.globals
o.misc:	c.misc
o.misc:	h.include
o.misc:	C:h.swis
o.misc:	C:h.kernel
o.misc:	<Lib$Dir>.DebugLib.h.DebugLib
o.misc:	h.numconvert
o.misc:	h.strings
o.misc:	h.misc
o.miscwimp:	c.miscwimp
o.miscwimp:	h.include
o.miscwimp:	C:h.swis
o.miscwimp:	C:h.kernel
o.miscwimp:	h.tbox
o.miscwimp:	tbox:h.event
o.miscwimp:	tbox:h.wimp
o.miscwimp:	tbox:h.toolbox
o.miscwimp:	tbox:h.toolbox
o.miscwimp:	tbox:h.window
o.miscwimp:	tbox:h.gadgets
o.miscwimp:	tbox:h.gadgets
o.miscwimp:	tbox:h.menu
o.miscwimp:	tbox:h.proginfo
o.miscwimp:	tbox:h.iconbar
o.miscwimp:	tbox:h.quit
o.miscwimp:	tbox:h.saveas
o.miscwimp:	tbox:h.wimplib
o.miscwimp:	<Lib$Dir>.MsgLib.h.MsgLib
o.miscwimp:	<Lib$Dir>.DebugLib.h.DebugLib
o.miscwimp:	<Lib$Dir>.UILib.h.UILib
o.miscwimp:	h.state
o.miscwimp:	h.error
o.miscwimp:	h.globals
o.miscwimp:	h.miscwimp
o.numconvert:	c.numconvert
o.numconvert:	h.numconvert
o.runes:	c.runes
o.runes:	h.include
o.runes:	C:h.swis
o.runes:	C:h.kernel
o.runes:	h.tbox
o.runes:	tbox:h.event
o.runes:	tbox:h.wimp
o.runes:	tbox:h.toolbox
o.runes:	tbox:h.toolbox
o.runes:	tbox:h.window
o.runes:	tbox:h.gadgets
o.runes:	tbox:h.gadgets
o.runes:	tbox:h.menu
o.runes:	tbox:h.proginfo
o.runes:	tbox:h.iconbar
o.runes:	tbox:h.quit
o.runes:	tbox:h.saveas
o.runes:	tbox:h.wimplib
o.runes:	<Lib$Dir>.MsgLib.h.MsgLib
o.runes:	<Lib$Dir>.DebugLib.h.DebugLib
o.runes:	<Lib$Dir>.UILib.h.UILib
o.runes:	h.state
o.runes:	h.error
o.runes:	h.runes
o.runes:	h.miscwimp
o.runes:	h.addedit
o.runes:	h.misc
o.runes:	h.numconvert
o.runes:	h.flash
o.runes:	h.search
o.runes:	h.globals
o.search:	c.search
o.search:	h.include
o.search:	C:h.swis
o.search:	C:h.kernel
o.search:	h.tbox
o.search:	tbox:h.event
o.search:	tbox:h.wimp
o.search:	tbox:h.toolbox
o.search:	tbox:h.toolbox
o.search:	tbox:h.window
o.search:	tbox:h.gadgets
o.search:	tbox:h.gadgets
o.search:	tbox:h.menu
o.search:	tbox:h.proginfo
o.search:	tbox:h.iconbar
o.search:	tbox:h.quit
o.search:	tbox:h.saveas
o.search:	tbox:h.wimplib
o.search:	<Lib$Dir>.MsgLib.h.MsgLib
o.search:	<Lib$Dir>.DebugLib.h.DebugLib
o.search:	<Lib$Dir>.UILib.h.UILib
o.search:	h.state
o.search:	C:h.ctype
o.search:	C:h.time
o.search:	h.globals
o.search:	h.misc
o.search:	h.miscwimp
o.search:	h.numconvert
o.search:	h.strings
o.search:	h.flash
o.search:	h.runes
o.search:	h.error
o.search:	h.addrbookIF
o.search:	h.memhandle
o.search:	h.search
o.strings:	c.strings
o.strings:	h.include
o.strings:	C:h.swis
o.strings:	C:h.kernel
o.strings:	h.tbox
o.strings:	tbox:h.event
o.strings:	tbox:h.wimp
o.strings:	tbox:h.toolbox
o.strings:	tbox:h.toolbox
o.strings:	tbox:h.window
o.strings:	tbox:h.gadgets
o.strings:	tbox:h.gadgets
o.strings:	tbox:h.menu
o.strings:	tbox:h.proginfo
o.strings:	tbox:h.iconbar
o.strings:	tbox:h.quit
o.strings:	tbox:h.saveas
o.strings:	tbox:h.wimplib
o.strings:	<Lib$Dir>.MsgLib.h.MsgLib
o.strings:	<Lib$Dir>.DebugLib.h.DebugLib
o.strings:	<Lib$Dir>.UILib.h.UILib
o.strings:	h.state
o.strings:	h.strings
o.strings:	h.misc
o.strings:	C:h.ctype
o.memhandle:	c.memhandle
o.memhandle:	h.include
o.memhandle:	C:h.swis
o.memhandle:	C:h.kernel
o.memhandle:	<Lib$Dir>.DebugLib.h.DebugLib
o.memhandle:	h.addrbookIF
o.memhandle:	h.memhandle
