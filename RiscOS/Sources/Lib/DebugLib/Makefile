# Makefile for DebugLib
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <Export$Dir>
LIBDIR		= <Lib$Dir>
EXPORT_DIR	= <Lib$Dir>.DebugLib


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

CCFLAGS		= -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~cf~nr~v
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

REMOTEDBLIB	= ${LIBDIR}.debug.o.lremote
PDEBUG		= Libs.PD_PL
PDEBUGZM	= Libs.PD_M_PL
#PDEBUG		= ${LIBDIR}.PDebug.o.Lib_PL
#PDEBUGZM	= ${LIBDIR}.PDebug.o.Lib_M_PL


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= DebugLib
TARGET		= DebugLib
TARGETZ		= DebugLibzm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS = \
 o.debug \
 o.inits \
 o.quits \
 o.output \
 o.serial_lib \
 o.buffering \

OBJSZ = \
 oz.debug \
 oz.inits \
 oz.quits \
 oz.output \
 oz.serial_lib \
 oz.buffering \

LIBS = \
 ${PDEBUG} \
 libs.t_pl \
 ${REMOTEDBLIB} \

LIBSZ = \
 ${PDEBUGZM} \
 libs.t_m_pl \
 ${REMOTEDBLIB}zm \

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -zm -o $@ $<
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
	${CP} 	${TARGET} ${INSTDIR}.${COMPONENT} 		${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.DebugLib dirs
	${CP}	h.DebugLib	${EXPORT_DIR}.h.DebugLib 	${CPFLAGS}
	${CP}   VersionNum      ${EXPORT_DIR}.VersionNum 	${CPFLAGS}
	@echo * ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
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

${TARGET}: ${OBJS} o.dirs
	${LD} ${LNFLAGS} -o ${TARGET} ${OBJS} ${LIBS}

${TARGETZ}: ${OBJSZ} o.dirs
	${LD} ${LNFLAGS} -o ${TARGETZ} ${OBJSZ} ${LIBSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
