# Makefile for MovieFS KickStart module
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
CFLAGS    = -ffa -zM
SQZFLAGS  = -f -v
MODSQZFLAGS = -f -v
CINCLUDES = -IC:,C:ThirdParty.WSS.BaseLib

include StdTools
include ModStdRule

#
# Libraries:
#

include ModuleLibs

BASELIB   = <Lib$Dir>.ThirdParty.WSS.BaseLib.o.BaseLibZM
WSSLIB    = <Lib$Dir>.ThirdParty.WSS.WSSLib.o.WSSLibZM

#
# Program specific options:
#

COMPONENT = KickStart
TARGET    = ${COMPONENT}
OBJS      = client.o cveneer.o filter.o filtercode.o header.o recog.o
LIBS      = ${BASELIB}
DIRS      = local_dirs
RESFSDIR  = ${RESDIR}.^.Apps.!MovieFS

include RAMCModule

#
# RISC OS build rules:
#

resources: ${RAM_MODULE}
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        ${MKDIR} ${RESFSDIR}
        ShrinkWrapZipCompress ${RAM_MODULE} ${RESFSDIR}.${COMPONENT}
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Targets:
#

local_dirs:
        ${MKDIR} o

#
# Dynamic dependencies:
