# Makefile for NBLib
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXPORT_DIR	= <Lib$Dir>.NBLib


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= ${MEMCHECKCC} cc
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


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= NBLib
TARGET		= NBLib
TARGETZ		= NBLibzm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS = \
 o.nb_dirs \
 o.nb_files \
 o.nb_flags \
 o.nb_gadgets \
 o.nb_misc \
 o.nb_msgtran \
 o.nb_objects \
 o.nb_taskwin \
 o.nb_window

OBJSZ = \
 oz.nb_dirs \
 oz.nb_files \
 oz.nb_flags \
 oz.nb_gadgets \
 oz.nb_misc \
 oz.nb_msgtran \
 oz.nb_objects \
 oz.nb_taskwin \
 oz.nb_window

LIBS =

LIBSZ =

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
	@echo ** ${COMPONENT}: rom module installed

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ** ${COMPONENT}: cleaned

export_hdrs: h.NBLib dirs
	${CP}	h.NBLib	${EXPORT_DIR}.h.NBLib ${CPFLAGS}
	${CP}	h.NBDefs	${EXPORT_DIR}.h.NBDefs ${CPFLAGS}
	${CP}	h.NBStructs	${EXPORT_DIR}.h.NBStructs ${CPFLAGS}
	${CP}	h.NBSwis	${EXPORT_DIR}.h.NBSwis ${CPFLAGS}
	@echo ** ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	@echo ** ${COMPONENT}: export complete (libs)

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
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS} ${LIBS}

${TARGETZ}: ${OBJSZ} o.dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ} ${LIBSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
