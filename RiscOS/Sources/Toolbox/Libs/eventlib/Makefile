# Makefile for eventlib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-09-03	BAL		Created
# 1997-09-23	BAL		Fixed broken export for module build
# 				Removed rogue reference to TARGETZM in
#                               install_rom rule
#


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>


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

CFLAGS		= -c -depend !Depend -ffah -strict ${INCLUDES} ${DFLAGS}
AFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~CLFNR~V
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

INCLUDES	= -Itbox:,C:
#DFLAGS		= -Ddebugging=1


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= eventlib
COMPONENTZ	= eventlibm
TARGET		= eventlib
TARGETZ		= eventlibm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS		=\
 o.event\
 o.tbevent\
 o.wimpevent\
 o.wimpmsg\
 o.!!version
 
OBJSZ		=\
 z.event\
 z.tbevent\
 z.wimpevent\
 z.wimpmsg\
 o.!!version

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CFLAGS} -o $@ $<
.c.z:;     ${CC} ${CFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${AFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMHGFLAGS} -d $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${COMPONENT} ${COMPONENTZ}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

rom: ${TARGET} ${TARGETZ}
	@echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGETZ}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.event dirs
	${CP}	h.event		${LIBDIR}.tboxlibs.h.event	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${LIBDIR}.tboxlibs.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${LIBDIR}.tboxlibs.o.${TARGETZ}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${LIBDIR}.tboxlibs
	${MKDIR} ${LIBDIR}.tboxlibs.h
	${MKDIR} ${LIBDIR}.tboxlibs.o
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
