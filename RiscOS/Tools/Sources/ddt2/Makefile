# Makefile for DDT2
# =================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 16-Oct-2000	ADH	Created from PlugPlay Makefile.

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
XCP		= x copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe
XWIPE		= x wipe
AWK		= do awk

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

DDT2FLAGS	= ${BASEFLAGS}
DDT2FLAGSD	= ${BASEFLAGS} ${BASEFLAGSD}
DDT2FLAGSZ	= ${BASEFLAGS}
DDT2FLAGSDZ	= ${BASEFLAGS} ${BASEFLAGSD}


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

# RAM application "DDT2"

OBJSDDT2 = \
 opp.BEManager		\
 opp.BEMenu		\
 opp.Breakpoint		\
 opp.DBEDemo		\
 opp.DynaMenu		\
 opp.Errors		\
 opp.EventMenu		\
 opp.File		\
 opp.FromROSLib		\
 opp.Global		\
 opp.Handlers		\
 opp.List		\
 opp.ListAdd		\
 opp.ListMenu		\
 opp.Main		\
 opp.MainMenu		\
 opp.Menus		\
 opp.NewProject		\
 opp.Projects		\
 opp.RMA		\
 opp.ServMenu		\
 opp.TGUtils		\
 opp.UpCallMenu		\
 opp.Utils		\
 opp.Watchpoint		\
 opp.Windows		\
 o.ROSLib

OBJSDDT2D = \
 oppd.BEManager		\
 oppd.BEMenu		\
 oppd.Breakpoint	\
 oppd.DBEDemo		\
 oppd.DynaMenu		\
 oppd.Errors		\
 oppd.EventMenu		\
 oppd.File		\
 oppd.FromROSLib	\
 oppd.Global		\
 oppd.Handlers		\
 oppd.List		\
 oppd.ListAdd		\
 oppd.ListMenu		\
 oppd.Main		\
 oppd.MainMenu		\
 oppd.Menus		\
 oppd.NewProject	\
 oppd.Projects		\
 oppd.RMA		\
 oppd.ServMenu		\
 oppd.TGUtils		\
 oppd.UpCallMenu	\
 oppd.Utils		\
 oppd.Watchpoint	\
 oppd.Windows		\
 o.ROSLib

# RAM module "DDT2"

OBJSDDT2RZ = \
 opprz.BEManager	\
 opprz.BEMenu		\
 opprz.Breakpoint	\
 opprz.DBEDemo		\
 opprz.DynaMenu		\
 opprz.Errors		\
 opprz.EventMenu	\
 opprz.File		\
 opprz.FromROSLib	\
 opprz.Global		\
 opprz.Handlers		\
 opprz.List		\
 opprz.ListAdd		\
 opprz.ListMenu		\
 opprz.Main		\
 opprz.MainMenu		\
 opprz.Menus		\
 opprz.NewProject	\
 opprz.Projects		\
 opprz.RMA		\
 opprz.ServMenu		\
 opprz.TGUtils		\
 opprz.UpCallMenu	\
 opprz.Utils		\
 opprz.Watchpoint	\
 opprz.Windows		\
 o.ROSLib

OBJSDDT2RDZ = \
 opprdz.BEManager	\
 opprdz.BEMenu		\
 opprdz.Breakpoint	\
 opprdz.DBEDemo		\
 opprdz.DynaMenu	\
 opprdz.Errors		\
 opprdz.EventMenu	\
 opprdz.File		\
 opprdz.FromROSLib	\
 opprdz.Global		\
 opprdz.Handlers	\
 opprdz.List		\
 opprdz.ListAdd		\
 opprdz.ListMenu	\
 opprdz.Main		\
 opprdz.MainMenu	\
 opprdz.Menus		\
 opprdz.NewProject	\
 opprdz.Projects	\
 opprdz.RMA		\
 opprdz.ServMenu	\
 opprdz.TGUtils		\
 opprdz.UpCallMenu	\
 opprdz.Utils		\
 opprdz.Watchpoint	\
 opprdz.Windows		\
 o.ROSLib

# ROM module "DDT2"; same as RAM module but has "-DROM"
# on CC command line

OBJSDDT2Z = \
 oppz.BEManager		\
 oppz.BEMenu		\
 oppz.Breakpoint	\
 oppz.DBEDemo		\
 oppz.DynaMenu		\
 oppz.Errors		\
 oppz.EventMenu		\
 oppz.File		\
 oppz.FromROSLib	\
 oppz.Global		\
 oppz.Handlers		\
 oppz.List		\
 oppz.ListAdd		\
 oppz.ListMenu		\
 oppz.Main		\
 oppz.MainMenu		\
 oppz.Menus		\
 oppz.NewProject	\
 oppz.Projects		\
 oppz.RMA		\
 oppz.ServMenu		\
 oppz.TGUtils		\
 oppz.UpCallMenu	\
 oppz.Utils		\
 oppz.Windows		\
 oppz.Watchpoint	\
 o.ROSLib


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .opp .oppd .oppz .opprz .opprdz .s .c

.c.opp:;	@echo
		@echo Compiling $< (application)
		@echo ${DDT2FLAGS}
		@${CC} ${CFLAGS} ${DDT2FLAGS}        -c -UROM -DAPPLICATION -o $@ $<

.c.oppd:;	@echo
		@echo Compiling $< (debug application)
		@echo ${DDT2FLAGSD}
		@${CC} ${CFLAGS} ${DDT2FLAGSD}       -c -UROM -DAPPLICATION -g -o $@ $<

.c.oppz:;	@echo
		@echo Compiling $< (ROM module)
		@echo ${DDT2FLAGSZ}
		@${CC} ${CFLAGS} ${DDT2FLAGSZ}       -c -zM -DROM -UAPPLICATION -o $@ $<

.c.opprz:;	@echo
		@echo Compiling $< (RAM module)
		@echo ${DDT2FLAGS}
		@${CC} ${CFLAGS} ${DDT2FLAGS}        -c -zM -UROM -UAPPLICATION -o $@ $<

.c.opprdz:;	@echo
		@echo Compiling $< (RAM debug module)
		@echo ${DDT2FLAGSD}
		@${CC} ${CFLAGS} ${DDT2FLAGSD}       -c -zM -UROM -UAPPLICATION -g -o $@ $<

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
	${XCP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22		${CPFLAGS}n
	${XCP} ${LDIR}.!Sprites23		${INSTDIR}.!Sprites23		${CPFLAGS}n
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}~n
	${AWK} -f ${ADDTOMESSAGES} ${VERSIONNUM} >> ${INSTDIR}.Messages
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}n
	${CP} ${LDIR}.Sprites			${INSTDIR}.Sprites		${CPFLAGS}n
	${XCP} ${LDIR}.Sprites22		${INSTDIR}.Sprites22		${CPFLAGS}n
	${XCP} ${LDIR}.Sprites23		${INSTDIR}.Sprites23		${CPFLAGS}n
	${CP} ${LDIR}.UpCalls			${INSTDIR}.UpCalls		${CPFLAGS}n
	${CP} ${LDIR}.Services			${INSTDIR}.Services		${CPFLAGS}n

install: install_common
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}
	@echo

installd: install_common
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Debug application installed to ${INSTDIR}
	@echo
	@Echo DDT + ViewFinder crash avoidance:
	If "<ViewFinder$Enable>" = "1" Then SetType ${INSTDIR}.!RunImage Absolute
	@Echo

install_ram: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Run			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: RAM module installed to ${INSTDIR}
	@echo

install_ramd: install_common
	${CP} ${TARGET}				${INSTDIR}.${APP}		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!Boot			${INSTDIR}.!Boot		${CPFLAGS}~n
	${CP} ${LDIR}.RAM.!RunD			${INSTDIR}.!Run			${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: RAM debug module installed to ${INSTDIR}
	@echo

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: ROM Module installed to ${INSTDIR}
	@echo

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
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
	${CP} ${LDIR}.UpCalls		${RESDIR}.UpCalls	${CPFLAGS}
	${CP} ${LDIR}.Services		${RESDIR}.Services	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module
	@echo

clean:
	${XWIPE}	rm		${WFLAGS}
	${XWIPE}	abs		${WFLAGS}
	${XWIPE}	aof		${WFLAGS}
	${XWIPE}	linked		${WFLAGS}
	${XWIPE}	o		${WFLAGS}
	${XWIPE}	oz		${WFLAGS}
	${XWIPE}	opp		${WFLAGS}
	${XWIPE}	oppd		${WFLAGS}
	${XWIPE}	oppz		${WFLAGS}
	${XWIPE}	opprz		${WFLAGS}
	${XWIPE}	opprdz		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	${DIRS}
	@echo
	@echo ${COMPONENT}: Cleaned
	@echo

clean_all: clean
	${XWIPE}	syms		${WFLAGS}
	${XWIPE}	Targets		${WFLAGS}
	@echo
	@echo ${COMPONENT}: Cleaned all
	@echo

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
	@${MKDIR} Targets.DDT2RM
	@${MKDIR} Targets.DDT2RM.!DDT2
	@${MKDIR} Targets.DDT2Ap
	@${MKDIR} Targets.DDT2Ap.!DDT2
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

abs.!RI_DDT2: ${OBJSDDT2} ${LIBS} ${DIRS}
	@echo
	${LD} -s syms.!RI_DDT2 -o $@ ${OBJSDDT2} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_DDT2D: ${OBJSDDT2D} ${LIBSD} ${DIRS}
	@echo
	${LD} -debug -s syms.!RI_DDT2D -o $@ ${OBJSDDT2D} ${LIBSD}

${MODULE}: o.ModuleWrap ${OBJSDDT2RZ} ${LIBSZ} ${CLIB} ${DIRS}
	@echo
	${LD} -s syms.RAMMod -o $@ -module o.ModuleWrap ${OBJSDDT2RZ} ${LIBSZ} ${CLIB}

${MODULE}D: o.ModuleWrap ${OBJSDDT2RDZ} ${LIBSDZ} ${CLIB} ${DIRS}
	@echo
	${LD} -s syms.ROMDMod -o $@ -module o.ModuleWrap ${OBJSDDT2RDZ} ${LIBSDZ} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSDDT2Z} ${ROMCSTUBS} ${LIBSZ} ${DIRS}
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSDDT2Z} ${LIBSZ} ${ROMCSTUBS}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
