# Makefile for Inetlib module linkable version
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
COMPONENT = inetlibzm
TARGET    = inetlibzm
OBJS      =	  \
  o.inet_addr     \
  o.inet_nwork    \
  o.inet_nof      \
  o.inet_ntoa     \
  o.inet_lnaof    \
  o.inet_maddr    \
  o.getnbyad      \
  o.getnbyna      \
  o.getnent       \
  o.getprent      \
  o.getprnam      \
  o.getproto      \
  o.getsbyna      \
  o.getsbypo      \
  o.getsent       \
  o.gethent       \
  o.gethnad       \
  o.htonl

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

#
# Target 
#
${TARGET}: ${OBJS}
         libfile -c inetlibzm 30 20
         libfile -i inetlibzm o.*
         libfile -o -l inetlibzm


# Dynamic dependencies:
o.inet_addr:	c.inet_addr
o.inet_addr:	^.^.AUN.include.sys.h.types
o.inet_addr:	^.^.AUN.include.netinet.h.in
o.inet_nwork:	c.inet_nwork
o.inet_nwork:	^.^.AUN.include.sys.h.types
o.inet_nof:	c.inet_nof
o.inet_nof:	^.^.AUN.include.sys.h.types
o.inet_nof:	^.^.AUN.include.netinet.h.in
o.inet_ntoa:	c.inet_ntoa
o.inet_ntoa:	^.^.AUN.include.sys.h.types
o.inet_ntoa:	^.^.AUN.include.netinet.h.in
o.inet_lnaof:	c.inet_lnaof
o.inet_lnaof:	^.^.AUN.include.sys.h.types
o.inet_lnaof:	^.^.AUN.include.netinet.h.in
o.inet_maddr:	c.inet_maddr
o.inet_maddr:	^.^.AUN.include.sys.h.types
o.inet_maddr:	^.^.AUN.include.netinet.h.in
o.getnbyad:	c.getnbyad
o.getnbyad:	^.^.AUN.include.h.netdb
o.getnbyna:	c.getnbyna
o.getnbyna:	^.^.AUN.include.h.netdb
o.getnent:	c.getnent
o.getnent:	^.^.AUN.include.sys.h.types
o.getnent:	^.^.AUN.include.sys.h.socket
o.getnent:	^.^.AUN.include.h.netdb
o.getprent:	c.getprent
o.getprent:	^.^.AUN.include.sys.h.types
o.getprent:	^.^.AUN.include.sys.h.socket
o.getprent:	^.^.AUN.include.h.netdb
o.getprnam:	c.getprnam
o.getprnam:	^.^.AUN.include.h.netdb
o.getproto:	c.getproto
o.getproto:	^.^.AUN.include.h.netdb
o.getsbyna:	c.getsbyna
o.getsbyna:	^.^.AUN.include.h.netdb
o.getsbypo:	c.getsbypo
o.getsbypo:	^.^.AUN.include.h.netdb
o.getsent:	c.getsent
o.getsent:	^.^.AUN.include.sys.h.types
o.getsent:	^.^.AUN.include.sys.h.socket
o.getsent:	^.^.AUN.include.h.netdb
o.gethent:	c.gethent
o.gethent:	^.^.AUN.include.sys.h.types
o.gethent:	^.^.AUN.include.sys.h.socket
o.gethent:	^.^.AUN.include.h.netdb
o.gethnad:	c.gethnad
o.gethnad:	^.^.AUN.include.h.netdb
o.gethnad:	C:h.kernel
o.htonl:	c.htonl
o.htonl:	^.^.AUN.include.sys.h.types
