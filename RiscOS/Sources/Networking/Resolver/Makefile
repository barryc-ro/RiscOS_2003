# Makefile for resolver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <Cexport$dir>

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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS} -pcc
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
LDFLAGS = 

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIB=InetInc:^.^.libs.inetlib.inetlibzm
SOCKLIB=InetInc:^.^.libs.socklib.socklibzm
UNIXLIB=InetInc:^.^.libs.unixlib.unixlibzm
DESKLIB=libs.desklib.DeskLib_rm
#
# Include files
#
INCLUDES = -IC:,InetInc:

DFLAGS   = -Darm -DROM=1

#
# Program specific options:
#
COMPONENT = Resolver
TARGET    = aof.${COMPONENT}
OBJS      = res_query.o Events.o \
	    Cache.o ExtLib.o Hosts.o Resolver.o \
	    res_comp.o res_init.o res_mkquer.o  \
	    StrCaseCmp.o Stubs.o SWI.o
LIBS      = ${DESKLIB} ${INETLIB} ${SOCKLIB} ${UNIXLIB}
EXPORTS   =

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

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

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${LIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} ${LIBS}
        ${LD} ${LDFLAGS} -o $@ -module ${OBJS} ${CLIB} ${LIBS}

# Dynamic dependencies:
o.res_query:	c.res_query
o.res_query:	InetInc:sys.h.types
o.res_query:	InetInc:sys.h.socket
o.res_query:	InetInc:netinet.h.in
o.res_query:	InetInc:netns.h.ns
o.res_query:	h.netdb
o.res_query:	C:h.kernel
o.res_query:	InetInc:arpa.h.inet
o.res_query:	InetInc:arpa.h.nameser
o.res_query:	h.resolv
o.res_query:	InetInc:sys.h.types
o.res_query:	h.herrno
o.res_query:	libs.desklib.h.Core
o.res_query:	libs.desklib.h.LinkList
o.res_query:	h.Cache
o.res_query:	h.Core
o.res_query:	h.Debug
o.res_query:	h.Hosts
o.Events:	c.Events
o.Events:	C:h.kernel
o.Events:	C:h.swis
o.Events:	InetInc:sys.h.types
o.Events:	InetInc:sys.h.time
o.Events:	InetInc:sys.h.socket
o.Events:	InetInc:sys.h.ioctl
o.Events:	InetInc:sys.h.ttychars
o.Events:	InetInc:sys.h.ttydev
o.Events:	InetInc:netinet.h.in
o.Events:	InetInc:arpa.h.nameser
o.Events:	InetInc:arpa.h.inet
o.Events:	h.resolv
o.Events:	InetInc:sys.h.types
o.Events:	h.herrno
o.Events:	h.netdb
o.Events:	InetInc:sys.h.types
o.Events:	libs.desklib.h.Core
o.Events:	libs.desklib.h.LinkList
o.Events:	libs.desklib.h.Time
o.Events:	h.Cache
o.Events:	h.Events
o.Events:	h.Debug
o.Events:	h.res_query
o.Events:	h.Core
o.Events:	h.Hosts
o.Cache:	c.Cache
o.Cache:	C:h.kernel
o.Cache:	h.htypes
o.Cache:	h.netdb
o.Cache:	libs.desklib.h.Core
o.Cache:	libs.desklib.h.LinkList
o.Cache:	libs.desklib.h.Time
o.Cache:	h.Core
o.Cache:	h.Cache
o.Cache:	h.ExtLib
o.Cache:	h.Debug
o.Cache:	h.Hosts
o.ExtLib: s.ExtLib
o.ExtLib: hdr.RegDefs
o.ExtLib: hdr.SwiNos
o.Hosts:	c.Hosts
o.Hosts:	C:h.kernel
o.Hosts:	h.htypes
o.Hosts:	h.netdb
o.Hosts:	InetInc:sys.h.socket
o.Hosts:	InetInc:netinet.h.in
o.Hosts:	InetInc:arpa.h.inet
o.Hosts:	InetInc:arpa.h.nameser
o.Hosts:	h.resolv
o.Hosts:	h.herrno
o.Hosts:	libs.desklib.h.Core
o.Hosts:	libs.desklib.h.Time
o.Hosts:	libs.desklib.h.LinkList
o.Hosts:	h.Core
o.Hosts:	h.Name
o.Hosts:	h.Cache
o.Hosts:	h.ExtLib
o.Hosts:	h.Hosts
o.Hosts:	h.Debug
o.res_comp:	c.res_comp
o.res_comp:	h.htypes
o.res_comp:	InetInc:arpa.h.nameser
o.res_init:	c.res_init
o.res_init:	h.htypes
o.res_init:	InetInc:sys.h.socket
o.res_init:	InetInc:netinet.h.in
o.res_init:	InetInc:arpa.h.nameser
o.res_init:	h.resolv
o.res_init:	InetInc:arpa.h.inet
o.res_init:	h.Debug
o.res_init:	C:h.kernel
o.res_mkquer:	c.res_mkquer
o.res_mkquer:	h.htypes
o.res_mkquer:	InetInc:netinet.h.in
o.res_mkquer:	InetInc:arpa.h.nameser
o.res_mkquer:	h.resolv
o.StrCaseCmp:	c.StrCaseCmp
o.StrCaseCmp:	h.htypes
o.Stubs: s.Stubs
o.Stubs: hdr.RegDefs
o.Stubs: hdr.SwiNos
o.SWI:	c.SWI
o.SWI:	C:h.kernel
o.SWI:	C:h.swis
o.SWI:	h.htypes
o.SWI:	InetInc:sys.h.errno
o.SWI:	InetInc:sys.^.h.errno
o.SWI:	C:h.kernel
o.SWI:	h.netdb
o.SWI:	libs.desklib.h.Core
o.SWI:	libs.desklib.h.LinkList
o.SWI:	h.Cache
o.SWI:	h.Debug
o.SWI:	h.Events
o.SWI:	h.Hosts
o.SWI:	h.res_query
o.SWI:	h.ExtLib
o.SWI:	h.Core
