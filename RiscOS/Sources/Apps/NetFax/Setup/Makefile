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


