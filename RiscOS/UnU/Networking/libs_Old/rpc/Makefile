# Makefile for RPClib module linkable version
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 05-May-95  RWB          Mangled.
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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -pcc -depend !Depend ${INCLUDES} -zM -Darm -DRISCOS
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC: -I^ -I^.^.AUN.include

#
# Program specific options:
#
COMPONENT = rpclibzm
TARGET    = rpclibzm
OBJS      =	\
  o.auth_none   \
  o.auth_unix   \
  o.authunix_p  \
  o.bindresvpo  \
  o.clnt_gener  \
  o.clnt_perro  \
  o.clnt_raw    \
  o.clnt_simpl  \
  o.clnt_udp    \
  o.rpc_dtable  \
  o.get_myaddr  \
  o.getrpcent   \
  o.getrpcport  \
  o.pmap_clnt   \
  o.pmap_getma  \
  o.pmap_getpo  \
  o.pmap_prot   \
  o.pmap_prot2  \
  o.pmap_rmt    \
  o.rpc_prot    \
  o.rpc_common  \
  o.rpc_callms  \
  o.xdr         \
  o.xdr_array   \
  o.xdr_mem     \
  o.xdr_rec     \
  o.xdr_refere  \
  o.xdr_stdio   \
  o.callback

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a linkable rpclib
#
all: ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS}
     libfile -c rpclibzm 30 20
     libfile -i rpclibzm o.*
     libfile -d rpclibzm o.rpc_common
     libfile -o -l rpclibzm


# Dynamic dependencies:
o.auth_none:	c.auth_none
o.auth_none:	^.rpc.h.types
o.auth_none:	^.^.AUN.include.sys.h.types
o.auth_none:	^.^.AUN.include.sys.h.time
o.auth_none:	^.^.AUN.include.sys.h.time
o.auth_none:	^.rpc.h.xdr
o.auth_none:	^.rpc.h.auth
o.auth_unix:	c.auth_unix
o.auth_unix:	^.rpc.h.types
o.auth_unix:	^.^.AUN.include.sys.h.types
o.auth_unix:	^.^.AUN.include.sys.h.time
o.auth_unix:	^.^.AUN.include.sys.h.time
o.auth_unix:	^.rpc.h.xdr
o.auth_unix:	^.rpc.h.auth
o.auth_unix:	^.rpc.h.auth_unix
o.authunix_p:	c.authunix_p
o.authunix_p:	^.rpc.h.types
o.authunix_p:	^.^.AUN.include.sys.h.types
o.authunix_p:	^.^.AUN.include.sys.h.time
o.authunix_p:	^.^.AUN.include.sys.h.time
o.authunix_p:	^.rpc.h.xdr
o.authunix_p:	^.rpc.h.auth
o.authunix_p:	^.rpc.h.auth_unix
o.bindresvpo:	c.bindresvpo
o.bindresvpo:	^.^.AUN.include.sys.h.types
o.bindresvpo:	^.^.AUN.include.sys.h.errno
o.bindresvpo:	^.^.AUN.include.sys.^.h.errno
o.bindresvpo:	C:h.kernel
o.bindresvpo:	^.^.AUN.include.sys.h.socket
o.bindresvpo:	^.^.AUN.include.netinet.h.in
o.clnt_gener:	c.clnt_gener
o.clnt_gener:	^.rpc.h.rpc
o.clnt_gener:	^.rpc.h.types
o.clnt_gener:	^.^.AUN.include.sys.h.types
o.clnt_gener:	^.^.AUN.include.sys.h.time
o.clnt_gener:	^.^.AUN.include.sys.h.time
o.clnt_gener:	^.^.AUN.include.netinet.h.in
o.clnt_gener:	^.rpc.h.xdr
o.clnt_gener:	^.rpc.h.auth
o.clnt_gener:	^.rpc.h.clnt
o.clnt_gener:	^.rpc.h.rpc_msg
o.clnt_gener:	^.rpc.h.auth_unix
o.clnt_gener:	^.rpc.h.svc
o.clnt_gener:	^.rpc.h.svc_auth
o.clnt_gener:	^.rpc.h.netdb
o.clnt_gener:	^.^.AUN.include.sys.h.socket
o.clnt_gener:	^.^.AUN.include.sys.h.errno
o.clnt_gener:	^.^.AUN.include.sys.^.h.errno
o.clnt_gener:	C:h.kernel
o.clnt_gener:	^.^.AUN.include.h.netdb
o.clnt_perro:	c.clnt_perro
o.clnt_perro:	^.rpc.h.types
o.clnt_perro:	^.^.AUN.include.sys.h.types
o.clnt_perro:	^.^.AUN.include.sys.h.time
o.clnt_perro:	^.^.AUN.include.sys.h.time
o.clnt_perro:	^.rpc.h.auth
o.clnt_perro:	^.rpc.h.clnt
o.clnt_raw:	c.clnt_raw
o.clnt_raw:	^.rpc.h.rpc
o.clnt_raw:	^.rpc.h.types
o.clnt_raw:	^.^.AUN.include.sys.h.types
o.clnt_raw:	^.^.AUN.include.sys.h.time
o.clnt_raw:	^.^.AUN.include.sys.h.time
o.clnt_raw:	^.^.AUN.include.netinet.h.in
o.clnt_raw:	^.rpc.h.xdr
o.clnt_raw:	^.rpc.h.auth
o.clnt_raw:	^.rpc.h.clnt
o.clnt_raw:	^.rpc.h.rpc_msg
o.clnt_raw:	^.rpc.h.auth_unix
o.clnt_raw:	^.rpc.h.svc
o.clnt_raw:	^.rpc.h.svc_auth
o.clnt_raw:	^.rpc.h.netdb
o.clnt_simpl:	c.clnt_simpl
o.clnt_simpl:	^.rpc.h.rpc
o.clnt_simpl:	^.rpc.h.types
o.clnt_simpl:	^.^.AUN.include.sys.h.types
o.clnt_simpl:	^.^.AUN.include.sys.h.time
o.clnt_simpl:	^.^.AUN.include.sys.h.time
o.clnt_simpl:	^.^.AUN.include.netinet.h.in
o.clnt_simpl:	^.rpc.h.xdr
o.clnt_simpl:	^.rpc.h.auth
o.clnt_simpl:	^.rpc.h.clnt
o.clnt_simpl:	^.rpc.h.rpc_msg
o.clnt_simpl:	^.rpc.h.auth_unix
o.clnt_simpl:	^.rpc.h.svc
o.clnt_simpl:	^.rpc.h.svc_auth
o.clnt_simpl:	^.rpc.h.netdb
o.clnt_simpl:	^.^.AUN.include.sys.h.socket
o.clnt_simpl:	^.^.AUN.include.h.netdb
o.clnt_udp:	c.clnt_udp
o.clnt_udp:	^.rpc.h.rpc
o.clnt_udp:	^.rpc.h.types
o.clnt_udp:	^.^.AUN.include.sys.h.types
o.clnt_udp:	^.^.AUN.include.sys.h.time
o.clnt_udp:	^.^.AUN.include.sys.h.time
o.clnt_udp:	^.^.AUN.include.netinet.h.in
o.clnt_udp:	^.rpc.h.xdr
o.clnt_udp:	^.rpc.h.auth
o.clnt_udp:	^.rpc.h.clnt
o.clnt_udp:	^.rpc.h.rpc_msg
o.clnt_udp:	^.rpc.h.auth_unix
o.clnt_udp:	^.rpc.h.svc
o.clnt_udp:	^.rpc.h.svc_auth
o.clnt_udp:	^.rpc.h.netdb
o.clnt_udp:	^.^.AUN.include.sys.h.socket
o.clnt_udp:	^.^.AUN.include.sys.h.ioctl
o.clnt_udp:	^.^.AUN.include.sys.h.ttychars
o.clnt_udp:	^.^.AUN.include.sys.h.ttydev
o.clnt_udp:	^.^.AUN.include.h.netdb
o.clnt_udp:	^.^.AUN.include.sys.h.errno
o.clnt_udp:	^.^.AUN.include.sys.^.h.errno
o.clnt_udp:	C:h.kernel
o.clnt_udp:	^.rpc.h.pmap_clnt
o.rpc_dtable:	c.rpc_dtable
o.get_myaddr:	c.get_myaddr
o.get_myaddr:	^.rpc.h.types
o.get_myaddr:	^.^.AUN.include.sys.h.types
o.get_myaddr:	^.^.AUN.include.sys.h.time
o.get_myaddr:	^.^.AUN.include.sys.h.time
o.get_myaddr:	^.rpc.h.pmap_prot
o.get_myaddr:	^.^.AUN.include.sys.h.socket
o.get_myaddr:	^.^.AUN.include.net.h.if
o.get_myaddr:	^.^.AUN.include.sys.h.mbuf
o.get_myaddr:	^.^.AUN.include.net.h.if_arp
o.get_myaddr:	^.^.AUN.include.sys.h.socket
o.get_myaddr:	^.^.AUN.include.sys.h.ioctl
o.get_myaddr:	^.^.AUN.include.sys.h.ttychars
o.get_myaddr:	^.^.AUN.include.sys.h.ttydev
o.get_myaddr:	^.^.AUN.include.arpa.h.inet
o.get_myaddr:	^.^.AUN.include.netinet.h.in
o.getrpcent:	c.getrpcent
o.getrpcent:	^.^.AUN.include.sys.h.types
o.getrpcent:	^.rpc.h.rpc
o.getrpcent:	^.rpc.h.types
o.getrpcent:	^.^.AUN.include.sys.h.time
o.getrpcent:	^.^.AUN.include.sys.h.time
o.getrpcent:	^.^.AUN.include.netinet.h.in
o.getrpcent:	^.rpc.h.xdr
o.getrpcent:	^.rpc.h.auth
o.getrpcent:	^.rpc.h.clnt
o.getrpcent:	^.rpc.h.rpc_msg
o.getrpcent:	^.rpc.h.auth_unix
o.getrpcent:	^.rpc.h.svc
o.getrpcent:	^.rpc.h.svc_auth
o.getrpcent:	^.rpc.h.netdb
o.getrpcent:	^.^.AUN.include.h.netdb
o.getrpcent:	^.^.AUN.include.sys.h.socket
o.getrpcport:	c.getrpcport
o.getrpcport:	^.rpc.h.rpc
o.getrpcport:	^.rpc.h.types
o.getrpcport:	^.^.AUN.include.sys.h.types
o.getrpcport:	^.^.AUN.include.sys.h.time
o.getrpcport:	^.^.AUN.include.sys.h.time
o.getrpcport:	^.^.AUN.include.netinet.h.in
o.getrpcport:	^.rpc.h.xdr
o.getrpcport:	^.rpc.h.auth
o.getrpcport:	^.rpc.h.clnt
o.getrpcport:	^.rpc.h.rpc_msg
o.getrpcport:	^.rpc.h.auth_unix
o.getrpcport:	^.rpc.h.svc
o.getrpcport:	^.rpc.h.svc_auth
o.getrpcport:	^.rpc.h.netdb
o.getrpcport:	^.^.AUN.include.h.netdb
o.getrpcport:	^.^.AUN.include.sys.h.socket
o.pmap_clnt:	c.pmap_clnt
o.pmap_clnt:	^.rpc.h.rpc
o.pmap_clnt:	^.rpc.h.types
o.pmap_clnt:	^.^.AUN.include.sys.h.types
o.pmap_clnt:	^.^.AUN.include.sys.h.time
o.pmap_clnt:	^.^.AUN.include.sys.h.time
o.pmap_clnt:	^.^.AUN.include.netinet.h.in
o.pmap_clnt:	^.rpc.h.xdr
o.pmap_clnt:	^.rpc.h.auth
o.pmap_clnt:	^.rpc.h.clnt
o.pmap_clnt:	^.rpc.h.rpc_msg
o.pmap_clnt:	^.rpc.h.auth_unix
o.pmap_clnt:	^.rpc.h.svc
o.pmap_clnt:	^.rpc.h.svc_auth
o.pmap_clnt:	^.rpc.h.netdb
o.pmap_clnt:	^.rpc.h.pmap_prot
o.pmap_clnt:	^.rpc.h.pmap_clnt
o.pmap_getma:	c.pmap_getma
o.pmap_getma:	^.rpc.h.rpc
o.pmap_getma:	^.rpc.h.types
o.pmap_getma:	^.^.AUN.include.sys.h.types
o.pmap_getma:	^.^.AUN.include.sys.h.time
o.pmap_getma:	^.^.AUN.include.sys.h.time
o.pmap_getma:	^.^.AUN.include.netinet.h.in
o.pmap_getma:	^.rpc.h.xdr
o.pmap_getma:	^.rpc.h.auth
o.pmap_getma:	^.rpc.h.clnt
o.pmap_getma:	^.rpc.h.rpc_msg
o.pmap_getma:	^.rpc.h.auth_unix
o.pmap_getma:	^.rpc.h.svc
o.pmap_getma:	^.rpc.h.svc_auth
o.pmap_getma:	^.rpc.h.netdb
o.pmap_getma:	^.rpc.h.pmap_prot
o.pmap_getma:	^.rpc.h.pmap_clnt
o.pmap_getma:	^.^.AUN.include.sys.h.socket
o.pmap_getma:	^.^.AUN.include.h.netdb
o.pmap_getma:	^.^.AUN.include.sys.h.errno
o.pmap_getma:	^.^.AUN.include.sys.^.h.errno
o.pmap_getma:	C:h.kernel
o.pmap_getma:	^.^.AUN.include.net.h.if
o.pmap_getma:	^.^.AUN.include.sys.h.mbuf
o.pmap_getma:	^.^.AUN.include.net.h.if_arp
o.pmap_getma:	^.^.AUN.include.sys.h.socket
o.pmap_getma:	^.^.AUN.include.sys.h.ioctl
o.pmap_getma:	^.^.AUN.include.sys.h.ttychars
o.pmap_getma:	^.^.AUN.include.sys.h.ttydev
o.pmap_getpo:	c.pmap_getpo
o.pmap_getpo:	^.rpc.h.rpc
o.pmap_getpo:	^.rpc.h.types
o.pmap_getpo:	^.^.AUN.include.sys.h.types
o.pmap_getpo:	^.^.AUN.include.sys.h.time
o.pmap_getpo:	^.^.AUN.include.sys.h.time
o.pmap_getpo:	^.^.AUN.include.netinet.h.in
o.pmap_getpo:	^.rpc.h.xdr
o.pmap_getpo:	^.rpc.h.auth
o.pmap_getpo:	^.rpc.h.clnt
o.pmap_getpo:	^.rpc.h.rpc_msg
o.pmap_getpo:	^.rpc.h.auth_unix
o.pmap_getpo:	^.rpc.h.svc
o.pmap_getpo:	^.rpc.h.svc_auth
o.pmap_getpo:	^.rpc.h.netdb
o.pmap_getpo:	^.rpc.h.pmap_prot
o.pmap_getpo:	^.rpc.h.pmap_clnt
o.pmap_getpo:	^.^.AUN.include.sys.h.socket
o.pmap_getpo:	^.^.AUN.include.net.h.if
o.pmap_getpo:	^.^.AUN.include.sys.h.mbuf
o.pmap_getpo:	^.^.AUN.include.net.h.if_arp
o.pmap_getpo:	^.^.AUN.include.sys.h.socket
o.pmap_prot:	c.pmap_prot
o.pmap_prot:	^.rpc.h.types
o.pmap_prot:	^.^.AUN.include.sys.h.types
o.pmap_prot:	^.^.AUN.include.sys.h.time
o.pmap_prot:	^.^.AUN.include.sys.h.time
o.pmap_prot:	^.rpc.h.xdr
o.pmap_prot:	^.rpc.h.pmap_prot
o.pmap_prot2:	c.pmap_prot2
o.pmap_prot2:	^.rpc.h.types
o.pmap_prot2:	^.^.AUN.include.sys.h.types
o.pmap_prot2:	^.^.AUN.include.sys.h.time
o.pmap_prot2:	^.^.AUN.include.sys.h.time
o.pmap_prot2:	^.rpc.h.xdr
o.pmap_prot2:	^.rpc.h.pmap_prot
o.pmap_rmt:	c.pmap_rmt
o.pmap_rmt:	^.rpc.h.rpc
o.pmap_rmt:	^.rpc.h.types
o.pmap_rmt:	^.^.AUN.include.sys.h.types
o.pmap_rmt:	^.^.AUN.include.sys.h.time
o.pmap_rmt:	^.^.AUN.include.sys.h.time
o.pmap_rmt:	^.^.AUN.include.netinet.h.in
o.pmap_rmt:	^.rpc.h.xdr
o.pmap_rmt:	^.rpc.h.auth
o.pmap_rmt:	^.rpc.h.clnt
o.pmap_rmt:	^.rpc.h.rpc_msg
o.pmap_rmt:	^.rpc.h.auth_unix
o.pmap_rmt:	^.rpc.h.svc
o.pmap_rmt:	^.rpc.h.svc_auth
o.pmap_rmt:	^.rpc.h.netdb
o.pmap_rmt:	^.rpc.h.pmap_prot
o.pmap_rmt:	^.rpc.h.pmap_clnt
o.pmap_rmt:	^.rpc.h.pmap_rmt
o.pmap_rmt:	^.^.AUN.include.sys.h.socket
o.pmap_rmt:	^.^.AUN.include.sys.h.errno
o.pmap_rmt:	^.^.AUN.include.sys.^.h.errno
o.pmap_rmt:	C:h.kernel
o.pmap_rmt:	^.^.AUN.include.net.h.if
o.pmap_rmt:	^.^.AUN.include.sys.h.mbuf
o.pmap_rmt:	^.^.AUN.include.net.h.if_arp
o.pmap_rmt:	^.^.AUN.include.sys.h.socket
o.pmap_rmt:	^.^.AUN.include.sys.h.ioctl
o.pmap_rmt:	^.^.AUN.include.sys.h.ttychars
o.pmap_rmt:	^.^.AUN.include.sys.h.ttydev
o.pmap_rmt:	^.^.AUN.include.arpa.h.inet
o.rpc_prot:	c.rpc_prot
o.rpc_prot:	^.^.AUN.include.sys.h.param
o.rpc_prot:	^.^.AUN.include.sys.h.sysconf
o.rpc_prot:	^.^.AUN.include.machine.h.machparam
o.rpc_prot:	^.^.AUN.include.sys.h.signal
o.rpc_prot:	^.^.AUN.include.sys.h.types
o.rpc_prot:	^.rpc.h.rpc
o.rpc_prot:	^.rpc.h.types
o.rpc_prot:	^.^.AUN.include.sys.h.time
o.rpc_prot:	^.^.AUN.include.sys.h.time
o.rpc_prot:	^.^.AUN.include.netinet.h.in
o.rpc_prot:	^.rpc.h.xdr
o.rpc_prot:	^.rpc.h.auth
o.rpc_prot:	^.rpc.h.clnt
o.rpc_prot:	^.rpc.h.rpc_msg
o.rpc_prot:	^.rpc.h.auth_unix
o.rpc_prot:	^.rpc.h.svc
o.rpc_prot:	^.rpc.h.svc_auth
o.rpc_prot:	^.rpc.h.netdb
o.rpc_callms:	c.rpc_callms
o.rpc_callms:	^.^.AUN.include.sys.h.param
o.rpc_callms:	^.^.AUN.include.sys.h.sysconf
o.rpc_callms:	^.^.AUN.include.machine.h.machparam
o.rpc_callms:	^.^.AUN.include.sys.h.signal
o.rpc_callms:	^.^.AUN.include.sys.h.types
o.rpc_callms:	^.rpc.h.rpc
o.rpc_callms:	^.rpc.h.types
o.rpc_callms:	^.^.AUN.include.sys.h.time
o.rpc_callms:	^.^.AUN.include.sys.h.time
o.rpc_callms:	^.^.AUN.include.netinet.h.in
o.rpc_callms:	^.rpc.h.xdr
o.rpc_callms:	^.rpc.h.auth
o.rpc_callms:	^.rpc.h.clnt
o.rpc_callms:	^.rpc.h.rpc_msg
o.rpc_callms:	^.rpc.h.auth_unix
o.rpc_callms:	^.rpc.h.svc
o.rpc_callms:	^.rpc.h.svc_auth
o.rpc_callms:	^.rpc.h.netdb
o.xdr:	c.xdr
o.xdr:	^.rpc.h.types
o.xdr:	^.^.AUN.include.sys.h.types
o.xdr:	^.^.AUN.include.sys.h.time
o.xdr:	^.^.AUN.include.sys.h.time
o.xdr:	^.rpc.h.xdr
o.xdr_array:	c.xdr_array
o.xdr_array:	^.rpc.h.types
o.xdr_array:	^.^.AUN.include.sys.h.types
o.xdr_array:	^.^.AUN.include.sys.h.time
o.xdr_array:	^.^.AUN.include.sys.h.time
o.xdr_array:	^.rpc.h.xdr
o.xdr_mem:	c.xdr_mem
o.xdr_mem:	^.rpc.h.types
o.xdr_mem:	^.^.AUN.include.sys.h.types
o.xdr_mem:	^.^.AUN.include.sys.h.time
o.xdr_mem:	^.^.AUN.include.sys.h.time
o.xdr_mem:	^.rpc.h.xdr
o.xdr_mem:	^.^.AUN.include.netinet.h.in
o.xdr_rec:	c.xdr_rec
o.xdr_rec:	^.rpc.h.types
o.xdr_rec:	^.^.AUN.include.sys.h.types
o.xdr_rec:	^.^.AUN.include.sys.h.time
o.xdr_rec:	^.^.AUN.include.sys.h.time
o.xdr_rec:	^.rpc.h.xdr
o.xdr_rec:	^.^.AUN.include.netinet.h.in
o.xdr_refere:	c.xdr_refere
o.xdr_refere:	^.rpc.h.types
o.xdr_refere:	^.^.AUN.include.sys.h.types
o.xdr_refere:	^.^.AUN.include.sys.h.time
o.xdr_refere:	^.^.AUN.include.sys.h.time
o.xdr_refere:	^.rpc.h.xdr
o.xdr_stdio:	c.xdr_stdio
o.xdr_stdio:	^.rpc.h.types
o.xdr_stdio:	^.^.AUN.include.sys.h.types
o.xdr_stdio:	^.^.AUN.include.sys.h.time
o.xdr_stdio:	^.^.AUN.include.sys.h.time
o.xdr_stdio:	^.rpc.h.xdr
o.callback: s.callback
o.callback: Hdr:ListOpts
o.callback: Hdr:Macros
o.callback: Hdr:System
o.callback: Hdr:SWIs
o.callback: Hdr:CPU.Generic26
o.callback: Hdr:IO.GenericIO
o.callback: Hdr:RISCOS
o.callback: Hdr:EnvNumbers
o.callback: Hdr:ModHand
