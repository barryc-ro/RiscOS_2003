# Makefile for Daytona ENote editor
# =================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1997-12-16    BAL     Created

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = DayNote
APP             = !DayNote
MODULE          = rm.!DayNote
ROM_MODULE      = aof.DayNote
LDIR            = Resources.${LOCALE}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = ${RESDIR}.${COMPONENT}
RESAPP          = <Resource$Dir>.Apps.${APP}


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR           = cdir
AS              = objasm
CC              = cc
CMHG            = cmhg
CP              = copy
LD              = link
RM              = remove
SQUEEZE         = squeeze
WIPE            = -wipe

AFLAGS          = ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS          = ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wp
CPFLAGS         = ~cfr~v~n
SQFLAGS         = -f
WFLAGS          = ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS          = 
DDFLAGS         = ${DFLAGS} -DDEBUGLIB -fn
DZFLAGS         = 
DDZFLAGS        = ${DZFLAGS} -fn


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR          = <Lib$Dir>

CLIB            = CLib:o.stubs
ROMCSTUBS       = RISC_OSLib:o.romcstubs
ABSSYM          = RISC_OSLib:o.c_abssym
WRAPPER         = RISC_OSLib:s.ModuleWrap

EVENTLIB        = tbox:o.eventlib
TOOLBOXLIB      = tbox:o.toolboxlib
WIMPLIB         = tbox:o.wimplib
TEXTGADGETLIB	= tbox:o.textgadget

UILIB		= ${LIBDIR}.Netfax.UILib.o.UILib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
FLASHLIB	= ${LIBDIR}.Netfax.FlashLib.o.FlashLib
FLASHLIB_T	= ${LIBDIR}.Netfax.FlashLib.o.FlashLib_t
DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglib

LIBS = \
 ${FLASHLIB} \
 ${UILIB} \
 ${MSGLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB}


LIBSM = \
 ${FLASHLIB} \
 ${UILIB}zm \
 ${MSGLIB}zm \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \


LIBSD = \
 ${FLASHLIB_T} \
 ${UILIB} \
 ${MSGLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \
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
 o.confmanIF \
 o.flash \
 o.memhandle \
 o.forms \
 o.notes \
 o.search \
 o.daytonaIF \
 o.error \
 o.fontsetup \
 o.main \
 o.menuevents \
 o.misc \
 o.miscwimp \
 o.runes \
 o.strings \


OBJSZ = \
 oz.confmanIF \
 oz.flash \
 oz.memhandle \
 oz.forms \
 oz.notes \
 oz.search \
 oz.daytonaIF \
 oz.error \
 oz.fontsetup \
 oz.main \
 oz.menuevents \
 oz.misc \
 oz.miscwimp \
 oz.runes \
 oz.strings \


OBJSD = \
 od.confmanIF \
 od.flash \
 od.memhandle \
 od.forms \
 od.notes \
 od.search \
 od.daytonaIF \
 od.error \
 od.fontsetup \
 od.main \
 od.menuevents \
 od.misc \
 od.miscwimp \
 od.runes \
 od.strings \


OBJSDZ = \
 odz.confmanIF \
 odz.flash \
 odz.memhandle \
 odz.forms \
 odz.notes \
 odz.search \
 odz.daytonaIF \
 odz.error \
 odz.fontsetup \
 odz.main \
 odz.menuevents \
 odz.misc \
 odz.miscwimp \
 odz.runes \
 odz.strings \


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;          @echo
                @echo Compiling $<
                @echo ${DFLAGS}
                @${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<

.c.oz:;         @echo
                @echo Compiling $<
                @echo ${DZFLAGS}
                @${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -zps1 -DROM -o $@ $<

.c.od:;         @echo
                @echo Compiling $<
                @echo ${DDFLAGS}
                @${CC} ${CFLAGS} ${DDFLAGS}       -c -o $@ $<

.c.odz:;        @echo
                @echo Compiling $<
                @echo ${DDZFLAGS}
                @${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -zps1 -DROM -o $@ $<

.s.o:;          @echo
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
        ${CP} ${LDIR}.!Boot                     ${INSTDIR}.!Boot                ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites                  ${INSTDIR}.!Sprites             ${CPFLAGS}
        ${CP} ${LDIR}.Messages                  ${INSTDIR}.Messages             ${CPFLAGS}
        ${CP} ${LDIR}.Res                       ${INSTDIR}.Res                  ${CPFLAGS}

install: install_common
        ${CP} ${LDIR}.!Run              ${INSTDIR}.!Run         ${CPFLAGS}
        ${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
        -Access ${INSTDIR}.!Boot	lr/r
	-Access ${INSTDIR}.!Run		lr/r
	-Access ${INSTDIR}.!RunImage	lr/r
	-Access ${INSTDIR}.Messages	lr/r
	-Access ${INSTDIR}.Res		lr/r
        @echo
        @echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
        ${CP} ${LDIR}.!RunT             ${INSTDIR}.!RunT        ${CPFLAGS}
        ${CP} ${TARGET}			${INSTDIR}.!RunImageT	${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Debug application installed ${INSTDIR}

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Module installed (ROM)

resources:
	@echo ${RESDIR}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESAPP}
        ${CP} ${LDIR}.ROM.!Run          ${RESAPP}.!Run          ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites          ${RESDIR}.!Sprites      ${CPFLAGS}
        ${CP} ${LDIR}.Messages          ${RESDIR}.Messages      ${CPFLAGS}
        ${CP} ${LDIR}.Res               ${RESDIR}.Res           ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Resource files copied to Messages module

clean:
        @echo starting
        ${WIPE} abs             ${WFLAGS}
        ${WIPE} aof             ${WFLAGS}
        ${WIPE} linked          ${WFLAGS}
        ${WIPE} map             ${WFLAGS}
        ${WIPE} o               ${WFLAGS}
        ${WIPE} od              ${WFLAGS}
        ${WIPE} oz              ${WFLAGS}
        ${WIPE} odz             ${WFLAGS}
        ${WIPE} rm              ${WFLAGS}
        ${RM}   s.AppName
        ${RM}   s.ModuleWrap
        ${RM}   o.dirs
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
        @${MKDIR} Targets.!DayNote
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
        ${WIPE} Targets         ${WFLAGS}
        ${WIPE} syms		${WFLAGS}
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

rm.!DayNoteD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
        @echo
        ${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
od.memhandle:	c.memhandle
od.memhandle:	h.include
od.memhandle:	C:h.swis
od.memhandle:	C:h.kernel
od.memhandle:	C:debuglib.h.DebugLib
od.memhandle:	h.addrbookIF
od.memhandle:	h.memhandle
od.error:	c.error
od.error:	h.include
od.error:	C:h.swis
od.error:	C:h.kernel
od.error:	h.tbox
od.error:	tbox:h.event
od.error:	tbox:h.wimp
od.error:	tbox:h.toolbox
od.error:	tbox:h.toolbox
od.error:	tbox:h.window
od.error:	tbox:h.gadgets
od.error:	tbox:h.gadgets
od.error:	tbox:h.menu
od.error:	tbox:h.proginfo
od.error:	tbox:h.iconbar
od.error:	tbox:h.quit
od.error:	tbox:h.wimplib
od.error:	tbox:h.textarea
od.error:	C:msglib.h.MsgLib
od.error:	C:debuglib.h.DebugLib
od.error:	C:netfax.uilib.h.UILib
od.error:	h.addrbookIF
od.error:	h.miscwimp
od.error:	h.runes
od.error:	h.error
od.error:	h.globals
od.fontsetup:	c.fontsetup
od.fontsetup:	h.include
od.fontsetup:	C:h.swis
od.fontsetup:	C:h.kernel
od.fontsetup:	h.tbox
od.fontsetup:	tbox:h.event
od.fontsetup:	tbox:h.wimp
od.fontsetup:	tbox:h.toolbox
od.fontsetup:	tbox:h.toolbox
od.fontsetup:	tbox:h.window
od.fontsetup:	tbox:h.gadgets
od.fontsetup:	tbox:h.gadgets
od.fontsetup:	tbox:h.menu
od.fontsetup:	tbox:h.proginfo
od.fontsetup:	tbox:h.iconbar
od.fontsetup:	tbox:h.quit
od.fontsetup:	tbox:h.wimplib
od.fontsetup:	tbox:h.textarea
od.fontsetup:	C:msglib.h.MsgLib
od.fontsetup:	C:debuglib.h.DebugLib
od.fontsetup:	C:netfax.uilib.h.UILib
od.fontsetup:	h.fontsetup
od.fontsetup:	h.addrbookIF
od.fontsetup:	h.globals
od.main:	c.main
od.main:	h.include
od.main:	C:h.swis
od.main:	C:h.kernel
od.main:	h.tbox
od.main:	tbox:h.event
od.main:	tbox:h.wimp
od.main:	tbox:h.toolbox
od.main:	tbox:h.toolbox
od.main:	tbox:h.window
od.main:	tbox:h.gadgets
od.main:	tbox:h.gadgets
od.main:	tbox:h.menu
od.main:	tbox:h.proginfo
od.main:	tbox:h.iconbar
od.main:	tbox:h.quit
od.main:	tbox:h.wimplib
od.main:	tbox:h.textarea
od.main:	C:msglib.h.MsgLib
od.main:	C:debuglib.h.DebugLib
od.main:	C:netfax.uilib.h.UILib
od.main:	h.addrbookIF
od.main:	h.daytonaIF
od.main:	h.confmanIF
od.main:	h.menuevents
od.main:	h.runes
od.main:	h.fontsetup
od.main:	h.forms
od.main:	h.notes
od.main:	h.flash
od.main:	h.search
od.main:	h.error
od.main:	h.globals
od.menuevents:	c.menuevents
od.menuevents:	h.include
od.menuevents:	C:h.swis
od.menuevents:	C:h.kernel
od.menuevents:	h.tbox
od.menuevents:	tbox:h.event
od.menuevents:	tbox:h.wimp
od.menuevents:	tbox:h.toolbox
od.menuevents:	tbox:h.toolbox
od.menuevents:	tbox:h.window
od.menuevents:	tbox:h.gadgets
od.menuevents:	tbox:h.gadgets
od.menuevents:	tbox:h.menu
od.menuevents:	tbox:h.proginfo
od.menuevents:	tbox:h.iconbar
od.menuevents:	tbox:h.quit
od.menuevents:	tbox:h.wimplib
od.menuevents:	tbox:h.textarea
od.menuevents:	C:msglib.h.MsgLib
od.menuevents:	C:debuglib.h.DebugLib
od.menuevents:	C:netfax.uilib.h.UILib
od.menuevents:	h.error
od.menuevents:	h.menuevents
od.menuevents:	h.addrbookIF
od.menuevents:	h.runes
od.menuevents:	h.globals
od.misc:	c.misc
od.misc:	h.include
od.misc:	C:h.swis
od.misc:	C:h.kernel
od.misc:	C:debuglib.h.DebugLib
od.misc:	h.strings
od.misc:	h.misc
od.miscwimp:	c.miscwimp
od.miscwimp:	h.include
od.miscwimp:	C:h.swis
od.miscwimp:	C:h.kernel
od.miscwimp:	h.tbox
od.miscwimp:	tbox:h.event
od.miscwimp:	tbox:h.wimp
od.miscwimp:	tbox:h.toolbox
od.miscwimp:	tbox:h.toolbox
od.miscwimp:	tbox:h.window
od.miscwimp:	tbox:h.gadgets
od.miscwimp:	tbox:h.gadgets
od.miscwimp:	tbox:h.menu
od.miscwimp:	tbox:h.proginfo
od.miscwimp:	tbox:h.iconbar
od.miscwimp:	tbox:h.quit
od.miscwimp:	tbox:h.wimplib
od.miscwimp:	tbox:h.textarea
od.miscwimp:	C:msglib.h.MsgLib
od.miscwimp:	C:debuglib.h.DebugLib
od.miscwimp:	C:netfax.uilib.h.UILib
od.miscwimp:	h.error
od.miscwimp:	h.miscwimp
od.miscwimp:	h.addrbookIF
od.miscwimp:	h.globals
od.strings:	c.strings
od.strings:	h.include
od.strings:	C:h.swis
od.strings:	C:h.kernel
od.strings:	h.tbox
od.strings:	tbox:h.event
od.strings:	tbox:h.wimp
od.strings:	tbox:h.toolbox
od.strings:	tbox:h.toolbox
od.strings:	tbox:h.window
od.strings:	tbox:h.gadgets
od.strings:	tbox:h.gadgets
od.strings:	tbox:h.menu
od.strings:	tbox:h.proginfo
od.strings:	tbox:h.iconbar
od.strings:	tbox:h.quit
od.strings:	tbox:h.wimplib
od.strings:	tbox:h.textarea
od.strings:	C:msglib.h.MsgLib
od.strings:	C:debuglib.h.DebugLib
od.strings:	C:netfax.uilib.h.UILib
od.strings:	h.strings
od.strings:	h.misc
od.strings:	C:h.ctype
od.flash:	c.flash
od.flash:	h.include
od.flash:	C:h.swis
od.flash:	C:h.kernel
od.flash:	h.tbox
od.flash:	tbox:h.event
od.flash:	tbox:h.wimp
od.flash:	tbox:h.toolbox
od.flash:	tbox:h.toolbox
od.flash:	tbox:h.window
od.flash:	tbox:h.gadgets
od.flash:	tbox:h.gadgets
od.flash:	tbox:h.menu
od.flash:	tbox:h.proginfo
od.flash:	tbox:h.iconbar
od.flash:	tbox:h.quit
od.flash:	tbox:h.wimplib
od.flash:	tbox:h.textarea
od.flash:	C:msglib.h.MsgLib
od.flash:	C:debuglib.h.DebugLib
od.flash:	C:netfax.uilib.h.UILib
od.flash:	h.addrbookIF
od.flash:	h.forms
od.flash:	h.misc
od.flash:	C:netfax.flashlib.h.flashlib
od.flash:	h.globals
od.flash:	h.error
od.flash:	h.flash
od.confmanIF:	c.confmanIF
od.confmanIF:	h.include
od.confmanIF:	C:h.swis
od.confmanIF:	C:h.kernel
od.confmanIF:	h.tbox
od.confmanIF:	tbox:h.event
od.confmanIF:	tbox:h.wimp
od.confmanIF:	tbox:h.toolbox
od.confmanIF:	tbox:h.toolbox
od.confmanIF:	tbox:h.window
od.confmanIF:	tbox:h.gadgets
od.confmanIF:	tbox:h.gadgets
od.confmanIF:	tbox:h.menu
od.confmanIF:	tbox:h.proginfo
od.confmanIF:	tbox:h.iconbar
od.confmanIF:	tbox:h.quit
od.confmanIF:	tbox:h.wimplib
od.confmanIF:	tbox:h.textarea
od.confmanIF:	C:msglib.h.MsgLib
od.confmanIF:	C:debuglib.h.DebugLib
od.confmanIF:	C:netfax.uilib.h.UILib
od.confmanIF:	h.strings
od.confmanIF:	h.addrbookIF
od.confmanIF:	h.forms
od.confmanIF:	h.search
od.confmanIF:	h.confmanIF
od.confmanIF:	h.globals
o.confmanIF:	c.confmanIF
o.confmanIF:	h.include
o.confmanIF:	C:h.swis
o.confmanIF:	C:h.kernel
o.confmanIF:	h.tbox
o.confmanIF:	tbox:h.event
o.confmanIF:	tbox:h.wimp
o.confmanIF:	tbox:h.toolbox
o.confmanIF:	tbox:h.toolbox
o.confmanIF:	tbox:h.window
o.confmanIF:	tbox:h.gadgets
o.confmanIF:	tbox:h.gadgets
o.confmanIF:	tbox:h.menu
o.confmanIF:	tbox:h.proginfo
o.confmanIF:	tbox:h.iconbar
o.confmanIF:	tbox:h.quit
o.confmanIF:	tbox:h.wimplib
o.confmanIF:	tbox:h.textarea
o.confmanIF:	C:msglib.h.MsgLib
o.confmanIF:	C:debuglib.h.DebugLib
o.confmanIF:	C:netfax.uilib.h.UILib
o.confmanIF:	h.strings
o.confmanIF:	h.addrbookIF
o.confmanIF:	h.forms
o.confmanIF:	h.search
o.confmanIF:	h.confmanIF
o.confmanIF:	h.globals
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
o.flash:	tbox:h.wimplib
o.flash:	tbox:h.textarea
o.flash:	C:msglib.h.MsgLib
o.flash:	C:debuglib.h.DebugLib
o.flash:	C:netfax.uilib.h.UILib
o.flash:	h.addrbookIF
o.flash:	h.forms
o.flash:	h.misc
o.flash:	C:netfax.flashlib.h.flashlib
o.flash:	h.globals
o.flash:	h.error
o.flash:	h.flash
o.memhandle:	c.memhandle
o.memhandle:	h.include
o.memhandle:	C:h.swis
o.memhandle:	C:h.kernel
o.memhandle:	C:debuglib.h.DebugLib
o.memhandle:	h.addrbookIF
o.memhandle:	h.memhandle
o.forms:	c.forms
o.forms:	h.include
o.forms:	C:h.swis
o.forms:	C:h.kernel
o.forms:	h.tbox
o.forms:	tbox:h.event
o.forms:	tbox:h.wimp
o.forms:	tbox:h.toolbox
o.forms:	tbox:h.toolbox
o.forms:	tbox:h.window
o.forms:	tbox:h.gadgets
o.forms:	tbox:h.gadgets
o.forms:	tbox:h.menu
o.forms:	tbox:h.proginfo
o.forms:	tbox:h.iconbar
o.forms:	tbox:h.quit
o.forms:	tbox:h.wimplib
o.forms:	tbox:h.textarea
o.forms:	C:msglib.h.MsgLib
o.forms:	C:debuglib.h.DebugLib
o.forms:	C:netfax.uilib.h.UILib
o.forms:	h.error
o.forms:	h.misc
o.forms:	h.miscwimp
o.forms:	h.addrbookIF
o.forms:	h.daytonaIF
o.forms:	h.forms
o.forms:	h.runes
o.forms:	h.flash
o.forms:	h.strings
o.forms:	h.globals
o.notes:	c.notes
o.notes:	h.include
o.notes:	C:h.swis
o.notes:	C:h.kernel
o.notes:	h.tbox
o.notes:	tbox:h.event
o.notes:	tbox:h.wimp
o.notes:	tbox:h.toolbox
o.notes:	tbox:h.toolbox
o.notes:	tbox:h.window
o.notes:	tbox:h.gadgets
o.notes:	tbox:h.gadgets
o.notes:	tbox:h.menu
o.notes:	tbox:h.proginfo
o.notes:	tbox:h.iconbar
o.notes:	tbox:h.quit
o.notes:	tbox:h.wimplib
o.notes:	tbox:h.textarea
o.notes:	C:msglib.h.MsgLib
o.notes:	C:debuglib.h.DebugLib
o.notes:	C:netfax.uilib.h.UILib
o.notes:	h.error
o.notes:	h.misc
o.notes:	h.miscwimp
o.notes:	h.addrbookIF
o.notes:	h.daytonaIF
o.notes:	h.strings
o.notes:	h.notes
o.notes:	h.runes
o.notes:	h.globals
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
o.search:	tbox:h.wimplib
o.search:	tbox:h.textarea
o.search:	C:msglib.h.MsgLib
o.search:	C:debuglib.h.DebugLib
o.search:	C:netfax.uilib.h.UILib
o.search:	C:h.time
o.search:	h.misc
o.search:	h.miscwimp
o.search:	h.strings
o.search:	h.addrbookIF
o.search:	h.daytonaIF
o.search:	h.forms
o.search:	h.flash
o.search:	h.runes
o.search:	h.error
o.search:	h.search
o.search:	h.globals
o.daytonaIF:	c.daytonaIF
o.daytonaIF:	h.include
o.daytonaIF:	C:h.swis
o.daytonaIF:	C:h.kernel
o.daytonaIF:	h.tbox
o.daytonaIF:	tbox:h.event
o.daytonaIF:	tbox:h.wimp
o.daytonaIF:	tbox:h.toolbox
o.daytonaIF:	tbox:h.toolbox
o.daytonaIF:	tbox:h.window
o.daytonaIF:	tbox:h.gadgets
o.daytonaIF:	tbox:h.gadgets
o.daytonaIF:	tbox:h.menu
o.daytonaIF:	tbox:h.proginfo
o.daytonaIF:	tbox:h.iconbar
o.daytonaIF:	tbox:h.quit
o.daytonaIF:	tbox:h.wimplib
o.daytonaIF:	tbox:h.textarea
o.daytonaIF:	C:msglib.h.MsgLib
o.daytonaIF:	C:debuglib.h.DebugLib
o.daytonaIF:	C:netfax.uilib.h.UILib
o.daytonaIF:	h.addrbookIF
o.daytonaIF:	h.DaytonaIF
o.daytonaIF:	h.memhandle
o.daytonaIF:	h.notes
o.daytonaIF:	h.forms
o.daytonaIF:	h.globals
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
o.error:	tbox:h.wimplib
o.error:	tbox:h.textarea
o.error:	C:msglib.h.MsgLib
o.error:	C:debuglib.h.DebugLib
o.error:	C:netfax.uilib.h.UILib
o.error:	h.addrbookIF
o.error:	h.miscwimp
o.error:	h.runes
o.error:	h.error
o.error:	h.globals
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
o.fontsetup:	tbox:h.wimplib
o.fontsetup:	tbox:h.textarea
o.fontsetup:	C:msglib.h.MsgLib
o.fontsetup:	C:debuglib.h.DebugLib
o.fontsetup:	C:netfax.uilib.h.UILib
o.fontsetup:	h.fontsetup
o.fontsetup:	h.addrbookIF
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
o.main:	tbox:h.wimplib
o.main:	tbox:h.textarea
o.main:	C:msglib.h.MsgLib
o.main:	C:debuglib.h.DebugLib
o.main:	C:netfax.uilib.h.UILib
o.main:	h.addrbookIF
o.main:	h.daytonaIF
o.main:	h.confmanIF
o.main:	h.menuevents
o.main:	h.runes
o.main:	h.fontsetup
o.main:	h.forms
o.main:	h.notes
o.main:	h.flash
o.main:	h.search
o.main:	h.error
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
o.menuevents:	tbox:h.wimplib
o.menuevents:	tbox:h.textarea
o.menuevents:	C:msglib.h.MsgLib
o.menuevents:	C:debuglib.h.DebugLib
o.menuevents:	C:netfax.uilib.h.UILib
o.menuevents:	h.error
o.menuevents:	h.menuevents
o.menuevents:	h.addrbookIF
o.menuevents:	h.runes
o.menuevents:	h.globals
o.misc:	c.misc
o.misc:	h.include
o.misc:	C:h.swis
o.misc:	C:h.kernel
o.misc:	C:debuglib.h.DebugLib
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
o.miscwimp:	tbox:h.wimplib
o.miscwimp:	tbox:h.textarea
o.miscwimp:	C:msglib.h.MsgLib
o.miscwimp:	C:debuglib.h.DebugLib
o.miscwimp:	C:netfax.uilib.h.UILib
o.miscwimp:	h.error
o.miscwimp:	h.miscwimp
o.miscwimp:	h.addrbookIF
o.miscwimp:	h.globals
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
o.runes:	tbox:h.wimplib
o.runes:	tbox:h.textarea
o.runes:	C:msglib.h.MsgLib
o.runes:	C:debuglib.h.DebugLib
o.runes:	C:netfax.uilib.h.UILib
o.runes:	h.error
o.runes:	h.addrbookIF
o.runes:	h.daytonaIF
o.runes:	h.forms
o.runes:	h.notes
o.runes:	h.runes
o.runes:	h.miscwimp
o.runes:	h.misc
o.runes:	h.strings
o.runes:	h.globals
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
o.strings:	tbox:h.wimplib
o.strings:	tbox:h.textarea
o.strings:	C:msglib.h.MsgLib
o.strings:	C:debuglib.h.DebugLib
o.strings:	C:netfax.uilib.h.UILib
o.strings:	h.strings
o.strings:	h.misc
o.strings:	C:h.ctype
od.forms:	c.forms
od.forms:	h.include
od.forms:	C:h.swis
od.forms:	C:h.kernel
od.forms:	h.tbox
od.forms:	tbox:h.event
od.forms:	tbox:h.wimp
od.forms:	tbox:h.toolbox
od.forms:	tbox:h.toolbox
od.forms:	tbox:h.window
od.forms:	tbox:h.gadgets
od.forms:	tbox:h.gadgets
od.forms:	tbox:h.menu
od.forms:	tbox:h.proginfo
od.forms:	tbox:h.iconbar
od.forms:	tbox:h.quit
od.forms:	tbox:h.wimplib
od.forms:	tbox:h.textarea
od.forms:	C:msglib.h.MsgLib
od.forms:	C:debuglib.h.DebugLib
od.forms:	C:netfax.uilib.h.UILib
od.forms:	h.error
od.forms:	h.misc
od.forms:	h.miscwimp
od.forms:	h.addrbookIF
od.forms:	h.daytonaIF
od.forms:	h.forms
od.forms:	h.runes
od.forms:	h.flash
od.forms:	h.strings
od.forms:	h.globals
od.notes:	c.notes
od.notes:	h.include
od.notes:	C:h.swis
od.notes:	C:h.kernel
od.notes:	h.tbox
od.notes:	tbox:h.event
od.notes:	tbox:h.wimp
od.notes:	tbox:h.toolbox
od.notes:	tbox:h.toolbox
od.notes:	tbox:h.window
od.notes:	tbox:h.gadgets
od.notes:	tbox:h.gadgets
od.notes:	tbox:h.menu
od.notes:	tbox:h.proginfo
od.notes:	tbox:h.iconbar
od.notes:	tbox:h.quit
od.notes:	tbox:h.wimplib
od.notes:	tbox:h.textarea
od.notes:	C:msglib.h.MsgLib
od.notes:	C:debuglib.h.DebugLib
od.notes:	C:netfax.uilib.h.UILib
od.notes:	h.error
od.notes:	h.misc
od.notes:	h.miscwimp
od.notes:	h.addrbookIF
od.notes:	h.daytonaIF
od.notes:	h.strings
od.notes:	h.notes
od.notes:	h.runes
od.notes:	h.globals
od.search:	c.search
od.search:	h.include
od.search:	C:h.swis
od.search:	C:h.kernel
od.search:	h.tbox
od.search:	tbox:h.event
od.search:	tbox:h.wimp
od.search:	tbox:h.toolbox
od.search:	tbox:h.toolbox
od.search:	tbox:h.window
od.search:	tbox:h.gadgets
od.search:	tbox:h.gadgets
od.search:	tbox:h.menu
od.search:	tbox:h.proginfo
od.search:	tbox:h.iconbar
od.search:	tbox:h.quit
od.search:	tbox:h.wimplib
od.search:	tbox:h.textarea
od.search:	C:msglib.h.MsgLib
od.search:	C:debuglib.h.DebugLib
od.search:	C:netfax.uilib.h.UILib
od.search:	C:h.time
od.search:	h.misc
od.search:	h.miscwimp
od.search:	h.strings
od.search:	h.addrbookIF
od.search:	h.daytonaIF
od.search:	h.forms
od.search:	h.flash
od.search:	h.runes
od.search:	h.error
od.search:	h.search
od.search:	h.globals
od.daytonaIF:	c.daytonaIF
od.daytonaIF:	h.include
od.daytonaIF:	C:h.swis
od.daytonaIF:	C:h.kernel
od.daytonaIF:	h.tbox
od.daytonaIF:	tbox:h.event
od.daytonaIF:	tbox:h.wimp
od.daytonaIF:	tbox:h.toolbox
od.daytonaIF:	tbox:h.toolbox
od.daytonaIF:	tbox:h.window
od.daytonaIF:	tbox:h.gadgets
od.daytonaIF:	tbox:h.gadgets
od.daytonaIF:	tbox:h.menu
od.daytonaIF:	tbox:h.proginfo
od.daytonaIF:	tbox:h.iconbar
od.daytonaIF:	tbox:h.quit
od.daytonaIF:	tbox:h.wimplib
od.daytonaIF:	tbox:h.textarea
od.daytonaIF:	C:msglib.h.MsgLib
od.daytonaIF:	C:debuglib.h.DebugLib
od.daytonaIF:	C:netfax.uilib.h.UILib
od.daytonaIF:	h.addrbookIF
od.daytonaIF:	h.DaytonaIF
od.daytonaIF:	h.memhandle
od.daytonaIF:	h.notes
od.daytonaIF:	h.forms
od.daytonaIF:	h.globals
od.runes:	c.runes
od.runes:	h.include
od.runes:	C:h.swis
od.runes:	C:h.kernel
od.runes:	h.tbox
od.runes:	tbox:h.event
od.runes:	tbox:h.wimp
od.runes:	tbox:h.toolbox
od.runes:	tbox:h.toolbox
od.runes:	tbox:h.window
od.runes:	tbox:h.gadgets
od.runes:	tbox:h.gadgets
od.runes:	tbox:h.menu
od.runes:	tbox:h.proginfo
od.runes:	tbox:h.iconbar
od.runes:	tbox:h.quit
od.runes:	tbox:h.wimplib
od.runes:	tbox:h.textarea
od.runes:	C:msglib.h.MsgLib
od.runes:	C:debuglib.h.DebugLib
od.runes:	C:netfax.uilib.h.UILib
od.runes:	h.error
od.runes:	h.addrbookIF
od.runes:	h.daytonaIF
od.runes:	h.forms
od.runes:	h.notes
od.runes:	h.runes
od.runes:	h.miscwimp
od.runes:	h.misc
od.runes:	h.strings
od.runes:	h.globals
