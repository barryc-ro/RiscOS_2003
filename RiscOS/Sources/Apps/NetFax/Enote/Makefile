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
DDFLAGS         = ${DFLAGS} -fn
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

UILIB		= ${LIBDIR}.UILib.o.UILib
ERRORLIB	= ${LIBDIR}.ErrorLib.o.ErrorLib
MSGLIB		= ${LIBDIR}.MsgLib.o.MsgLib
FLASHLIB	= ${LIBDIR}.FlashLib.o.FlashLib


LIBS = \
 ${FLASHLIB} \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB}


LIBSM = \
 ${FLASHLIB} \
 ${UILIB}zm \
 ${MSGLIB}zm \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \


LIBSD = \
 ${FLASHLIB} \
 ${UILIB} \
 ${MSGLIB} \
 ${ERRORLIB} \
 ${TEXTGADGETLIB} \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB}


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

OBJS =\
 o.daytonaIF\
 o.error\
 o.fontsetup\
 o.main\
 o.menuevents\
 o.misc\
 o.miscwimp\
 o.runes\
 o.strings\


OBJSZ =\
 oz.daytonaIF\
 oz.error\
 oz.fontsetup\
 oz.main\
 oz.menuevents\
 oz.misc\
 oz.miscwimp\
 oz.runes\
 oz.strings\


OBJSD =\
 od.daytonaIF\
 od.error\
 od.fontsetup\
 od.main\
 od.menuevents\
 od.misc\
 od.miscwimp\
 od.runes\
 od.strings\


OBJSDZ =\
 odz.daytonaIF\
 odz.error\
 odz.fontsetup\
 odz.main\
 odz.menuevents\
 odz.misc\
 odz.miscwimp\
 odz.runes\
 odz.strings\


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
                @${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

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
        ${CP} ${TARGET}                         ${INSTDIR}.!RunImage            ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites                  ${INSTDIR}.!Sprites             ${CPFLAGS}
        ${CP} ${LDIR}.Messages                  ${INSTDIR}.Messages             ${CPFLAGS}
        ${CP} ${LDIR}.Res                       ${INSTDIR}.Res                  ${CPFLAGS}

install: install_common
        ${CP} ${LDIR}.!Run              ${INSTDIR}.!Run         ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
        ${CP} ${LDIR}.!RunD             ${INSTDIR}.!Run         ${CPFLAGS}
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

rm.!DayNoteD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
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
oz.daytonaIF:	c.daytonaIF
oz.daytonaIF:	h.include
oz.daytonaIF:	C:h.swis
oz.daytonaIF:	C:h.kernel
oz.daytonaIF:	h.tbox
oz.daytonaIF:	tbox:h.event
oz.daytonaIF:	tbox:h.wimp
oz.daytonaIF:	tbox:h.toolbox
oz.daytonaIF:	tbox:h.toolbox
oz.daytonaIF:	tbox:h.window
oz.daytonaIF:	tbox:h.gadgets
oz.daytonaIF:	tbox:h.gadgets
oz.daytonaIF:	tbox:h.menu
oz.daytonaIF:	tbox:h.proginfo
oz.daytonaIF:	tbox:h.iconbar
oz.daytonaIF:	tbox:h.quit
oz.daytonaIF:	tbox:h.wimplib
oz.daytonaIF:	tbox:h.textarea
oz.daytonaIF:	<Lib$Dir>.MsgLib.h.MsgLib
oz.daytonaIF:	<Lib$Dir>.DebugLib.h.DebugLib
oz.daytonaIF:	<Lib$Dir>.UILib.h.UILib
oz.daytonaIF:	h.DaytonaIF
oz.daytonaIF:	h.globals
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
oz.error:	tbox:h.wimplib
oz.error:	tbox:h.textarea
oz.error:	<Lib$Dir>.MsgLib.h.MsgLib
oz.error:	<Lib$Dir>.DebugLib.h.DebugLib
oz.error:	<Lib$Dir>.UILib.h.UILib
oz.error:	h.miscwimp
oz.error:	h.runes
oz.error:	h.error
oz.error:	h.globals
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
oz.fontsetup:	tbox:h.wimplib
oz.fontsetup:	tbox:h.textarea
oz.fontsetup:	<Lib$Dir>.MsgLib.h.MsgLib
oz.fontsetup:	<Lib$Dir>.DebugLib.h.DebugLib
oz.fontsetup:	<Lib$Dir>.UILib.h.UILib
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
oz.main:	tbox:h.wimplib
oz.main:	tbox:h.textarea
oz.main:	<Lib$Dir>.MsgLib.h.MsgLib
oz.main:	<Lib$Dir>.DebugLib.h.DebugLib
oz.main:	<Lib$Dir>.UILib.h.UILib
oz.main:	h.daytonaif
oz.main:	h.menuevents
oz.main:	h.runes
oz.main:	h.fontsetup
oz.main:	h.error
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
oz.menuevents:	tbox:h.wimplib
oz.menuevents:	tbox:h.textarea
oz.menuevents:	<Lib$Dir>.MsgLib.h.MsgLib
oz.menuevents:	<Lib$Dir>.DebugLib.h.DebugLib
oz.menuevents:	<Lib$Dir>.UILib.h.UILib
oz.menuevents:	h.error
oz.menuevents:	h.menuevents
oz.menuevents:	h.runes
oz.menuevents:	h.globals
oz.misc:	c.misc
oz.misc:	h.include
oz.misc:	C:h.swis
oz.misc:	C:h.kernel
oz.misc:	<Lib$Dir>.DebugLib.h.DebugLib
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
oz.miscwimp:	tbox:h.wimplib
oz.miscwimp:	tbox:h.textarea
oz.miscwimp:	<Lib$Dir>.MsgLib.h.MsgLib
oz.miscwimp:	<Lib$Dir>.DebugLib.h.DebugLib
oz.miscwimp:	<Lib$Dir>.UILib.h.UILib
oz.miscwimp:	h.error
oz.miscwimp:	h.globals
oz.miscwimp:	h.miscwimp
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
oz.runes:	tbox:h.wimplib
oz.runes:	tbox:h.textarea
oz.runes:	<Lib$Dir>.MsgLib.h.MsgLib
oz.runes:	<Lib$Dir>.DebugLib.h.DebugLib
oz.runes:	<Lib$Dir>.UILib.h.UILib
oz.runes:	h.error
oz.runes:	h.daytonaIF
oz.runes:	h.runes
oz.runes:	h.miscwimp
oz.runes:	h.misc
oz.runes:	h.globals
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
oz.strings:	tbox:h.wimplib
oz.strings:	tbox:h.textarea
oz.strings:	<Lib$Dir>.MsgLib.h.MsgLib
oz.strings:	<Lib$Dir>.DebugLib.h.DebugLib
oz.strings:	<Lib$Dir>.UILib.h.UILib
oz.strings:	h.strings
oz.strings:	h.misc
oz.strings:	C:h.ctype
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
o.daytonaIF:	<Lib$Dir>.MsgLib.h.MsgLib
o.daytonaIF:	<Lib$Dir>.DebugLib.h.DebugLib
o.daytonaIF:	<Lib$Dir>.UILib.h.UILib
o.daytonaIF:	h.addrbookIF
o.daytonaIF:	h.DaytonaIF
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
o.error:	<Lib$Dir>.MsgLib.h.MsgLib
o.error:	<Lib$Dir>.DebugLib.h.DebugLib
o.error:	<Lib$Dir>.UILib.h.UILib
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
o.fontsetup:	<Lib$Dir>.MsgLib.h.MsgLib
o.fontsetup:	<Lib$Dir>.DebugLib.h.DebugLib
o.fontsetup:	<Lib$Dir>.UILib.h.UILib
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
o.main:	tbox:h.wimplib
o.main:	tbox:h.textarea
o.main:	<Lib$Dir>.MsgLib.h.MsgLib
o.main:	<Lib$Dir>.DebugLib.h.DebugLib
o.main:	<Lib$Dir>.UILib.h.UILib
o.main:	h.addrbookIF
o.main:	h.daytonaIF
o.main:	h.menuevents
o.main:	h.runes
o.main:	h.fontsetup
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
o.menuevents:	<Lib$Dir>.MsgLib.h.MsgLib
o.menuevents:	<Lib$Dir>.DebugLib.h.DebugLib
o.menuevents:	<Lib$Dir>.UILib.h.UILib
o.menuevents:	h.error
o.menuevents:	h.menuevents
o.menuevents:	h.addrbookIF
o.menuevents:	h.runes
o.menuevents:	h.globals
o.misc:	c.misc
o.misc:	h.include
o.misc:	C:h.swis
o.misc:	C:h.kernel
o.misc:	<Lib$Dir>.DebugLib.h.DebugLib
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
o.miscwimp:	<Lib$Dir>.MsgLib.h.MsgLib
o.miscwimp:	<Lib$Dir>.DebugLib.h.DebugLib
o.miscwimp:	<Lib$Dir>.UILib.h.UILib
o.miscwimp:	h.error
o.miscwimp:	h.globals
o.miscwimp:	h.miscwimp
o.miscwimp:	h.addrbookIF
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
o.runes:	<Lib$Dir>.MsgLib.h.MsgLib
o.runes:	<Lib$Dir>.DebugLib.h.DebugLib
o.runes:	<Lib$Dir>.UILib.h.UILib
o.runes:	h.error
o.runes:	h.addrbookIF
o.runes:	h.daytonaIF
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
o.strings:	<Lib$Dir>.MsgLib.h.MsgLib
o.strings:	<Lib$Dir>.DebugLib.h.DebugLib
o.strings:	<Lib$Dir>.UILib.h.UILib
o.strings:	h.strings
o.strings:	h.misc
o.strings:	C:h.ctype
