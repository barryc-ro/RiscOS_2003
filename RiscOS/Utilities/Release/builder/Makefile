# Makefile for BuildApp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11 Sep 1997   RWB 	  New for BuildApp
# 12 Sep 1997	RWB	  Added install target
#

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
CHMOD	= access

AFLAGS = -depend !Depend -Stamp -quit -To $@ -From
CFLAGS  = -c -depend !Depend ${THROWBACK} ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CHMODFLAGS = RW/R
#DFLAGS  = -DDEBUG

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = Tbox:o.eventlib
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
#
# Include files
#
INCLUDES = -ITbox: -IC:


# Program specific options:
#
COMPONENT = Builder
TARGET    = !RunImage
APP	  = ${INSTALL_DIR}.!${COMPONENT}

OBJS      = o.main o.db o.msg o.env o.file o.parse o.build


#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install: ${TARGET}
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${CHMOD} ${APP} ${CHMODFLAGS}
	${CHMOD} ${APP}.* ${CHMODFLAGS}
	@echo ${COMPONENT}: installed
#
# Target 
#
${TARGET}: ${OBJS}  
        ${LD} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
#
# Dynamic dependencies:o.main:	c.main
o.db:	c.db
o.msg:	c.msg
o.msg:	C:h.kernel
o.msg:	C:h.swis
o.msg:	h.constants
o.msg:	VersionNum
o.msg:	h.db
o.msg:	h.constants
o.env:	c.env
o.env:	C:h.kernel
o.env:	C:h.swis
o.env:	Tbox:h.gadgets
o.env:	Tbox:h.toolbox
o.env:	Tbox:h.wimp
o.env:	h.constants
o.env:	VersionNum
o.env:	h.db
o.env:	h.constants
o.file:	c.file
o.file:	C:h.kernel
o.file:	C:h.swis
o.parse:	c.parse
o.parse:	h.constants
o.parse:	VersionNum
o.parse:	h.db
o.parse:	h.constants
o.build:	c.build
o.build:	C:h.kernel
o.build:	Tbox:h.gadgets
o.build:	Tbox:h.toolbox
o.build:	Tbox:h.wimp
o.build:	h.constants
o.build:	VersionNum
o.build:	h.db
o.build:	h.constants
o.build:	h.parse
o.build:	h.file
o.build:	h.msg
o.build:	C:h.kernel
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	Tbox:h.wimp
o.main:	Tbox:h.toolbox
o.main:	Tbox:h.event
o.main:	Tbox:h.wimplib
o.main:	Tbox:h.gadgets
o.main:	Tbox:h.window
o.main:	Tbox:h.menu
o.main:	Tbox:h.proginfo
o.main:	Tbox:h.iconbar
o.main:	h.resources
o.main:	h.constants
o.main:	VersionNum
o.main:	h.db
o.main:	h.constants
o.main:	h.msg
o.main:	C:h.kernel
o.main:	h.env
o.main:	C:h.kernel
o.main:	h.file
o.main:	h.parse
o.main:	h.build
