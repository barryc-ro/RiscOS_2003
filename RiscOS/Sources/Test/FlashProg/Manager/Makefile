# Makefile for NC ROM image management software, NCRmgr.
# ======================================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1999-01-05	ADH	Created from Browse Makefile.

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= NCRmgr
APP		= !NCRmgr
MODULE		= rm.!NCRmgr
ROM_MODULE	= aof.NCRmgr
LDIR		= Resources.${LOCALE}.${SYSTEM}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.${COMPONENT}
RESAPP		= <resource$dir>.Apps.${APP}


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
CFLAGS		= ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wpc
CPFLAGS		= ~cfr~v
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# BASEFLAGS holds common compile time options that all builds will use.

BASEFLAGS	=

# Additional build-dependent flags.

DFLAGS		= ${BASEFLAGS}
DDFLAGS		= ${DFLAGS} -fn -DTRACE
DZFLAGS		= ${BASEFLAGS}
DDZFLAGS	= ${DZFLAGS} -fn -DTRACE


# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap
ZLIB		= C:zlib.o.zlib
FLEXLIB		= tbox:o.flexlib
INETLIB		= TCPIPLibs:o.inetlib

LIBSM = \
 ${FLEXLIB}zm \
 ${INETLIB}zm \
 ${ZLIB}zm

LIBS = \
 ${CLIB} \
 ${FLEXLIB} \
 ${INETLIB} \
 ${ZLIB}

LIBSD = \
 ${CLIB} \
 ${FLEXLIB} \
 ${INETLIB} \
 ${ZLIB}


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,TCPIPLibs:

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.Messages \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 ${LDIR}.Sprites \
 ${LDIR}.!Sprites


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.ASCIIdraw		\
 o.Colours		\
 o.FileOps		\
 o.FromROSLib		\
 o.Global		\
 o.KeyControl		\
 o.Main			\
 o.Redraw		\
 o.Repository		\
 o.RMA			\
 o.State		\
 o.Utils		\
 o.ROSLib		\
 o.svcprint

OBJSZ = \
 oz.ASCIIdraw		\
 oz.Colours		\
 oz.FileOps		\
 oz.FromROSLib		\
 oz.Global		\
 oz.KeyControl		\
 oz.Main		\
 oz.Redraw		\
 oz.Repository		\
 oz.RMA			\
 oz.State		\
 oz.svcprint		\
 oz.Utils		\
 o.ROSLib

OBJSD = \
 od.ASCIIdraw		\
 od.Colours		\
 od.FileOps		\
 od.FromROSLib		\
 od.Global		\
 od.KeyControl		\
 od.Main		\
 od.Redraw		\
 od.Repository		\
 od.RMA			\
 od.State		\
 od.svcprint		\
 od.Utils		\
 o.ROSLib

OBJSDZ = \
 odz.ASCIIdraw		\
 odz.Colours		\
 odz.FileOps		\
 odz.FromROSLib		\
 odz.Global		\
 odz.KeyControl		\
 odz.Main		\
 odz.Redraw		\
 odz.Repository		\
 odz.RMA		\
 odz.State		\
 odz.svcprint		\
 odz.Utils		\
 o.ROSLib


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
		@${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

.c.odz:;	@echo
		@echo Compiling $<
		@echo ${DDZFLAGS}
		@${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -DROM -o $@ $<

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
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	-${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}n
	-${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}n
	-${CP} ${LDIR}.Sprites			${INSTDIR}.Sprites		${CPFLAGS}n
	-${CP} ${LDIR}.Modules			${INSTDIR}.Modules		${CPFLAGS}n


install: install_common
	-${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	-${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}~n
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}
	@echo ${COMPONENT}:
	@echo ${COMPONENT}: To allow the "Build production disc" facility to work,
	@echo ${COMPONENT}: create a directory called 'Programmer' inside !NCRmgr.
	@echo ${COMPONENT}: Copy any programmer software into this, including the
	@echo ${COMPONENT}: 'Auto' Obey file which runs it. This file is executed
	@echo ${COMPONENT}: by an NC when told to boot from a Zip disc, e.g. by
	@echo ${COMPONENT}: holding down Space after a hard reset.
	@echo ${COMPONENT}:
	@echo ${COMPONENT}: For more details see 2205,064/FS for NCRmgr and
	@echo ${COMPONENT}: 2205,063/FS for NCRwriter (programming software).
	@echo

installd: install_common
	-${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	-${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}~n
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
	-${CP} ${LDIR}.ROM.!Boot	${RESAPP}.!Boot		${CPFLAGS}
	-${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	-${CP} ${LDIR}.!Sprites		${RESDIR}.!Sprites	${CPFLAGS}
	-${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	-${CP} ${LDIR}.Sprites		${RESDIR}.Sprites	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied

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
	@${MKDIR} Targets.NCRmgr
	@${MKDIR} Targets.NCRmgr.!NCRmgr
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
	${WIPE}	syms		${WFLAGS}
	${WIPE}	Targets		${WFLAGS}
	@echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

rom_link:
	${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
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

rm.!NCRmgrD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
