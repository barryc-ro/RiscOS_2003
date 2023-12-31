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
# 06-Sep-2001	ADH	Added ability to build the SGI demo application
#			variant.

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
MKDIR		= do mkdir -p
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
XCP		= x copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe
XWIPE		= x wipe
AWK		= do awk
NOP		= |

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
ASMUTILS	= <Lib$Dir>.AsmUtils.o.AsmUtilszm

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
 ${SOCKLIB}zm \
 ${ASMUTILS}

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
 ${SOCKLIB}zm \
 ${ASMUTILS}


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
 opp.Mouse		\
 opp.Overlay		\
 opp.Parameters		\
 opp.Play		\
 opp.PlugIn		\
 opp.Protocols		\
 opp.RMA		\
 opp.URLutils		\
 opp.Utils		\
 o.Fixed		\
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
 oppd.Mouse		\
 oppd.Overlay		\
 oppd.Parameters	\
 oppd.Play		\
 oppd.PlugIn		\
 oppd.Protocols		\
 oppd.RMA		\
 oppd.URLutils		\
 oppd.Utils		\
 o.Fixed		\
 o.ROSLib		\
 o.SetPW

# SGI RAM demo application

OBJSPPS = \
 opps.Filter		\
 opps.FromROSLib	\
 opps.FullScreen	\
 opps.Global		\
 opps.Handlers		\
 opps.Main		\
 opps.ModeInfo		\
 opps.Mouse		\
 opps.Overlay		\
 opps.Parameters	\
 opps.Play		\
 opps.PlugIn		\
 opps.Protocols		\
 opps.RMA		\
 opps.URLutils		\
 opps.Utils		\
 o.Fixed		\
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
 opprz.Mouse		\
 opprz.Overlay		\
 opprz.Parameters	\
 opprz.Play		\
 opprz.PlugIn		\
 opprz.Protocols	\
 opprz.RMA		\
 opprz.URLutils		\
 opprz.Utils		\
 o.FilterHelp		\
 o.Fixed		\
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
 opprdz.Mouse		\
 opprdz.Overlay		\
 opprdz.Parameters	\
 opprdz.Play		\
 opprdz.PlugIn		\
 opprdz.Protocols	\
 opprdz.RMA		\
 opprdz.URLutils	\
 opprdz.Utils		\
 o.FilterHelp		\
 o.Fixed		\
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
 oppz.Mouse		\
 oppz.Overlay		\
 oppz.Parameters	\
 oppz.Play		\
 oppz.PlugIn		\
 oppz.Protocols		\
 oppz.RMA		\
 oppz.URLutils		\
 oppz.Utils		\
 o.FilterHelp		\
 o.Fixed		\
 o.ROSLib		\
 o.SetPW


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .opp .oppd .opps .oppz .opprz .opprdz .s .c

.c.opp:;	@echo
		@echo Compiling $< (application)
		@echo ${PPFLAGS}
		@${CC} ${CFLAGS} ${PPFLAGS}        -c -UROM -DAPPLICATION -o $@ $<

.c.oppd:;	@echo
		@echo Compiling $< (debug application)
		@echo ${PPFLAGSD}
		@${CC} ${CFLAGS} ${PPFLAGSD}       -c -UROM -DAPPLICATION -g -o $@ $<

.c.opps:;	@echo
		@echo Compiling $< (application, SGI demo build)
		@echo ${PPFLAGS} -DSGI
		@${CC} ${CFLAGS} ${PPFLAGS}        -c -UROM -DAPPLICATION -DSGI -o $@ $<

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
	${MKDIR} ${INSTDIR}
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}n
	${XCP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22		${CPFLAGS}n
	${XCP} ${LDIR}.!Sprites23		${INSTDIR}.!Sprites23		${CPFLAGS}n
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}~n
	${AWK} -f ${ADDTOMESSAGES} ${VERSIONNUM} >> ${INSTDIR}.Messages
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}n
	${CP} ${LDIR}.Sprites			${INSTDIR}.Sprites		${CPFLAGS}n
	${XCP} ${LDIR}.Sprites22		${INSTDIR}.Sprites22		${CPFLAGS}n
	${XCP} ${LDIR}.Sprites23		${INSTDIR}.Sprites23		${CPFLAGS}n
	${XCP} ${LDIR}.About			${INSTDIR}.About		${CPFLAGS}n
	${XWIPE} ${INSTDIR}.About.CVS	${WFLAGS}

install: install_${TYPE}
	${NOP}

install_: install_app
	${NOP}

install_app: install_common
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application (absolute !RunImage) installed to ${INSTDIR}

install_mod:
	${MKDIR} ${INSTDIR}
	${CP} ${TARGET}				${INSTDIR}.${COMPONENT}		${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Module installed to ${INSTDIR}

install_debug: install_common
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Debug application (absolute !RunImage) installed to ${INSTDIR}

install_sgi: install_common
	${XWIPE} ${INSTDIR}.Sprites	${WFLAGS}
	${CP} ${LDIR}.SpritesSGI		${INSTDIR}.Sprites		${CPFLAGS}~n
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: SGI demo application (absolute !RunImage) installed to ${INSTDIR}

install_ram: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application (module !RunImage) installed to ${INSTDIR}

install_ramdebug: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application (module !RunImage) installed to ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: ROM module installed to ${INSTDIR}

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}.About
	${CP} ${LDIR}.ROM.!Boot		${RESAPP}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESAPP}.!Sprites	${CPFLAGS}
	${XCP} ${LDIR}.!Sprites22	${RESAPP}.!Sprites22	${CPFLAGS}
	${XCP} ${LDIR}.!Sprites23	${RESAPP}.!Sprites23	${CPFLAGS}
	${CP} ${LDIR}.Sprites		${RESDIR}.Sprites	${CPFLAGS}
	${XCP} ${LDIR}.Sprites22	${RESDIR}.Sprites22	${CPFLAGS}
	${XCP} ${LDIR}.Sprites23	${RESDIR}.Sprites23	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${AWK} -f ${ADDTOMESSAGES} ${VERSIONNUM} >> ${RESDIR}.Messages
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.About		${RESDIR}.About		${CPFLAGS}
	${XWIPE} ${RESDIR}.About.CVS	${WFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	${XWIPE}	rm		${WFLAGS}
	${XWIPE}	abs		${WFLAGS}
	${XWIPE}	aof		${WFLAGS}
	${XWIPE}	linked		${WFLAGS}
	${XWIPE}	o		${WFLAGS}
	${XWIPE}	oz		${WFLAGS}
	${XWIPE}	opp		${WFLAGS}
	${XWIPE}	oppd		${WFLAGS}
	${XWIPE}	opps		${WFLAGS}
	${XWIPE}	oppz		${WFLAGS}
	${XWIPE}	opprz		${WFLAGS}
	${XWIPE}	opprdz		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	${DIRS}
	@echo
	@echo ${COMPONENT}: Cleaned

clean_all: clean
	@echo
	${XWIPE}	syms		${WFLAGS}
	${XWIPE}	Targets		${WFLAGS}
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
	@${MKDIR} opps
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
	@${MKDIR} Targets.PlugPlaySG
	@${MKDIR} Targets.PlugPlaySG.!PlugPlay
	@echo
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

abs.!RI_PPS: ${OBJSPPS} ${LIBS} ${DIRS}
	@echo
	${LD} -s syms.!RI_PPS -o $@ ${OBJSPPS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

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
