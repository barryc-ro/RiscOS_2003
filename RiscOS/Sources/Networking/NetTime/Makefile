# Makefile for NetTime
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 17-Jun-96	JRH	Created
# 24-Nov-98	JMF	Switch to DebugLib for debugging
# 12-Jan-99	JMF	Add support for EUROPEAN_DST_BODGE
# 09-Jun-99     SBF	Added support for StripDepnd tool

COMPONENT .= NetTime

#
# Generic options:
#
CINCLUDES = -IC: -IC:TCPIPLibs
CDEFINES  = -DCOMPAT_INET4
CFLAGS    = -ffa -zM -zps1 -Wp

COMOBJS   = extralong.o mul64.o machine.o nt_nvram.o dns.o \
            edst.o ntp.o rtcadjust.o dhcp.o dst.o adst.o

ROM_OBJS  = ${COMOBJS} header.o module.o
RAM_OBJS  = ${COMOBJS} headerSA.o moduleSA.o

include Makefiles:ModuleLibs
LIBS      = ${NET4LIBS}

include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:ROMCModule
include Makefiles:RAMCModule

#
# Generic rules:
#
clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} h.header
        @echo ${COMPONENT}: cleaned

resources:
        TokenCheck LocalRes:Messages
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

machine.o module.o: header.h
o.headerSA: cmhg.header; ${CMHG} ${CMHGFLAGS} -DSTANDALONE -o $@ cmhg.header
moduleSA.o: header.h module.c; ${CC} ${CFLAGS} -DSTANDALONE -o $@ module.c

#
# Dynamic dependencies:
