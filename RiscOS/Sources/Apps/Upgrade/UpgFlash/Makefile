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

ABSSYM     = RISC_OSLib:o.AbsSym
ANSILIB    = CLIB:o.ansilib
CLIB       = CLIB:o.stubs
EVENTLIB   = Tbox:o.eventlib
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
RLIB       = RISCOSLIB:o.risc_oslib
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
RSTUBS     = RISCOSLIB:o.rstubs
TCPIPLIB   = <Lib$Dir>.TCPIPLibs.o.*
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
ZLIB 	   = <Lib$Dir>.zlib.o.zlib
#
# Include files
#
INCLUDES = -Itbox:,C:,TCPIPLibs:

# Program specific options:
#
COMPONENT = UpgFlash
TARGET    = image.upgflash
APP	  = !${COMPONENT}

OBJS      =	\
o.main		\
o.flashprog     \
o.fetcher	\
o.stuff         \
o.endscreen	\
o.messages	\


#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
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
        ${LD} -o $@ ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${ZLIB} ${ANSILIB} ${TCPIPLIB}


#
# Dynamic dependencies:


o.BORDERDBUG: s.BORDERDBUG hdr:ListOpts hdr:Macros hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS hdr:Proc hdr:APCS.<APCS> Hdr:APCS.Common
o.ProgOS: c.ProgOS C:h.stdio C:h.stdlib C:h.string C:h.kernel C:h.swis C:zlib.h.zlib C:zlib.h.zconf h.globals h.ProgOS h.flashprog h.endscreen h.fetcher h.messages
o.main: c.main C:h.stdio C:h.stdlib C:h.string C:h.kernel C:h.swis C:zlib.h.zlib C:zlib.h.zconf h.globals h.ProgOS h.flashprog h.endscreen h.fetcher h.messages
o.flashprog: c.flashprog C:zlib.h.zlib C:zlib.h.zconf h.globals h.flashprog
o.fetcher: c.fetcher C:h.kernel C:h.swis h.fetcher
o.stuff: s.stuff Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Debug Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:IO.IOMD Hdr:MEMM.ARM600
o.endscreen: c.endscreen C:h.swis C:h.kernel C:h.kernel h.globals h.endscreen h.messages
o.messages: c.messages C:h.swis C:h.kernel C:h.kernel C:h.string h.globals h.messages
