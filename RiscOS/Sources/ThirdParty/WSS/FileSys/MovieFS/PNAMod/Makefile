# Makefile for MovieFS PNA module
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
CINCLUDES = -IC:,C:ThirdParty.WSS.BaseLib,TCPIPLibs:

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

COMPONENT = PNA
TARGET    = ${COMPONENT}
OBJS      = authkey.o callback.o caller.o challenge.o chpres.o chunk.o \
            data.o error.o fileio.o filepnach.o forcecallb.o gethost.o \
            gethost_.o header.o http.o httpchunk.o httppnach.o interleave.o \
            interlsl.o main.o md5c.o md5hl.o memory.o netpnach.o \
            nocasecmp.o packetread.o pna.o pnach.o pqueue.o queue.o ra.o \
            rmheader.o safeputc.o sysvar.o tcpbsd.o time.o url.o
LIBS      = ${BASELIB} ${INETLIB} ${SOCK4LIB} ${UNIXLIB}
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
        -${MKDIR} ${INSTDIR}

#
# Dynamic dependencies:
