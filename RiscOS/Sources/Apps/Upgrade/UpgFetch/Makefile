# Skeleton OBJASM/C Makefile
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created
# 17 Sep 1997   RWB		Customised for SCTest

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o
INSTDIR = <obey$dir>.install
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
CHMOD	= access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -Wa -depend !Depend ${THROWBACK} -ff ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS  =  ${DEBUG}
#
# Libraries
#

CLIB       = CLIB:o.stubs
ANSILIB    = CLIB:o.ansilib
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
EVENTLIB   = Tbox:o.eventlib
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
TCPIPLIB   = <Lib$Dir>.TCPIPLibs.o.*
ZLIB 	   = <Lib$Dir>.zlib.o.zlib
#
# Include files
#
INCLUDES = -Itbox:,C:,TCPIPLibs:

# Program specific options:
#
COMPONENT = UpgFetch
TARGET    = image.upgfetch
APP	  = !${COMPONENT}

OBJS      =	\
o.main		\
o.ui		\
o.kill		\
o.messages   	\


#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cw.o:;	    ${CC}   ${CFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${TARGET}
	@echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#

install: ${TARGET}
	${MKDIR} ${INSTDIR}.${APP}
        ${CP} ${TARGET} ${INSTDIR}.${APP}.${COMPONENT} ${CPFLAGS}
        ${CP} Resources.* ${INSTDIR}.${APP}.* ${CPFLAGS}
        @echo ${COMPONENT}: application installed

clean:
        ${WIPE} o.*       ${WFLAGS}
        ${WIPE} install.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${ZLIB} ${TCPIPLIB}


#
# Dynamic dependencies:

o.testftp: c.testftp C:h.string C:h.swis C:h.kernel C:h.kernel C:interface.h.URL C:h.math
o.fetchimage: c.fetchimage C:h.string C:h.swis C:h.kernel C:h.kernel C:interface.h.URL C:h.math tbox:h.wimplib tbox:h.wimp tbox:h.toolbox h.FetchImage h.kill h.ui h.messages h.globals
o.kill: c.kill C:h.string C:h.swis C:h.kernel C:h.kernel h.kill tbox:h.wimplib tbox:h.wimp tbox:h.wimp h.globals
o.main: c.main C:h.string C:h.swis C:h.kernel C:h.kernel C:interface.h.URL C:h.math tbox:h.wimplib tbox:h.wimp tbox:h.toolbox h.FetchImage h.kill h.ui h.messages h.globals
o.ui: c.ui C:h.kernel C:h.swis tbox:h.wimplib tbox:h.wimp tbox:h.toolbox tbox:h.window tbox:h.gadgets tbox:h.event h.ui h.messages h.globals
o.messages: c.messages C:h.swis C:h.kernel C:h.kernel C:h.string h.messages h.globals
