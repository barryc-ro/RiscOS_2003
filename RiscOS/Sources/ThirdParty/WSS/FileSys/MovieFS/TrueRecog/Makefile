# Makefile for MovieFS MFSUniv module
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
CFLAGS    = -ffa -zM ${OPTIONS}
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

COMPONENT = MFSUniv
TARGET    = ${COMPONENT}
OBJS      = aiff.o animator.o atom.o au.o authority.o avi.o cveneer.o \
            decode.o dl.o error.o fileio.o flc.o header.o hub.o image.o \
            main.o memory.o nosup.o part.o prot1.o prot2.o qt.o recognise.o \
            replay.o riff.o sprite.o tracks.o unit.o var.o vidqt.o voc.o \
            wav.o word.o wss.o wve.o
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
