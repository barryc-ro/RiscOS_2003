# Makefile for PlugPlay
# =====================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 03-Apr-2000	ADH	Created from Browse Makefile.
# 28-Apr-2000	ADH	Significant alterations and extensions to support
#			ROM and RAM module builds, and applications.

# -----------------------------------------------------------------------------
# Program specific options
#

APP		= !${COMPONENT}
MODULE		= rm.!${COMPONENT}
ROM_MODULE	= aof.${COMPONENT}
DIRS		= o._dirs_

# Where are the resources?

LDIR		= LocalRes:${COMPONENT}

# Need to use various things for ROM builds or Messages file version tokens

VERSIONNUM	= VersionNum
MAKEAPPNAME	= awk.AppName
ADDTOMESSAGES	= awk.AddToMsgs

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <Resource$Dir>.Resources2.${COMPONENT}
RESAPP		= <Resource$Dir>.Apps.${APP}


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
AFLAGSRAMZ	= ${AFLAGS} -PD "REMOVE_RAMLOAD_CHECK SETL {TRUE}" -PD "UsePathForHelpMessages SETL {TRUE}"
CFLAGS		= ${THROWBACK} -depend !Depend -ffa -Wpc ${INCLUDES}
CPFLAGS		= ~cfr~v
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# BASEFLAGS holds common compile time options that all builds will use.
# BASEFLAGSD holds additional flags for debug builds.

BASEFLAGS	= -DCOMPAT_INET4 "-DSYSTEM=\"${COMPONENT}\""
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

# Application build libraries

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

# Application debug build libraries

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

# Module build libraries

LIBSZ = \
 ${URILIB} \
 ${FLEXLIB}zm \
 ${EVENTLIB}m \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${RENDERLIB} \
 ${INETLIB}zm \
 ${SOCKLIB}zm

# Module debug build libraries

LIBSDZ = \
 ${URILIB} \
 ${FLEXLIB}zm \
 ${EVENTLIB}m \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${RENDERLIB} \
 ${DEBUGLIB}zm \
 ${INETLIB}zm \
 ${SOCKLIB}zm


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


# -----------------------------------------------------------------------------
# Include files (objects)
#

# RAM application "PlugPlay"

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

# RAM module "PlugPlay"

OBJSPPRZ = \
 opprz.Filter		\
 opprz.FromROSLib	\
 opprz.FullScreen	\
 opprz.Global		\
 opprz.Handlers		\
 opprz.Main		\
 opprz.ModeInfo		\
 opprz.Overlay		\
 opprz.PlugIn		\
 opprz.Protocols	\
 opprz.RMA		\
 opprz.URLutils		\
 opprz.Utils		\
 o.FilterHelp		\
 o.ROSLib		\
 o.SetPW

OBJSPPRDZ = \
 opprdz.Filter		\
 opprdz.FromROSLib	\
 opprdz.FullScreen	\
 opprdz.Global		\
 opprdz.Handlers	\
 opprdz.Main		\
 opprdz.ModeInfo	\
 opprdz.Overlay		\
 opprdz.PlugIn		\
 opprdz.Protocols	\
 opprdz.RMA		\
 opprdz.URLutils	\
 opprdz.Utils		\
 o.FilterHelp		\
 o.ROSLib		\
 o.SetPW

# ROM module "PlugPlay"; same as RAM module but has "-DROM"
# on CC command line

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
 o.FilterHelp		\
 o.ROSLib		\
 o.SetPW


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .opp .oppd .oppz .opprz .opprdz .s .c

.c.opp:;	@echo
		@echo Compiling $< (application)
		@echo ${PPFLAGS}
		@${CC} ${CFLAGS} ${PPFLAGS}        -c -UROM -DAPPLICATION -o $@ $<

.c.oppd:;	@echo
		@echo Compiling $< (debug application)
		@echo ${PPFLAGSD}
		@${CC} ${CFLAGS} ${PPFLAGSD}       -c -UROM -DAPPLICATION -g -o $@ $<

.c.oppz:;	@echo
		@echo Compiling $< (ROM module)
		@echo ${PPFLAGSZ}
		@${CC} ${CFLAGS} ${PPFLAGSZ}       -c -zM -DROM -UAPPLICATION -o $@ $<

.c.opprz:;	@echo
		@echo Compiling $< (RAM module)
		@echo ${PPFLAGS}
		@${CC} ${CFLAGS} ${PPFLAGS}        -c -zM -UROM -UAPPLICATION -o $@ $<

.c.opprdz:;	@echo
		@echo Compiling $< (RAM debug module)
		@echo ${PPFLAGSD}
		@${CC} ${CFLAGS} ${PPFLAGSD}       -c -zM -UROM -UAPPLICATION -g -o $@ $<

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
	@echo ${COMPONENT}: Module built (ROM module)

ram: ${MODULE}
	@echo ${COMPONENT}: Module built (RAM module)

ramd: ${MODULE}D
	@echo ${COMPONENT}: Module built (RAM debug module)

install_common: ${FILES}
	@echo
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
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Debug application installed to ${INSTDIR}

install_ram: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: RAM module installed to ${INSTDIR}

install_ramd: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: RAM debug module installed to ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: ROM Module installed to ${INSTDIR}

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
	${AWK} -f ${ADDTOMESSAGES} ${VERSIONNUM} >> ${RESDIR}.Messages
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.About		${RESDIR}.About		${CPFLAGS}
	${WIPE} ${RESDIR}.About.CVS	${WFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	${WIPE}	rm		${WFLAGS}
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	oz		${WFLAGS}
	${WIPE}	opp		${WFLAGS}
	${WIPE}	oppd		${WFLAGS}
	${WIPE}	oppz		${WFLAGS}
	${WIPE}	opprz		${WFLAGS}
	${WIPE}	opprdz		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	${DIRS}
	@echo
	@echo ${COMPONENT}: Cleaned

clean_all: clean
	@echo
	${WIPE}	syms		${WFLAGS}
	${WIPE}	Targets		${WFLAGS}
	@echo
	@echo ${COMPONENT}: Cleaned all

${DIRS}:
	@${MKDIR} abs
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} o
	@${MKDIR} oz
	@${MKDIR} opp
	@${MKDIR} oppd
	@${MKDIR} oppz
	@${MKDIR} opprz
	@${MKDIR} opprdz
	@${MKDIR} rm
	@${MKDIR} syms
	@${MKDIR} Targets
	@${MKDIR} Targets.PlugPlayRM
	@${MKDIR} Targets.PlugPlayRM.!PlugPlay
	@${MKDIR} Targets.PlugPlayAp
	@${MKDIR} Targets.PlugPlayAp.!PlugPlay
	create ${DIRS}
	stamp  ${DIRS}


# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
#

rom_link:
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

o.ModuleWrap: s.ModuleWrap s.AppName s.ResFiles
	${AS} ${AFLAGSRAMZ} s.ModuleWrap $@

oz.ModuleWrap: s.ModuleWrap s.AppName s.ResFiles
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
	@echo
	${AWK} -f ${MAKEAPPNAME} ${LDIR}.Messages ${VERSIONNUM} > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI_PP: ${OBJSPP} ${LIBS} ${DIRS}
	@echo
	${LD} -s syms.!RI_PP -o $@ ${OBJSPP} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_PPD: ${OBJSPPD} ${LIBSD} ${DIRS}
	@echo
	${LD} -debug -s syms.!RI_PPD -o $@ ${OBJSPPD} ${LIBSD}

${MODULE}: o.ModuleWrap ${OBJSPPRZ} ${LIBSZ} ${CLIB} ${DIRS}
	@echo
	${LD} -s syms.RAMMod -o $@ -module o.ModuleWrap ${OBJSPPRZ} ${LIBSZ} ${CLIB}

${MODULE}D: o.ModuleWrap ${OBJSPPRDZ} ${LIBSDZ} ${CLIB} ${DIRS}
	@echo
	${LD} -s syms.ROMDMod -o $@ -module o.ModuleWrap ${OBJSPPRDZ} ${LIBSDZ} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSPPZ} ${ROMCSTUBS} ${LIBSZ} ${DIRS}
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSPPZ} ${LIBSZ} ${ROMCSTUBS}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
