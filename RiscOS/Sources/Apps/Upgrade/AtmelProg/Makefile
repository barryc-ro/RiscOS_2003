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
INSTDIR = $
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
STRIP   = -stripdepnd

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
COMPONENT = AtmelProg
TARGET    = aof.atmelprog
APP	  = !${COMPONENT}

OBJS      =	\
o.ProgOS	\
o.flashprog     \
o.fetcher	\
o.stuff         \



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
        ${CP} ${TARGET} ${INSTDIR}.${APP}.${TARGET} ${CPFLAGS}
        ${CP} Resources.* ${INSTDIR}.${APP}.* ${CPFLAGS}
        @echo ${COMPONENT}: application installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} o
        ${RM} aof
        ${STRIP}
        @echo ${COMPONENT}: cleaned

dirs:
        ${MKDIR} o
        ${MKDIR} aof

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} dirs
        ${LD} -o $@ ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${ZLIB} ${ANSILIB} ${TCPIPLIB}


#
# Dynamic dependencies:
