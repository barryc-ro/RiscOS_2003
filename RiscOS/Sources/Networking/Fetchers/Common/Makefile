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
CFLAGS    = -Wp

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
o.base64: c.base64 h.base64
o.dns: c.dns TCPIPLibs:sys.h.errno C:h.kernel C:h.swis TCPIPLibs:h.inetlib TCPIPLibs:sys.h.types TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.endian TCPIPLibs:machine.h.ansi TCPIPLibs:machine.h.types TCPIPLibs:netinet.h.in TCPIPLibs:h.socklib TCPIPLibs:sys.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.stat TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.time TCPIPLibs:sys.h.uio TCPIPLibs:sys.h.cdefs C:h.kernel TCPIPLibs:sys.h.types TCPIPLibs:sys.h.socket TCPIPLibs:netinet.h.in TCPIPLibs:h.netdb TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.ioctl TCPIPLibs:sys.h.ttycom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.filio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.sockio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.types TCPIPLibs:sys.h.time TCPIPLibs:sys.h.ioctl TCPIPLibs:arpa.h.inet TCPIPLibs:sys.h.cdefs h.dns
o.generic: c.generic C:h.kernel TCPIPLibs:sys.h.types TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.endian TCPIPLibs:machine.h.ansi TCPIPLibs:machine.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.errno C:h.kernel TCPIPLibs:h.socklib TCPIPLibs:sys.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.stat TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.time TCPIPLibs:sys.h.uio TCPIPLibs:sys.h.cdefs C:h.kernel h.generic
o.URLclient: c.URLclient C:h.kernel C:h.swis h.URLstruct h.URLclient
