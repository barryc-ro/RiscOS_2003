# Makefile for MovieFS FrontEnd absolute
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 13-Jul-01      BJGA   Created
#

#
# Generic options:
#

AFLAGS    =
CFLAGS    = -ffa
LDFLAGS   = -aif -o ${TARGET}
SQZFLAGS  = -f -v
MODSQZFLAGS = -f -v
CINCLUDES = -IC:,C:ThirdParty.WSS.WSSLib

include StdTools
include StdRules

#
# Libraries:
#

include AppLibs

BASELIB   = <Lib$Dir>.ThirdParty.WSS.BaseLib.o.BaseLib
WSSLIB    = <Lib$Dir>.ThirdParty.WSS.WSSLib.o.WSSLib

#
# Program specific options:
#

COMPONENT = FrontEnd
TARGET    = !${COMPONENT}
OBJS      = frontend.o
LIBS      = ${CLIB} ${WSSLIB}
DIRS      = local_dirs
RESFSDIR  = ${RESDIR}.^.Apps.!MovieFS

#
# RISC OS build rules:
#

all: ${TARGET}

install: ${TARGET}
        ${MKDIR} ${INSTDIR}
        ${CP} ${TARGET} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: installed

resources: ${TARGET}
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        ${MKDIR} ${RESFSDIR}
        ShrinkWrapZipCompress ${TARGET} ${RESFSDIR}.${TARGET}
        @echo ${COMPONENT}: resource files copied

clean:
        ${RM} ${TARGET}
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Targets:
#

${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} ${TARGET}

local_dirs:
        ${MKDIR} o

#
# Dynamic dependencies:
