# Makefile for flashlib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-10-07	BAL		Created
# 1998-02-06	JBeranek	Added rules for podule version


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
NFXLIBDIR	= <Lib$Dir>.Netfax
EXPORT_DIR	= <Lib$Dir>.Netfax.FlashLib


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

CCFLAGS		= -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS}
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:
DFLAGS		= 
TFLAGS		= -DPODULE_1


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= flashlib
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENT}z
TARGET_T        = ${COMPONENT}_t
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS		= o.flashlib

OBJSZ		= z.flashlib

OBJS_T		= ot.flashlib

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .ot .z .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.ot:;	   ${CC} ${CCFLAGS} ${TFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zm -zps1 -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${TARGET} ${TARGETZ} $(TARGET_T)


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} ot ${WFLAGS}
	${WIPE} z ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	${RM} ${TARGET_T}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.flashlib dirs
	${CP}	h.flashlib	${EXPORT_DIR}.h.flashlib	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} ${TARGET_T} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	${CP}	${TARGET_T}	${EXPORT_DIR}.o.${TARGET_T}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} ot
	${MKDIR} z

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${NFXLIBDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ}

${TARGET_T}: ${OBJS_T} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET_T} ${OBJS_T}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
