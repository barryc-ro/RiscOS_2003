# Makefile for NumConvert
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-11-22	BAL		Created
# 1998-02-17    MGR             Modified a bit (was for mgrlib)


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXPORT_DIR	= <Lib$Dir>.NumConvert


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

CCFLAGS		= -c -depend !Depend -strict -ffah ${INCLUDES} ${DFLAGS}
ASFLAGS		= -depend !Depend
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


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= NumConvert
TARGET		= numconv
TARGETZ		= numconvzm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.numconvert
 

OBJSZ =\
 z.numconvert


HDRS =\
 h.numconvert


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .z .s .h .c
.c.o:;     @echo
           ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     @echo
           ${CC} ${CCFLAGS} -zm -o $@ $<
.s.o:;     @echo
           ${AS} ${ASFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${TARGET} ${TARGETZ}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

test:
        @echo
        ${CC} ${CCFLAGS} -o ${TARGET}d -D${COMPONENT}_TESTHARNESS c.${COMPONENT}
        @echo
        ${LD} -o ${COMPONENT} ${TARGET}d ${CLIB}
        @echo
        @echo ${COMPONENT}: Test version built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: ROM module installed

clean:
        @echo
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGET}d
	${RM} ${TARGETZ}
	${RM} ${COMPONENT}
	@${RM} o.dirs
	@${RM} o.locdirs
	@echo
	@echo ${COMPONENT}: Cleaned

export_hdrs: ${HDRS} o.dirs
        @echo
	${CP}	h.numconvert	${EXPORT_DIR}.h.numconvert	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} o.dirs
        @echo
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (libs)

o.locdirs:
        @echo
	${MKDIR} o
	${MKDIR} z

o.dirs:
        @echo
	${MKDIR} ${LIBDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o
	@create o.dirs
	@stamp  o.dirs
	@create o.locdirs
	@stamp  o.locdirs
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} o.locdirs
        @echo
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}
	@echo
	@echo ${COMPONENT}: Non-module library built

${TARGETZ}: ${OBJSZ} o.locdirs
        @echo
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ}
	@echo
	@echo ${COMPONENT}: Module library built

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
