# Makefile for PDebug
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXPORT_DIR	= <Lib$Dir>.PDebug


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= cc
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
WIPE		= -wipe
TRIMAOF		= aof.trimaof
AOF2ALF		= aof.aof2alf

CCFLAGS		= -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} -throwback
ASFLAGS		= -depend !Depend -Stamp -NoCache -CloseExec -PreDefine "Desk_AS_EMBED_FNNAMES SETA 1" -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
LNFLAGS		= -aof
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>
CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym

DESK		= ${LIBDIR}.Desk.o.Desk


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= PDebug
TARGET		= Lib
TARGETZ		= Lib_M
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS = \
 o.Send \

OBJSZ = \
 oz.Send \

LIBS = \
 ${DESK} \

LIBSZ = \
 ${DESK}_M \

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -zm -zps1 -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${TARGET} ${TARGETZ}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGET}_PL
	${RM} ${TARGETZ}
	${RM} ${TARGETZ}_PL
	@echo ${COMPONENT}: cleaned

export_hdrs: h.PDebug dirs
	${CP}	h.PDebug	${EXPORT_DIR}.h.PDebug 		${CPFLAGS}
	@echo * ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGET}_PL	${EXPORT_DIR}.o.${TARGET}_PL	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	${CP}	${TARGETZ}_PL	${EXPORT_DIR}.o.${TARGETZ}_PL	${CPFLAGS}
	${CP}	VersionNum	${EXPORT_DIR}.VersionNum	${CPFLAGS}
	@echo * ${COMPONENT}: export complete (libs)

o.dirs:
	${MKDIR} o
	${MKDIR} oz
	create o.dirs
	stamp  o.dirs

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} ${LIBS} o.dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}
	${LD} ${LNFLAGS} ${LinkExtra} -o $@_PL ${OBJS} ${LIBS}
	${TRIMAOF} $@_PL $@_PL PDebug_*
	${AOF2ALF} $@_PL

${TARGETZ}: ${OBJSZ} ${LIBSZ} o.dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ}
	${LD} ${LNFLAGS} ${LinkExtra} -o $@_PL ${OBJSZ} ${LIBSZ}
	${TRIMAOF} $@_PL $@_PL PDebug_*
	${AOF2ALF} $@_PL

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
