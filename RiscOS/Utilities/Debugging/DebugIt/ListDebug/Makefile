# Makefile for ListDebug
# ======================
#

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= ListDebug
APP		= !ListDebug
LDIR		= Resources.${LOCALE}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = ${RESDIR}.${COMPONENT}

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= ${MemCheckCC} cc
CMHG		= cmhg
CP		= copy
LD		= link #${MemCheckLD} link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= -depend !Depend -nocache -stamp -quit ${THROWBACK}
CFLAGS		= -depend !Depend -ffah ${INCLUDES} -wp ${THROWBACK} -fn
CPFLAGS		= ~cfr~v~n
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS		= -fn -DDEBUGLIB
DDFLAGS		= -fn -DDEBUGLIB
DZFLAGS		= -fn -DDEBUGLIB
DDZFLAGS	= -fn -DDEBUGLIB


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>

CLIB		= C:o.stubs
CPPLIB		= CPPLib:o.c++lib
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib

LIBS = \
 ${WIMPLIB} \
 ${TOOLBOXLIB} \
 ${EVENTLIB} \
 ${CLIB} \

# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:

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
 ${LDIR}.!Sprites22 \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.alloc	\
 o.debug	\
 o.debugwin	\
 o.error	\
 o.main 	\
 o.message	\

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -c -o $@ $<


# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@*echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
	@*echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	@echo
	${CP} ${LDIR}.!Boot			${INSTDIR}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.!Run			${INSTDIR}.!Run 	   	${CPFLAGS}
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites	   	${CPFLAGS}
	${CP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22   	${CPFLAGS}
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}
	${CP} ${LDIR}.Res			${INSTDIR}.Res			${CPFLAGS}

install: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
	-Access ${INSTDIR}.!Boot	rw/rw
	-Access ${INSTDIR}.!Run		rw/rw
	-Access ${INSTDIR}.!Sprites 	rw/rw
	-Access ${INSTDIR}.!Sprites22	rw/rw
	-Access ${INSTDIR}.!RunImage	rw/rw
	-Access ${INSTDIR}.Messages	rw/rw
	-Access ${INSTDIR}.Res		rw/rw
	@echo
	@*echo ${COMPONENT}: Application installed to ${INSTDIR}

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} $modules
	${MKDIR} ${RESDIR}.User
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites22        ${RESDIR}.!Sprites22	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	@echo starting
	${WIPE}	abs		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	syms		${WFLAGS}
	${RM}	o.dirs
	@echo
	@echo ${COMPONENT}: Cleaned

o.dirs:
	@${MKDIR} abs
	@${MKDIR} o
	@${MKDIR} syms
	@${MKDIR} Targets
	@${MKDIR} Targets.!ListDebug
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

s.AppName: ${LDIR}.Messages
	awk -f awk.AppName ${LDIR}.Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@


# -----------------------------------------------------------------------------
# Dynamic dependencies:
