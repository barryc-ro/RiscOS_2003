# Makefile for PlugPlay
# =====================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 03-Apr-2000	ADH	Created from Browse Makefile.

# -----------------------------------------------------------------------------
# Program specific options
#

# These are for builds where SYSTEM is not defined on the AMU command
# line, e.g. ROM builds

COMPONENT	= PlugPlay
APP		= !PlugPlay
MODULE		= rm.!PlugPlay
ROM_MODULE	= aof.PlugPlay

# Where are the resources?

LDIR		= Resources.${LOCALE}.${SYSTEM}

# Need to use various things for ROM builds or Messages file version tokens

VERSIONNUM	= VersionNum
MAKEAPPNAME	= awk.AppName
ADDTOMESSAGES	= awk.AddToMsgs

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.${COMPONENT}
RESAPP		= <resource$dir>.Apps.${APP}


# -----------------------------------------------------------------------------
# Generic options
#

LIBDIR		= <Lib$Dir>
MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe
AWK		= awk

AFLAGS		= ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS		= ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -Wpc
CPFLAGS		= ~cfr~v
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# BASEFLAGS holds common compile time options that all builds will use.
# BASEFLAGSD holds additional flags for debug builds.

BASEFLAGS	= -DCOMPAT_INET4 "-DSYSTEM=\"${SYSTEM}\""
BASEFLAGSD	= -fn -DTRACE

# Build-dependent flags.

PPFLAGS		= ${BASEFLAGS}
PPFLAGSD	= ${BASEFLAGS} ${BASEFLAGSD}
PPFLAGSZ	= ${BASEFLAGS}
PPFLAGSDZ	= ${BASEFLAGS} ${BASEFLAGSD}


# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap
URILIB		= C:o.URI
FLEXLIB		= tbox:o.flexlib
EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
RENDERLIB	= tbox:o.renderlib
INETLIB		= TCPIPLibs:o.inetlib
SOCKLIB		= TCPIPLibs:o.socklib
DEBUGLIB	= <Lib$Dir>.Debuglib.o.debuglib

# ROM build libraries

LIBSZ = \
 ${URILIB} \
 ${FLEXLIB}zm \
 ${EVENTLIB}m \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${RENDERLIB} \
 ${INETLIB}zm \
 ${SOCKLIB}zm

# RAM release build libraries

LIBS = \
 ${CLIB} \
 ${URILIB} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${RENDERLIB} \
 ${INETLIB} \
 ${SOCKLIB}

# RAM debug build libraries

LIBSD = \
 ${CLIB} \
 ${URILIB} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${RENDERLIB} \
 ${DEBUGLIB} \
 ${INETLIB} \
 ${SOCKLIB}


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -IC:

FILES_COMMON = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${LDIR}.Sprites

FILES = \
 ${FILES_COMMON} \
 ${TARGET}

# Include these in the ROM module

RESFILES = \
 ${FILES_COMMON}


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJSPP = \
 opp.Filter		\
 opp.FromROSLib		\
 opp.FullScreen		\
 opp.Global		\
 opp.Handlers		\
 opp.Main		\
 opp.ModeInfo		\
 opp.Overlay		\
 opp.PlugIn		\
 opp.Protocols		\
 opp.RMA		\
 opp.URLutils		\
 opp.Utils		\
 o.ROSLib		\
 o.SetPW

OBJSPPD = \
 oppd.Filter		\
 oppd.FromROSLib	\
 oppd.FullScreen	\
 oppd.Global		\
 oppd.Handlers		\
 oppd.Main		\
 oppd.ModeInfo		\
 oppd.Overlay		\
 oppd.PlugIn		\
 oppd.Protocols		\
 oppd.RMA		\
 oppd.URLutils		\
 oppd.Utils		\
 o.ROSLib		\
 o.SetPW

OBJSPPZ = \
 oppz.Filter		\
 oppz.FromROSLib	\
 oppz.FullScreen	\
 oppz.Global		\
 oppz.Handlers		\
 oppz.Main		\
 oppz.ModeInfo		\
 oppz.Overlay		\
 oppz.PlugIn		\
 oppz.Protocols		\
 oppz.RMA		\
 oppz.URLutils		\
 oppz.Utils		\
 o.ROSLib		\
 o.SetPW

OBJSPPDZ = \
 oppdz.Filter		\
 oppdz.FromROSLib	\
 oppdz.FullScreen	\
 oppdz.Global		\
 oppdz.Handlers		\
 oppdz.Main		\
 oppdz.ModeInfo		\
 oppdz.Overlay		\
 oppdz.PlugIn		\
 oppdz.Protocols	\
 oppdz.RMA		\
 oppdz.URLutils		\
 oppdz.Utils		\
 o.ROSLib		\
 o.SetPW


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .opp .oppd .oppz .oppdz .s .c

.c.opp:;	@echo
		@echo Compiling $<
		@echo ${PPFLAGS}
		@${CC} ${CFLAGS} ${PPFLAGS}        -c -o $@ $<

.c.oppd:;	@echo
		@echo Compiling $< (debug)
		@echo ${PPFLAGSD}
		@${CC} ${CFLAGS} ${PPFLAGSD}       -c -g -o $@ $<

.c.oppz:;		@echo
		@echo Compiling $< (ROM)
		@echo ${PPFLAGSZ}
		@${CC} ${CFLAGS} ${PPFLAGSZ}       -c -zM -DROM -o $@ $<

.c.oppdz:;	@echo
		@echo Compiling $< (ROM debug)
		@echo ${PPFLAGSDZ}
		@${CC} ${CFLAGS} ${PPFLAGSDZ}      -c -zM -DROM -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${AFLAGS}
		@${AS} ${AFLAGS} $< $@


# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (RAM)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	@echo
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}n
	-${CP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22		${CPFLAGS}n
	-${CP} ${LDIR}.!Sprites23		${INSTDIR}.!Sprites23		${CPFLAGS}n
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}~n
	${AWK} -f ${ADDTOMESSAGES} ${VERSIONNUM} >> ${INSTDIR}.Messages
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}n
	${CP} ${LDIR}.Sprites			${INSTDIR}.Sprites		${CPFLAGS}n
	-${CP} ${LDIR}.Sprites22		${INSTDIR}.Sprites22		${CPFLAGS}n
	-${CP} ${LDIR}.Sprites23		${INSTDIR}.Sprites23		${CPFLAGS}n
	-${CP} ${LDIR}.About			${INSTDIR}.About		${CPFLAGS}n
	${WIPE} ${INSTDIR}.About.CVS	${WFLAGS}

install: install_common
	${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Debug application installed ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}.About
	${CP} ${LDIR}.ROM.!Boot		${RESAPP}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESAPP}.!Sprites	${CPFLAGS}
	-${CP} ${LDIR}.!Sprites22	${RESAPP}.!Sprites22	${CPFLAGS}
	-${CP} ${LDIR}.!Sprites23	${RESAPP}.!Sprites23	${CPFLAGS}
	${CP} ${LDIR}.Sprites		${RESDIR}.Sprites	${CPFLAGS}
	-${CP} ${LDIR}.Sprites22	${RESDIR}.Sprites22	${CPFLAGS}
	-${CP} ${LDIR}.Sprites23	${RESDIR}.Sprites23	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.About		${RESDIR}.About		${CPFLAGS}
	${WIPE} ${RESDIR}.About.CVS	${WFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	map		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	opp		${WFLAGS}
	${WIPE}	oppd		${WFLAGS}
	${WIPE}	oppz		${WFLAGS}
	${WIPE}	oppdz		${WFLAGS}
	${WIPE}	rm		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	opp.dirs
	@echo
	@echo ${COMPONENT}: Cleaned

clean_all: clean
	${WIPE}	syms		${WFLAGS}
	${WIPE}	Targets		${WFLAGS}
	@echo
	@echo ${COMPONENT}: Cleaned all

opp.dirs:
	@${MKDIR} abs
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} o
	@${MKDIR} opp
	@${MKDIR} oppd
	@${MKDIR} oppz
	@${MKDIR} oppdz
	@${MKDIR} rm
	@${MKDIR} syms
	@${MKDIR} Targets
	@${MKDIR} Targets.PlugPlay
	@${MKDIR} Targets.PlugPlay.!PlugPlay
	create opp.dirs
	stamp  opp.dirs


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJSPP} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJSPP} ${LIBS} > map.base0


# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
#

rom_link:
	${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

oppz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
	@echo
	${AWK} -f ${MAKEAPPNAME} ${LDIR}.Messages ${VERSIONNUM} > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI_PP: ${OBJSPP} ${LIBS} opp.dirs
	@echo
	${LD} -s syms.!RI_PP -o $@ ${OBJSPP} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_PPD: ${OBJSPPD} ${LIBSD} opp.dirs
	@echo
	${LD} -debug -s syms.!RI_PPD -o $@ ${OBJSPPD} ${LIBSD}

${MODULE}: oppz.ModuleWrap ${OBJSPPZ} ${LIBSZ} ${CLIB} opp.dirs
	@echo
	${LD} -s syms.RAMMod -o $@ -module oppz.ModuleWrap ${OBJSPPZ} ${LIBSZ} ${CLIB}

${ROM_MODULE}: oppz.ModuleWrap ${OBJSPPZ} ${ROMCSTUBS} ${LIBSZ} opp.dirs
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oppz.ModuleWrap ${OBJSPPZ} ${LIBSZ} ${ROMCSTUBS}

 ${MODULE}D: oppdz.ModuleWrap ${OBJSPPDZ} ${LIBSZ} opp.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oppz.ModuleWrap ${OBJSPPDZ} ${LIBSZ}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
