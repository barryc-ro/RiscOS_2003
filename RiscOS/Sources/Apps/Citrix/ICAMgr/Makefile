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

#DEBUG   = -g
AFLAGS  = -depend !Depend ${THROWBACK} -Stamp -quit -To $@ -From
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
EVENTLIB   = tbox:o.eventlib
TOOLBOXLIB = tbox:o.toolboxlib
WIMPLIB	   = tbox:o.wimplib
#
# Include files
#
INCLUDES = -IC:Tboxlibs. -IC:


# Program specific options:
#
COMPONENT = ICAMgr
TARGET    = !RunImage
APP	  = !${COMPONENT}

OBJS      = o.Connection \
            o.globals    \
            o.INI        \
            o.Main       \
            o.manager    \
            o.misc

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
        ${WIPE} ${APP} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install:
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.UK.* ${APP}.* ${CPFLAGS}
	@echo ${COMPONENT}: installed
#
# Target 
#
${TARGET}: ${OBJS}  
        ${LD} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
#
# Dynamic dependencies:
