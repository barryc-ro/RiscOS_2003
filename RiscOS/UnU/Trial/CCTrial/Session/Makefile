# Makefile for Cambridge Trial Phase 2 Session Management Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# ??-???-95  KR		  Original
# 07-Jul-95  RWB          Mangled, hacked and messed with.
#

#
# Paths
#
EXP_HDR = <export$dir>

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -pcc -depend !Depend -zM ${INCLUDES} ${DFLAGS} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DFLAGS  = -Darm -DRISCOS
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

SOCKLIB	   = TCPIPLib:o.socklibzm
UNIXLIB    = TCPIPLib:o.unixlibzm
INETLIB    = TCPIPLib:o.inetlibzm
RPCLIB     = TCPIPLib:o.rpclibzm
RPCDATALIB = TCPIPLib:o.rpc_datazm

#
# Include files
#
INCLUDES = -IC: -ITCPIPLib:

# Program specific options:
#
COMPONENT = Session
TARGET    = rm.Session

OBJS      =	\
  o.header	\
  o.main	\
  o.swi		\
  o.cli		\
  o.Sql_clnt 	\
  o.Sql_xdr 	\
  o.Ssn_clnt 	\
  o.Ssn_xdr 	\
  o.Event_clnt 	\
  o.Event_xdr
#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${RPCLIB}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB} ${RPCLIB} ${RPCDATALIB} ${INETLIB} ${SOCKLIB} ${UNIXLIB} 

#
# Dynamic dependencies:
o.swi:	c.swi
o.swi:	C:h.kernel
o.swi:	C:h.swis
o.swi:	TCPIPLib:rpc.h.rpc
o.swi:	TCPIPLib:rpc.h.types
o.swi:	TCPIPLib:sys.h.types
o.swi:	TCPIPLib:sys.h.time
o.swi:	TCPIPLib:sys.h.time
o.swi:	TCPIPLib:netinet.h.in
o.swi:	TCPIPLib:rpc.h.xdr
o.swi:	TCPIPLib:rpc.h.auth
o.swi:	TCPIPLib:rpc.h.clnt
o.swi:	TCPIPLib:rpc.h.rpc_msg
o.swi:	TCPIPLib:rpc.h.auth_unix
o.swi:	TCPIPLib:rpc.h.svc
o.swi:	TCPIPLib:rpc.h.svc_auth
o.swi:	TCPIPLib:rpc.h.netdb
o.swi:	h.ssn
o.swi:	TCPIPLib:rpc.h.rpc
o.swi:	h.Sql
o.swi:	TCPIPLib:rpc.h.rpc
o.swi:	h.Event
o.swi:	TCPIPLib:rpc.h.rpc
o.cli:	c.cli
o.cli:	C:h.kernel
o.cli:	TCPIPLib:sys.h.types
o.cli:	TCPIPLib:netinet.h.in
o.cli:	h.global
o.cli:	TCPIPLib:sys.h.types
o.cli:	TCPIPLib:netinet.h.in
o.cli:	h.Om_HBT
o.cli:	h.swi
o.cli:	C:h.kernel
o.main:	c.main
o.main:	C:h.errno
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	TCPIPLib:sys.h.types
o.main:	TCPIPLib:sys.h.uio
o.main:	TCPIPLib:sys.h.socket
o.main:	TCPIPLib:sys.h.ioctl
o.main:	TCPIPLib:sys.h.ttychars
o.main:	TCPIPLib:sys.h.ttydev
o.main:	TCPIPLib:arpa.h.inet
o.main:	TCPIPLib:netinet.h.in
o.main:	TCPIPLib:net.h.if
o.main:	TCPIPLib:sys.h.mbuf
o.main:	TCPIPLib:net.h.if_arp
o.main:	TCPIPLib:sys.h.socket
o.main:	TCPIPLib:h.netdb
o.main:	h.Om_HBT
o.main:	h.global
o.main:	TCPIPLib:sys.h.types
o.main:	TCPIPLib:netinet.h.in
o.main:	h.Om_HBT
o.main:	h.swi
o.main:	C:h.kernel
o.main:	h.cli
