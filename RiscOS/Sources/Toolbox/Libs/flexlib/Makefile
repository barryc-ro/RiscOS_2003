# Makefile for flexlib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-07-18	BAL		Created
# 1997-09-03	BAL		Tidied up for committal
# 1997-09-23	BAL		Fixed broken export for module build
# 				Removed rogue reference to TARGETZM in
#                               install_rom rule
#


# ------------------------------------------------------------------------------
# Paths
#

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


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= flexlib
COMPONENTZ	= flexlibzm
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENTZ}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS		= o.flex
 
OBJSZ		= z.flex


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${COMPONENT} ${COMPONENTZ}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.flex dirs
	${CP}	h.flex		${LIBDIR}.tboxlibs.h.flex	${CPFLAGS}
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


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.flex:	c.flex
o.flex:	C:h.kernel
o.flex:	C:h.swis
o.flex:	h.opts
o.flex:	h.flex
o.flex:	h.swiextra
z.flex:	c.flex
z.flex:	C:h.kernel
z.flex:	C:h.swis
z.flex:	h.opts
z.flex:	h.flex
z.flex:	h.swiextra
