# Makefile for Daytona Fax
# ========================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1997-10-06	BAL	Created
# 1998-01-28	MGR	Beautified and made parent of FaxSupport
# 1998-05-13	BAL	Added export of headers

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Fax
SUBCOMPONENT	= FaxSupport
APP		= !${COMPONENT}
MODULE		= rm.${APP}
ROM_MODULE	= aof.${COMPONENT}
LDIR		= Resources.${LOCALE}


# -----------------------------------------------------------------------------
# Paths for Messages module and Exports
#

RESDIR          = ${RESDIR}.${COMPONENT}
RESAPP          = <Resource$Dir>.Apps.${APP}

LIBDIR		= <Lib$Dir>
FAXLIBDIR	= ${LIBDIR}.fax

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
SQUEEZE		= @echo Not doing a squeeze
ACCESS          = @echo Not doing an -access
WIPE		= -wipe

FEATURES	= -wp -fah
AFLAGS		= ${THROWBACK} -depend !Depend -nocache ${ASMINCLUDES}
CFLAGS		= ${THROWBACK} -depend !Depend ${FEATURES} ${INCLUDES}
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

MGRLIB		= ${LIBDIR}.mgrlib.o.mgrlib

LIBSM = \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB}m \
 ${MGRLIB}

LIBS = \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${MGRLIB} \
 ${CLIB} \

LIBSD = \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${MGRLIB} \
 ${CLIB} \


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,${LIBDIR}
#ASMINCLUDES	= "-I {LIBDIR}.^.hdr"

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.!Sprites22 \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 ${LDIR}.!Sprites \
 ${LDIR}.!Sprites22 \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.faxdebug\
 o.main\
 o.mgrlib\
 o.text2tiff\
 o.tiff\
 o.tiffconv\
 o.tifflo\

OBJSZ = \
 oz.faxdebug\
 oz.main\
 oz.mgrlib\
 oz.text2tiff\
 oz.tiff\
 oz.tiffconv\
 o.tifflo\

OBJSD = \
 od.faxdebug\
 od.main\
 od.mgrlib\
 od.text2tiff\
 od.tiff\
 od.tiffconv\
 o.tifflo\

#OBJSDZ =


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		@echo
		@echo Compiling $< ( ${FEATURES} ${DFLAGS})
		@${CC} ${CFLAGS} ${DFLAGS} -c -o $@ $<

.c.oz:;		@echo
		@echo Compiling $< ( ${FEATURES} ${DZFLAGS}-zM -zps1 -DROM )
		@${CC} ${CFLAGS} ${DZFLAGS} -c -zM -zps1 -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $< ( ${FEATURES} ${DDFLAGS} -g )
		@${CC} ${CFLAGS} ${DDFLAGS} -c -g -o $@ $<

.c.odz:;	@echo
		@echo Compiling $< ( ${FEATURES} ${DDZFLAGS} -g -zM -zps1 -DROM )
		@${CC} ${CFLAGS} ${DDZFLAGS} -c -g -zM -zps1 -DROM -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@${AS} ${AFLAGS} $< $@


# -----------------------------------------------------------------------------
# Main rules
#

#all: ${FILES}
#	@echo
#	@echo ${APP}: Application built (Disc)

rom: ${ROM_MODULE} subcomponent
	@echo
	@echo ${APP}: Module built (ROM) in ${ROM_MODULE}

install_common: ${FILES} subcomponent
	@echo
	@echo Copying to target directory
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}
	${CP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22		${CPFLAGS}
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}

install: install_common
	@echo
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	@echo
	${ACCESS} ${INSTDIR}.!Boot	lr/r
	${ACCESS} ${INSTDIR}.!Run	lr/r
	${ACCESS} ${INSTDIR}.!RunImage	lr/r
	${ACCESS} ${INSTDIR}.Messages	lr/r
	${ACCESS} ${INSTDIR}.Res		lr/r
	@echo
	@echo ${APP}: Application installed in ${INSTDIR}

subcomponent:
        @echo
        @echo Doing ${SUBCOMPONENT} subcomponent...
        @dir ${SUBCOMPONENT}
        @amu ${MFLAGS}
        @dir ^
        @echo
        @echo Back to ${APP}...

installd: install_common
	@echo
	${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}
	@echo
	@echo ${APP}: Debug application installed in ${INSTDIR}

install_rom: ${ROM_MODULE} subcomponent
	@echo
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${APP}: Module installed (ROM)

export: export_${PHASE} subcomponent

export_hdrs: export_dirs
	${CP} h.global	${FAXLIBDIR}.h.* ${CPFLAGS}
	${CP} h.RCcodes	${FAXLIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: Export complete (hdrs)

export_libs:
        @echo ${COMPONENT}: No libraries to export

resources: subcomponent
	@echo
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESDIR}.!Sprites	${CPFLAGS}
	${CP} ${LDIR}.!Sprites22	${RESDIR}.!Sprites22	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	@echo
	@echo ${APP}: Resource files copied to Messages module

clean: subcomponent
	@echo
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	map		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	od		${WFLAGS}
	${WIPE}	oz		${WFLAGS}
	${WIPE}	odz		${WFLAGS}
	${WIPE}	rm		${WFLAGS}
	${WIPE}	syms		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	@${RM}	o.dirs
	@echo
	@echo ${APP}: Cleaned

o.dirs:
	@echo
	@echo Creating directories
	${MKDIR} abs
	${MKDIR} aof
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} o
	${MKDIR} od
	${MKDIR} oz
	${MKDIR} odz
	${MKDIR} rm
	${MKDIR} syms
	${MKDIR} Targets
	${MKDIR} Targets.${APP}
	@create o.dirs
	@stamp  o.dirs

export_dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${FAXLIBDIR}
	${MKDIR} ${FAXLIBDIR}.h


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
	@echo
	${WIPE}	Targets		${WFLAGS}
	@echo
	@echo ${APP}: Cleaned all


# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
#

rom_link: subcomponent
	@echo
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	@echo
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${APP}: rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
	@echo
	@echo Assembling s.ModuleWrap
	@${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	@${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
	@echo
	awk -f awk.AppName ${LDIR}.Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	@echo
	@echo Linking $@ ( ${OBJS} )
	@${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
	@echo
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
	@echo
	@echo Linking $@ ( -debug ${OBJSD} )
	@${LD} -debug -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
	@echo
	@echo Linking $@ ( oz.ModuleWrap ${OBJSZ} )
	@${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
	@echo
	@echo Linking $@ ( oz.ModuleWrap ${OBJSZ} )
	@${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.${APP}D: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	@echo
	@echo Linking $@ ( oz.ModuleWrap ${OBJSDZ} )
	@${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
