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

CCFLAGS		= -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} -throwback
ASFLAGS		= -depend !Depend -Stamp -quit
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

REMOTEDBLIB	= ${LIBDIR}.debug.o.lremote


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

OBJSZ = \
 oz.debug \
 oz.inits \
 oz.quits \
 oz.output \
 oz.serial_lib \

LIBS = \
 libs.pd_pl \
 libs.t_pl \
 ${REMOTEDBLIB} \

LIBSZ = \
 libs.pd_m_pl \
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
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.DebugLib dirs
	${CP}	h.DebugLib	${EXPORT_DIR}.h.DebugLib ${CPFLAGS}
	${CP}   VersionNum      ${EXPORT_DIR}.VersionNum ${CPFLAGS}
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
o.debug:	c.debug
o.debug:	h.include
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.debug:	h.PDebug
o.debug:	h.Trace
o.debug:	C:debug.h.remote
o.debug:	h.serial_lib
o.debug:	C:h.kernel
o.debug:	h.DebugLib
o.debug:	h.debug
o.debug:	h.inits
o.debug:	h.output
o.debug:	h.quits
o.debug:	h.globals
o.debug:	VersionNum
o.debug:	c.debug
o.debug:	h.include
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.debug:	h.PDebug
o.debug:	h.Trace
o.debug:	C:debug.h.remote
o.debug:	h.serial_lib
o.debug:	C:h.kernel
o.debug:	h.DebugLib
o.debug:	h.debug
o.debug:	h.inits
o.debug:	h.output
o.debug:	h.quits
o.debug:	h.globals
o.debug:	VersionNum
o.inits:	c.inits
o.inits:	h.include
o.inits:	C:h.kernel
o.inits:	C:h.swis
o.inits:	h.PDebug
o.inits:	h.Trace
o.inits:	C:debug.h.remote
o.inits:	h.serial_lib
o.inits:	C:h.kernel
o.inits:	h.DebugLib
o.inits:	h.output
o.inits:	h.inits
o.inits:	h.debug
o.inits:	h.globals
o.quits:	c.quits
o.quits:	h.include
o.quits:	C:h.kernel
o.quits:	C:h.swis
o.quits:	h.PDebug
o.quits:	h.Trace
o.quits:	C:debug.h.remote
o.quits:	h.serial_lib
o.quits:	C:h.kernel
o.quits:	h.DebugLib
o.quits:	h.quits
o.quits:	h.globals
o.output:	c.output
o.output:	h.include
o.output:	C:h.kernel
o.output:	C:h.swis
o.output:	h.PDebug
o.output:	h.Trace
o.output:	C:debug.h.remote
o.output:	h.serial_lib
o.output:	C:h.kernel
o.output:	h.DebugLib
o.output:	h.output
o.output:	h.debug
o.output:	h.globals
o.serial_lib:	c.serial_lib
o.serial_lib:	C:h.kernel
o.serial_lib:	C:h.swis
o.serial_lib:	h.serial_lib
o.serial_lib:	C:h.kernel
oz.debug:	c.debug
oz.debug:	h.include
oz.debug:	C:h.kernel
oz.debug:	C:h.swis
oz.debug:	h.PDebug
oz.debug:	h.Trace
oz.debug:	C:debug.h.remote
oz.debug:	h.serial_lib
oz.debug:	C:h.kernel
oz.debug:	h.DebugLib
oz.debug:	h.debug
oz.debug:	h.inits
oz.debug:	h.output
oz.debug:	h.quits
oz.debug:	h.globals
oz.debug:	VersionNum
oz.inits:	c.inits
oz.inits:	h.include
oz.inits:	C:h.kernel
oz.inits:	C:h.swis
oz.inits:	h.PDebug
oz.inits:	h.Trace
oz.inits:	C:debug.h.remote
oz.inits:	h.serial_lib
oz.inits:	C:h.kernel
oz.inits:	h.DebugLib
oz.inits:	h.output
oz.inits:	h.inits
oz.inits:	h.debug
oz.inits:	h.globals
oz.quits:	c.quits
oz.quits:	h.include
oz.quits:	C:h.kernel
oz.quits:	C:h.swis
oz.quits:	h.PDebug
oz.quits:	h.Trace
oz.quits:	C:debug.h.remote
oz.quits:	h.serial_lib
oz.quits:	C:h.kernel
oz.quits:	h.DebugLib
oz.quits:	h.quits
oz.quits:	h.globals
oz.output:	c.output
oz.output:	h.include
oz.output:	C:h.kernel
oz.output:	C:h.swis
oz.output:	h.PDebug
oz.output:	h.Trace
oz.output:	C:debug.h.remote
oz.output:	h.serial_lib
oz.output:	C:h.kernel
oz.output:	h.DebugLib
oz.output:	h.output
oz.output:	h.debug
oz.output:	h.globals
oz.serial_lib:	c.serial_lib
oz.serial_lib:	C:h.kernel
oz.serial_lib:	C:h.swis
oz.serial_lib:	h.serial_lib
oz.serial_lib:	C:h.kernel
