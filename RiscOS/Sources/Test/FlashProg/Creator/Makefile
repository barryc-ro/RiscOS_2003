# Makefile for NC ROM image creator software, MakeNCimg
# =====================================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1999-01-29	ADH	Created from NCRmgr Makefile.

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= MakeNCimg

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
 ${TARGET}

# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.MakeNCimg

OBJSD = \
 od.MakeNCimg


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${AFLAGS}
		@${AS} ${AFLAGS} $< $@



# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (Disc)

install_common: ${FILES}
	@echo
	${CP} ${TARGET}		${INSTDIR}.${COMPONENT}		${CPFLAGS}~n

install: install_common
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	@echo ${COMPONENT}: Debug application installed ${INSTDIR}

clean:
	@echo starting
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	map		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	od		${WFLAGS}
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
	@${MKDIR} syms
	@${MKDIR} Targets
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
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
	@echo
	${LD} -debug -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
