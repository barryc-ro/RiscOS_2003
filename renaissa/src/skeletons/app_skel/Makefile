# Skeleton OBJASM/C Makefile for application

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

AFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -To $@ -From
CFLAGS  = -c -depend !Depend ${THROWBACK} ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DFLAGS  = ${DEBUG}

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = C:Tboxlibs.o.eventlib
TOOLBOXLIB = C:Tboxlibs.o.toolboxlib
WIMPLIB	   = C:Tboxlibs.o.wimplib
#
# Include files
#
INCLUDES = -IC:Tboxlibs. -IC:


# Program specific options:
#
COMPONENT = App
TARGET    = !RunImage
APP	  = !${COMPONENT}

OBJS      = o.main


#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install:
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	@echo ${COMPONENT}: installed
#
# Target 
#
${TARGET}: ${OBJS}  
        ${LD} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
#
# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	C:Tboxlibs.h.wimp
o.main:	C:Tboxlibs.h.toolbox
o.main:	C:Tboxlibs.h.event
o.main:	C:Tboxlibs.h.wimplib
o.main:	C:Tboxlibs.h.gadgets
o.main:	C:Tboxlibs.h.window
o.main:	C:Tboxlibs.h.menu
o.main:	C:Tboxlibs.h.proginfo
o.main:	C:Tboxlibs.h.iconbar
o.main:	h.constants
o.main:	VersionNum
o.main:	h.resources
