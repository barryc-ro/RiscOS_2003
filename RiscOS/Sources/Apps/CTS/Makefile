# Makefile for CTS Plugin
# =======================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1999-05-11	JPB	Created from Daytona address book Makefile

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= CTS
APP		= !CTS
MODULE		= rm.!CTS
ROM_MODULE	= aof.CTS

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
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= -depend !Depend -nocache -stamp -quit ${THROWBACK}
CFLAGS		= ${THROWBACK} -depend !Depend -ffah ${INCLUDES} -wp
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

INETLIB		= TCPIPLIBS:o.inetlib
SOCKLIB		= TCPIPLIBS:o.socklib5
UNIXLIB		= TCPIPLIBS:o.unixlib

DEBUGLIB	= ${LIBDIR}.DebugLib.o.DebugLib

LIBSM =\
 ${WIMPLIB}\
 ${TOOLBOXLIB}\
 ${EVENTLIB}m\
 ${UNIXLIB}zm\
 ${INETLIB}zm\
 ${SOCKLIB}zm\

LIBS =\
 ${WIMPLIB}\
 ${TOOLBOXLIB}\
 ${EVENTLIB}\
 ${UNIXLIB}\
 ${INETLIB}\
 ${SOCKLIB}\
 ${CLIB}\

LIBSD =\
 ${WIMPLIB}\
 ${TOOLBOXLIB}\
 ${EVENTLIB}\
 ${DEBUGLIB}\
 ${UNIXLIB}\
 ${INETLIB}\
 ${SOCKLIB}\
 ${CLIB}\


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,TCPIPLibs:,C:

FILES = \
 LocalRes:!Boot \
 LocalRes:!Run \
 LocalRes:!Sprites \
 LocalRes:Messages \
 LocalRes:Res \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 LocalRes:!Sprites \

DIRS = o._dirs

# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS =\
 o.client\
 o.ctsmessage\
 o.ctsbrowser\
 o.ctsui\
 o.ctsutils\
 o.plugclient\
 o.smartcard\
 o.statetable\
 o.utils

OBJSZ =\
 oz.client\
 oz.ctsmessage\
 oz.ctsbrowser\
 oz.ctsui\
 oz.ctsutils\
 oz.plugclient\
 oz.smartcard\
 oz.statetable\
 oz.utils


OBJSD =\
 od.client\
 od.ctsmessage\
 od.ctsbrowser\
 od.ctsui\
 od.ctsutils\
 od.plugclient\
 od.smartcard\
 od.statetable\
 od.utils
 
#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.oz:;		${CC} ${CFLAGS} ${DZFLAGS} ${EXTRAFLAGS} -c -zM -zps1 -DROM -o $@ $<

.c.od:;		${CC} ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -c -o $@ $<

.c.odz:;	${CC} ${CFLAGS} ${DDZFLAGS} ${EXTRAFLAGS} -c -zM -zps1 -DROM -o $@ $<

.s.o:;		${AS} ${AFLAGS} $< $@


# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	@echo
	${CP} LocalRes:!Boot			${INSTDIR}.!Boot		${CPFLAGS}
	${CP} LocalRes:!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}
	${CP} LocalRes:Messages			${INSTDIR}.Messages		${CPFLAGS}
	${CP} LocalRes:Res			${INSTDIR}.Res			${CPFLAGS}
	${MKDIR} ${INSTDIR}.WebPages
	${CP} LocalRes:WebPages.01-1 		${INSTDIR}.WebPages.01-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.01-2 		${INSTDIR}.WebPages.01-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.01-3 		${INSTDIR}.WebPages.01-3 ${CPFLAGS}
	${CP} LocalRes:WebPages.02   		${INSTDIR}.WebPages.02   ${CPFLAGS}
	${CP} LocalRes:WebPages.03   		${INSTDIR}.WebPages.03   ${CPFLAGS}
	${CP} LocalRes:WebPages.04   		${INSTDIR}.WebPages.04   ${CPFLAGS}
	${CP} LocalRes:WebPages.05   		${INSTDIR}.WebPages.05   ${CPFLAGS}
	${CP} LocalRes:WebPages.06   		${INSTDIR}.WebPages.06   ${CPFLAGS}
	${CP} LocalRes:WebPages.11   		${INSTDIR}.WebPages.11   ${CPFLAGS}
	${CP} LocalRes:WebPages.21   		${INSTDIR}.WebPages.21   ${CPFLAGS}
	${CP} LocalRes:WebPages.22   		${INSTDIR}.WebPages.22   ${CPFLAGS}
	${CP} LocalRes:WebPages.23   		${INSTDIR}.WebPages.23   ${CPFLAGS}
	${CP} LocalRes:WebPages.24   		${INSTDIR}.WebPages.24   ${CPFLAGS}
	${CP} LocalRes:WebPages.25-1   		${INSTDIR}.WebPages.25-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.25-2   		${INSTDIR}.WebPages.25-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.26   		${INSTDIR}.WebPages.26   ${CPFLAGS}
	${CP} LocalRes:WebPages.27-1 		${INSTDIR}.WebPages.27-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.27-2 		${INSTDIR}.WebPages.27-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.27-3 		${INSTDIR}.WebPages.27-3 ${CPFLAGS}
	${CP} LocalRes:WebPages.28-1 		${INSTDIR}.WebPages.28-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.28-2 		${INSTDIR}.WebPages.28-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.30   		${INSTDIR}.WebPages.30   ${CPFLAGS}

install: install_common
	${CP} LocalRes:!Run		${INSTDIR}.!Run		${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
	-Access ${INSTDIR}.!Boot	lr/r
	-Access ${INSTDIR}.!Run		lr/r
	-Access ${INSTDIR}.!RunImage	lr/r
	-Access ${INSTDIR}.Messages	lr/r
	-Access ${INSTDIR}.Res		lr/r
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} LocalRes:!RunT		${INSTDIR}.!Run		${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Debug application installed to ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}.User
	${CP} LocalRes:ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} LocalRes:!Sprites		${RESDIR}.!Sprites	${CPFLAGS}
	${CP} LocalRes:Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} LocalRes:Res		${RESDIR}.Res		${CPFLAGS}
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
	${RM}	s.ModuleWrap
	@echo
	@echo ${COMPONENT}: Cleaned

${DIRS}:
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
	@${MKDIR} Targets.!CTS
	create ${DIRS}
	stamp  ${DIRS}


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
	${WIPE}	Targets		${WFLAGS}
	${WIPE} syms		${WFLAGS}
	@echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
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


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${EXTRALIBS} ${LIBS} ${DIRS}
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS} ${EXTRALIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${EXTRALIBS} ${LIBSD} ${DIRS}
	@echo
	${LD} -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD} ${EXTRALIBS}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${CLIB} ${DIRS}
	@echo
	${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} ${EXTRALIBS} ${DIRS}
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${EXTRALIBS} ${ROMCSTUBS}

rm.!CTSD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} ${EXTRALIBS} ${DIRS}
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM} ${EXTRALIBS}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
