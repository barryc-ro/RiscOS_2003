# Makefile for debuglib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-10-07	BAL		Created


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
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

INCLUDES	= -Itbox:,C:
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= debuglib
TARGET		= debuglib
TARGETZ		= debuglibzm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS		= o.debug o.inits o.quits o.output

OBJSZ		= z.debug z.inits z.quits z.output

LIBS		= libs.pd_pl libs.svcprint libs.t_m_pl libs.t_pl libs.remote

LIBSZ		= ${LIBS}

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .z .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zm -zps1 -o $@ $<
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
	${WIPE} z ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.debuglib dirs
	${CP}	h.debuglib	${EXPORT_DIR}.h.debuglib ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o
	${MKDIR} ${EXPORT_DIR}.z
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS} ${LIBS}
        
${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ} ${LIBSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.debug:	c.debug
o.debug:	h.include
o.debug:	tbox:h.toolbox
o.debug:	C:h.kernel
o.debug:	tbox:h.wimp
o.debug:	tbox:h.wimp
o.debug:	tbox:h.wimplib
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.debug:	h.svcprint
o.debug:	h.PDebug
o.debug:	h.Trace
o.debug:	h.remote
o.debug:	h.debug
o.debug:	h.inits
o.debug:	h.output
o.debug:	h.quits
o.debug:	h.globals
o.inits:	c.inits
o.inits:	h.include
o.inits:	tbox:h.toolbox
o.inits:	C:h.kernel
o.inits:	tbox:h.wimp
o.inits:	tbox:h.wimp
o.inits:	tbox:h.wimplib
o.inits:	C:h.kernel
o.inits:	C:h.swis
o.inits:	h.svcprint
o.inits:	h.PDebug
o.inits:	h.Trace
o.inits:	h.remote
o.inits:	h.output
o.inits:	h.inits
o.inits:	h.globals
o.quits:	c.quits
o.quits:	h.include
o.quits:	tbox:h.toolbox
o.quits:	C:h.kernel
o.quits:	tbox:h.wimp
o.quits:	tbox:h.wimp
o.quits:	tbox:h.wimplib
o.quits:	C:h.kernel
o.quits:	C:h.swis
o.quits:	h.svcprint
o.quits:	h.PDebug
o.quits:	h.Trace
o.quits:	h.remote
o.quits:	h.quits
o.quits:	h.globals
o.output:	c.output
o.output:	h.include
o.output:	tbox:h.toolbox
o.output:	C:h.kernel
o.output:	tbox:h.wimp
o.output:	tbox:h.wimp
o.output:	tbox:h.wimplib
o.output:	C:h.kernel
o.output:	C:h.swis
o.output:	h.svcprint
o.output:	h.PDebug
o.output:	h.Trace
o.output:	h.remote
o.output:	h.output
o.output:	h.debug
o.output:	h.globals
z.debug:	c.debug
z.debug:	h.include
z.debug:	tbox:h.toolbox
z.debug:	C:h.kernel
z.debug:	tbox:h.wimp
z.debug:	tbox:h.wimp
z.debug:	tbox:h.wimplib
z.debug:	C:h.kernel
z.debug:	C:h.swis
z.debug:	h.svcprint
z.debug:	h.PDebug
z.debug:	h.Trace
z.debug:	h.remote
z.debug:	h.debug
z.debug:	h.inits
z.debug:	h.output
z.debug:	h.quits
z.debug:	h.globals
z.inits:	c.inits
z.inits:	h.include
z.inits:	tbox:h.toolbox
z.inits:	C:h.kernel
z.inits:	tbox:h.wimp
z.inits:	tbox:h.wimp
z.inits:	tbox:h.wimplib
z.inits:	C:h.kernel
z.inits:	C:h.swis
z.inits:	h.svcprint
z.inits:	h.PDebug
z.inits:	h.Trace
z.inits:	h.remote
z.inits:	h.output
z.inits:	h.inits
z.inits:	h.globals
z.quits:	c.quits
z.quits:	h.include
z.quits:	tbox:h.toolbox
z.quits:	C:h.kernel
z.quits:	tbox:h.wimp
z.quits:	tbox:h.wimp
z.quits:	tbox:h.wimplib
z.quits:	C:h.kernel
z.quits:	C:h.swis
z.quits:	h.svcprint
z.quits:	h.PDebug
z.quits:	h.Trace
z.quits:	h.remote
z.quits:	h.quits
z.quits:	h.globals
z.output:	c.output
z.output:	h.include
z.output:	tbox:h.toolbox
z.output:	C:h.kernel
z.output:	tbox:h.wimp
z.output:	tbox:h.wimp
z.output:	tbox:h.wimplib
z.output:	C:h.kernel
z.output:	C:h.swis
z.output:	h.svcprint
z.output:	h.PDebug
z.output:	h.Trace
z.output:	h.remote
z.output:	h.output
z.output:	h.debug
z.output:	h.globals
