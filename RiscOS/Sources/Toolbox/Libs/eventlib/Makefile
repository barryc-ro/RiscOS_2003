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

INCLUDES	= -I<Lib$Dir>.tboxlibs.,<Lib$Dir>.tboxlibint.,C:
#DFLAGS		= -Ddebugging=1


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= eventlib
COMPONENTZ	= eventlibm
TARGET		= eventlib
TARGETZ		= eventlibm

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

export: export_${PHASE}

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


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.event:	c.event
o.event:	C:h.kernel
o.event:	h.debug
o.event:	h.types
o.event:	<Lib$Dir>.tboxlibs.h.wimp
o.event:	<Lib$Dir>.tboxlibs.h.toolbox
o.event:	h.event
o.event:	h.event
o.event:	h.wimpevent
o.event:	h.tbevent
o.event:	h.wimpmsg
o.tbevent:	c.tbevent
o.tbevent:	C:h.kernel
o.tbevent:	h.event
o.tbevent:	<Lib$Dir>.tboxlibs.h.wimp
o.tbevent:	<Lib$Dir>.tboxlibs.h.toolbox
o.tbevent:	h.debug
o.tbevent:	h.types
o.tbevent:	h.tbevent
o.wimpevent:	c.wimpevent
o.wimpevent:	C:h.kernel
o.wimpevent:	h.debug
o.wimpevent:	h.event
o.wimpevent:	<Lib$Dir>.tboxlibs.h.wimp
o.wimpevent:	<Lib$Dir>.tboxlibs.h.toolbox
o.wimpevent:	h.types
o.wimpevent:	h.wimpevent
o.wimpmsg:	c.wimpmsg
o.wimpmsg:	C:h.kernel
o.wimpmsg:	h.debug
o.wimpmsg:	h.event
o.wimpmsg:	<Lib$Dir>.tboxlibs.h.wimp
o.wimpmsg:	<Lib$Dir>.tboxlibs.h.toolbox
o.wimpmsg:	h.types
o.wimpmsg:	h.wimpmsg
z.event:	c.event
z.event:	C:h.kernel
z.event:	h.debug
z.event:	h.types
z.event:	<Lib$Dir>.tboxlibs.h.wimp
z.event:	<Lib$Dir>.tboxlibs.h.toolbox
z.event:	h.event
z.event:	h.event
z.event:	h.wimpevent
z.event:	h.tbevent
z.event:	h.wimpmsg
z.tbevent:	c.tbevent
z.tbevent:	C:h.kernel
z.tbevent:	h.event
z.tbevent:	<Lib$Dir>.tboxlibs.h.wimp
z.tbevent:	<Lib$Dir>.tboxlibs.h.toolbox
z.tbevent:	h.debug
z.tbevent:	h.types
z.tbevent:	h.tbevent
z.wimpevent:	c.wimpevent
z.wimpevent:	C:h.kernel
z.wimpevent:	h.debug
z.wimpevent:	h.event
z.wimpevent:	<Lib$Dir>.tboxlibs.h.wimp
z.wimpevent:	<Lib$Dir>.tboxlibs.h.toolbox
z.wimpevent:	h.types
z.wimpevent:	h.wimpevent
z.wimpmsg:	c.wimpmsg
z.wimpmsg:	C:h.kernel
z.wimpmsg:	h.debug
z.wimpmsg:	h.event
z.wimpmsg:	<Lib$Dir>.tboxlibs.h.wimp
z.wimpmsg:	<Lib$Dir>.tboxlibs.h.toolbox
z.wimpmsg:	h.types
z.wimpmsg:	h.wimpmsg
o.!!version: s.!!version
