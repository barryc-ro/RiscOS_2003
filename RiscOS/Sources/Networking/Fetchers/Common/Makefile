# Makefile for Network Fetcher common
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 04-May-00  SNB    Created
#

#
# Component specific options:
#

LIBDIR = <Lib$Dir>.Fetchers

TARGET = FetchLibZM
OBJS   = o.base64 o.dns o.generic o.URLclient
DIRS   = o._dirs


CINCLUDES = -ITCPIPLibs: -IC:
CFLAGS    = -Wp -zm -zps1

include Makefiles:StdTools
include Makefiles:StdRules

export: export_${PHASE}
        @|

export_hdrs:
        ${MKDIR} ${LIBDIR}.h
        ${CP} FetchMake ${LIBDIR}.FetchMake ${CPFLAGS}
        ${CP} h.* ${LIBDIR}.h.* ${CPFLAGS}
        ${CP} VersionNum ${LIBDIR}.LibVersion ${CPFLAGS}
        @echo ${COMPONENT}: header export complete

export_libs: ${TARGET} ${DIRS}
        ${MKDIR} ${LIBDIR}.o
        ${CP} ${TARGET} ${LIBDIR}.o.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: library export complete

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS}
        ${AR} ${ARFLAGS} -o $@ ${OBJS}

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

# Dynamic dependencies:
