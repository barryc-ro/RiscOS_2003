# Makefile for Net
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 01-Dec-94  JDavies      Modified to allow build without Generic include files
#                         and to include disc-based, not built-in, swis.h
# 04-Jan-94  JDavies      Added build of NetMsgs module for use of Net module
#                         not with the RISC OS Black ROM. Start use of -fah
#                         option.
# 11-Jan-95  JDavies      Continue -fah use.
# 13-Jan-95  JDavies      Add -ff and -zps1 options
# 31-Jan-95  JDavies      Split c.debug off as a separate file
# 06-Feb-95  JDavies      Tidied
# 21-Feb-95  JDavies      Commented out NetMsgs stuff (rm.netMsgs is no longer
#                         needed or made.
# 22-Feb-95  JDavies      Removed need for CLibInc$Path to be set, uses
#                         C$Liboot instead.
# 14-Mar-95  JDavies      Added use of mnscommon.c. c.NetI now compilable -fahf
# 17-Mar-95  JDavies      Removed CNFLAGS definition, no longer needed
#

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

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -fahf -zps1 -pcc -zM ${INCLUDES} ${DFLAGS}
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
# Program specific options:
#
COMPONENT = Net
TARGET    = aof.Net

VPATH     = @ gwroute
INCLUDES  = -ITCPIPLibs:,C:
#DFLAGS    = -DDEBUG

LIBS   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklib5zm

OBJS   = o.mns o.mnscommon o.io o.swis o.configure o.netasm o.route o.showrt \
         o.text o.inetfn o.debug

EOBJS  = o.mnsi o.mnscommon o.io_e o.swis_e o.configure o.netasm \
         o.text o.inetfn o.debug

IOBJS  = o.mnsi o.mnscommon o.io o.swis o.configure o.netasm \
         o.text o.inetfn o.debug

GOBJS  = o.mnsg o.mnscommon o.io o.swis o.configure o.netasm o.showrt \
	 o.text o.inetfn \
         o.af o.if o.input o.output o.startup o.tables \
         o.timer o.trace o.inet o.data
         

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# AUN build rules:
#
# all:    rm.Net rm.NetB rm.NetE rm.NetG rm.NetI
all:    rm.Net rm.NetB rm.NetE rm.NetI

install:    rm.NetI
        ${CP} rm.NetI ${INSTDIR}.NetI ${CPFLAGS}
        Access ${INSTDIR}.NetI WR/r
        @echo ${COMPONENT}: NetI installed
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
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files are in AUNMsgs

${TARGET}: ${OBJS} cmhg.MNSHdr_b ${ROMSTUBS}
        ${CMHG} cmhg.mnsHdr_b o.mnshdr_b
        ${LD} -o $@ -aof o.mnshdr_b ${OBJS} ${LIBS} ${ROMSTUBS}

rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# AUN:
#
#rm.NetMsgs: Resources.UK.Messages
#        modgen rm.~NetMsgs NetMsgs NetMsgs 0.03 Resources.UK.Messages Resources.Net.Messages
#        modsqz rm.~NetMsgs
#        remove rm.NetMsgs
#        rename rm.~NetMsgs rm.NetMsgs

rm.Net:  ${OBJS} cmhg.MNSHdr_c ${LIBS} ${LCLIB}
         ${CMHG} cmhg.mnsHdr_c o.mnshdr_c
         ${LD} -o $@ -module -map o.mnshdr_c ${OBJS} ${LIBS} ${CLIB} > map.linkmap_c

rm.NetB: ${OBJS} cmhg.MNSHdr_b ${LIBS} ${LCLIB}
         ${CMHG} cmhg.mnsHdr_b o.mnshdr_b
         ${LD} -o $@ -module -map o.mnshdr_b ${OBJS} ${LIBS} ${CLIB} > map.linkmap_b

rm.NetE: ${EOBJS} cmhg.MNSHdr_e ${LIBS} ${LCLIB}
         ${CMHG} cmhg.mnshdr_e o.mnshdr_e
         ${LD} -o $@ -module -map o.mnshdr_e ${EOBJS} ${LIBS} ${CLIB} > map.linkmap_e

rm.NetI: ${IOBJS} cmhg.MNSHdr_i ${LIBS} ${LCLIB}
         ${CMHG} cmhg.mnshdr_i o.mnshdr_i
         ${LD} -o $@ -module -map o.mnshdr_i ${IOBJS} ${LIBS} ${CLIB} > map.linkmap_i

rm.NetG: ${GOBJS} cmhg.Mnshdr_g ${LIBS} ${LCLIB}
         ${CMHG} cmhg.mnshdr_g o.mnshdr_g
         ${LD} -o $@ -module -map o.mnshdr_g ${GOBJS} ${LIBS} ${CLIB} > map.linkmap_g

#
# Special rules:
#

io_e.o: c.io
        ${CC} ${CFLAGS} -DEDDS c.io -o o.io_e

swis_e.o: c.swis
        ${CC} ${CFLAGS} -DEDDS c.swis -o o.swis_e

# Dynamic dependencies:
o.debug:	c.debug
o.debug:	C:h.kernel
o.debug:	C:h.swis
o.debug:	h.debug
o.mns:	c.mns
o.mns:	C:h.kernel
o.mns:	C:h.swis
o.mns:	TCPIPLibs:sys.h.types
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:machine.h.endian
o.mns:	TCPIPLibs:machine.h.ansi
o.mns:	TCPIPLibs:machine.h.types
o.mns:	TCPIPLibs:sys.h.uio
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:sys.h.socket
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:sys.h.time
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:sys.h.ioctl
o.mns:	TCPIPLibs:sys.h.ttycom
o.mns:	TCPIPLibs:sys.h.ioccom
o.mns:	TCPIPLibs:sys.h.ioccom
o.mns:	TCPIPLibs:sys.h.filio
o.mns:	TCPIPLibs:sys.h.ioccom
o.mns:	TCPIPLibs:sys.h.sockio
o.mns:	TCPIPLibs:sys.h.ioccom
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:sys.h.dcistructs
o.mns:	TCPIPLibs:netinet.h.in
o.mns:	TCPIPLibs:net.h.if
o.mns:	TCPIPLibs:sys.h.socket
o.mns:	TCPIPLibs:sys.h.time
o.mns:	TCPIPLibs:net.h.if_arp
o.mns:	TCPIPLibs:sys.h.socket
o.mns:	TCPIPLibs:protocols.h.routed
o.mns:	h.netasm
o.mns:	h.module
o.mns:	h.mnscommon
o.mns:	h.configure
o.mns:	h.debug
o.mns:	h.inetfn
o.mns:	h.io
o.mns:	h.showrt
o.mns:	TCPIPLibs:h.socklib
o.mns:	TCPIPLibs:sys.h.types
o.mns:	TCPIPLibs:sys.h.socket
o.mns:	TCPIPLibs:sys.h.stat
o.mns:	TCPIPLibs:sys.h.time
o.mns:	TCPIPLibs:sys.h.cdefs
o.mns:	TCPIPLibs:sys.h.time
o.mns:	TCPIPLibs:sys.h.uio
o.mns:	C:h.kernel
o.mns:	h.swicode
o.mns:	h.text
o.mns:	h.routecode
o.mns:	h.mns
o.mnscommon:	c.mnscommon
o.mnscommon:	C:h.kernel
o.mnscommon:	C:h.swis
o.mnscommon:	TCPIPLibs:sys.h.types
o.mnscommon:	TCPIPLibs:sys.h.cdefs
o.mnscommon:	TCPIPLibs:machine.h.endian
o.mnscommon:	TCPIPLibs:machine.h.ansi
o.mnscommon:	TCPIPLibs:machine.h.types
o.mnscommon:	TCPIPLibs:sys.h.socket
o.mnscommon:	TCPIPLibs:sys.h.cdefs
o.mnscommon:	TCPIPLibs:sys.h.ioctl
o.mnscommon:	TCPIPLibs:sys.h.ttycom
o.mnscommon:	TCPIPLibs:sys.h.ioccom
o.mnscommon:	TCPIPLibs:sys.h.ioccom
o.mnscommon:	TCPIPLibs:sys.h.filio
o.mnscommon:	TCPIPLibs:sys.h.ioccom
o.mnscommon:	TCPIPLibs:sys.h.sockio
o.mnscommon:	TCPIPLibs:sys.h.ioccom
o.mnscommon:	TCPIPLibs:sys.h.cdefs
o.mnscommon:	TCPIPLibs:netinet.h.in
o.mnscommon:	TCPIPLibs:sys.h.dcistructs
o.mnscommon:	h.netasm
o.mnscommon:	h.module
o.mnscommon:	h.debug
o.mnscommon:	h.mns
o.mnscommon:	h.mnscommon
o.mnscommon:	h.text
o.io:	c.io
o.io:	TCPIPLibs:sys.h.types
o.io:	TCPIPLibs:sys.h.cdefs
o.io:	TCPIPLibs:machine.h.endian
o.io:	TCPIPLibs:machine.h.ansi
o.io:	TCPIPLibs:machine.h.types
o.io:	TCPIPLibs:sys.h.socket
o.io:	TCPIPLibs:sys.h.cdefs
o.io:	TCPIPLibs:netinet.h.in
o.io:	TCPIPLibs:netinet.h.in_systm
o.io:	TCPIPLibs:netinet.h.ip
o.io:	TCPIPLibs:netinet.h.ip_var
o.io:	TCPIPLibs:netinet.h.udp
o.io:	TCPIPLibs:netinet.h.udp_var
o.io:	TCPIPLibs:sys.h.errno
o.io:	C:h.kernel
o.io:	TCPIPLibs:sys.h.mbuf
o.io:	C:h.kernel
o.io:	C:h.swis
o.io:	h.module
o.io:	h.io
o.io:	h.mns
o.io:	h.netasm
o.io:	TCPIPLibs:h.socklib
o.io:	TCPIPLibs:sys.h.types
o.io:	TCPIPLibs:sys.h.socket
o.io:	TCPIPLibs:sys.h.stat
o.io:	TCPIPLibs:sys.h.time
o.io:	TCPIPLibs:sys.h.cdefs
o.io:	TCPIPLibs:sys.h.cdefs
o.io:	TCPIPLibs:sys.h.time
o.io:	TCPIPLibs:sys.h.uio
o.io:	TCPIPLibs:sys.h.cdefs
o.io:	C:h.kernel
o.swis:	c.swis
o.swis:	C:h.kernel
o.swis:	C:h.swis
o.swis:	TCPIPLibs:sys.h.types
o.swis:	TCPIPLibs:sys.h.cdefs
o.swis:	TCPIPLibs:machine.h.endian
o.swis:	TCPIPLibs:machine.h.ansi
o.swis:	TCPIPLibs:machine.h.types
o.swis:	h.module
o.swis:	h.debug
o.swis:	h.inetfn
o.swis:	h.mns
o.swis:	h.mnscommon
o.swis:	h.netasm
o.swis:	h.io
o.swis:	h.swicode
o.configure:	c.configure
o.configure:	C:h.kernel
o.configure:	C:h.swis
o.configure:	TCPIPLibs:sys.h.types
o.configure:	TCPIPLibs:sys.h.cdefs
o.configure:	TCPIPLibs:machine.h.endian
o.configure:	TCPIPLibs:machine.h.ansi
o.configure:	TCPIPLibs:machine.h.types
o.configure:	TCPIPLibs:sys.h.socket
o.configure:	TCPIPLibs:sys.h.cdefs
o.configure:	TCPIPLibs:sys.h.ioctl
o.configure:	TCPIPLibs:sys.h.ttycom
o.configure:	TCPIPLibs:sys.h.ioccom
o.configure:	TCPIPLibs:sys.h.ioccom
o.configure:	TCPIPLibs:sys.h.filio
o.configure:	TCPIPLibs:sys.h.ioccom
o.configure:	TCPIPLibs:sys.h.sockio
o.configure:	TCPIPLibs:sys.h.ioccom
o.configure:	TCPIPLibs:sys.h.cdefs
o.configure:	TCPIPLibs:sys.h.dcistructs
o.configure:	TCPIPLibs:net.h.if
o.configure:	TCPIPLibs:sys.h.socket
o.configure:	TCPIPLibs:sys.h.time
o.configure:	TCPIPLibs:sys.h.cdefs
o.configure:	TCPIPLibs:net.h.if_arp
o.configure:	TCPIPLibs:sys.h.socket
o.configure:	h.configure
o.configure:	h.swicode
o.configure:	h.module
o.configure:	h.debug
o.netasm: s.netasm
o.netasm: Hdr:ListOpts
o.netasm: Hdr:Macros
o.netasm: Hdr:System
o.netasm: Hdr:SWIs
o.netasm: Hdr:CPU.Generic26
o.netasm: Hdr:IO.GenericIO
o.netasm: Hdr:RISCOS
o.netasm: Hdr:EnvNumbers
o.netasm: Hdr:FSNumbers
o.netasm: Hdr:NewErrors
o.netasm: Hdr:ModHand
o.netasm: Hdr:Econet
o.netasm: Hdr:MsgTrans
o.netasm: Hdr:Symbols
o.netasm: Hdr:CMOS
o.route:	c.route
o.route:	TCPIPLibs:sys.h.types
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	TCPIPLibs:machine.h.endian
o.route:	TCPIPLibs:machine.h.ansi
o.route:	TCPIPLibs:machine.h.types
o.route:	TCPIPLibs:sys.h.socket
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	TCPIPLibs:sys.h.ioctl
o.route:	TCPIPLibs:sys.h.ttycom
o.route:	TCPIPLibs:sys.h.ioccom
o.route:	TCPIPLibs:sys.h.ioccom
o.route:	TCPIPLibs:sys.h.filio
o.route:	TCPIPLibs:sys.h.ioccom
o.route:	TCPIPLibs:sys.h.sockio
o.route:	TCPIPLibs:sys.h.ioccom
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	TCPIPLibs:net.h.route
o.route:	TCPIPLibs:net.h.radix
o.route:	TCPIPLibs:protocols.h.routed
o.route:	TCPIPLibs:netinet.h.in
o.route:	h.module
o.route:	h.routecode
o.route:	TCPIPLibs:h.socklib
o.route:	TCPIPLibs:sys.h.types
o.route:	TCPIPLibs:sys.h.socket
o.route:	TCPIPLibs:sys.h.stat
o.route:	TCPIPLibs:sys.h.time
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	TCPIPLibs:sys.h.time
o.route:	TCPIPLibs:sys.h.uio
o.route:	TCPIPLibs:sys.h.cdefs
o.route:	C:h.kernel
o.showrt:	c.showrt
o.showrt:	C:h.kernel
o.showrt:	TCPIPLibs:sys.h.types
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	TCPIPLibs:machine.h.endian
o.showrt:	TCPIPLibs:machine.h.ansi
o.showrt:	TCPIPLibs:machine.h.types
o.showrt:	TCPIPLibs:sys.h.socket
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	TCPIPLibs:sys.h.ioctl
o.showrt:	TCPIPLibs:sys.h.ttycom
o.showrt:	TCPIPLibs:sys.h.ioccom
o.showrt:	TCPIPLibs:sys.h.ioccom
o.showrt:	TCPIPLibs:sys.h.filio
o.showrt:	TCPIPLibs:sys.h.ioccom
o.showrt:	TCPIPLibs:sys.h.sockio
o.showrt:	TCPIPLibs:sys.h.ioccom
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	TCPIPLibs:sys.h.mbuf
o.showrt:	TCPIPLibs:net.h.route
o.showrt:	TCPIPLibs:net.h.radix
o.showrt:	TCPIPLibs:netinet.h.in
o.showrt:	TCPIPLibs:h.nlist
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	h.module
o.showrt:	h.mnscommon
o.showrt:	h.mns
o.showrt:	h.showrt
o.showrt:	TCPIPLibs:h.socklib
o.showrt:	TCPIPLibs:sys.h.types
o.showrt:	TCPIPLibs:sys.h.socket
o.showrt:	TCPIPLibs:sys.h.stat
o.showrt:	TCPIPLibs:sys.h.time
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	TCPIPLibs:sys.h.time
o.showrt:	TCPIPLibs:sys.h.uio
o.showrt:	TCPIPLibs:sys.h.cdefs
o.showrt:	C:h.kernel
o.text:	c.text
o.text:	TCPIPLibs:sys.h.types
o.text:	TCPIPLibs:sys.h.cdefs
o.text:	TCPIPLibs:machine.h.endian
o.text:	TCPIPLibs:machine.h.ansi
o.text:	TCPIPLibs:machine.h.types
o.text:	h.module
o.text:	h.text
o.inetfn:	c.inetfn
o.inetfn:	TCPIPLibs:sys.h.types
o.inetfn:	TCPIPLibs:sys.h.cdefs
o.inetfn:	TCPIPLibs:machine.h.endian
o.inetfn:	TCPIPLibs:machine.h.ansi
o.inetfn:	TCPIPLibs:machine.h.types
o.inetfn:	TCPIPLibs:netinet.h.in
o.inetfn:	h.inetfn
o.mnsi:	c.mnsi
o.mnsi:	C:h.kernel
o.mnsi:	C:h.swis
o.mnsi:	TCPIPLibs:sys.h.types
o.mnsi:	TCPIPLibs:sys.h.cdefs
o.mnsi:	TCPIPLibs:machine.h.endian
o.mnsi:	TCPIPLibs:machine.h.ansi
o.mnsi:	TCPIPLibs:machine.h.types
o.mnsi:	TCPIPLibs:sys.h.uio
o.mnsi:	TCPIPLibs:sys.h.cdefs
o.mnsi:	TCPIPLibs:sys.h.socket
o.mnsi:	TCPIPLibs:sys.h.cdefs
o.mnsi:	TCPIPLibs:sys.h.time
o.mnsi:	TCPIPLibs:sys.h.cdefs
o.mnsi:	TCPIPLibs:sys.h.ioctl
o.mnsi:	TCPIPLibs:sys.h.ttycom
o.mnsi:	TCPIPLibs:sys.h.ioccom
o.mnsi:	TCPIPLibs:sys.h.ioccom
o.mnsi:	TCPIPLibs:sys.h.filio
o.mnsi:	TCPIPLibs:sys.h.ioccom
o.mnsi:	TCPIPLibs:sys.h.sockio
o.mnsi:	TCPIPLibs:sys.h.ioccom
o.mnsi:	TCPIPLibs:sys.h.cdefs
o.mnsi:	TCPIPLibs:netinet.h.in
o.mnsi:	TCPIPLibs:net.h.if
o.mnsi:	TCPIPLibs:sys.h.socket
o.mnsi:	TCPIPLibs:sys.h.time
o.mnsi:	TCPIPLibs:net.h.if_arp
o.mnsi:	TCPIPLibs:sys.h.socket
o.mnsi:	TCPIPLibs:sys.h.dcistructs
o.mnsi:	h.netasm
o.mnsi:	h.module
o.mnsi:	h.mns
o.mnsi:	h.mnscommon
o.mnsi:	h.debug
o.mnsi:	h.text
o.mnsi:	h.swicode
o.mnsg:	c.mnsg
o.mnsg:	C:h.kernel
o.mnsg:	C:h.swis
o.mnsg:	TCPIPLibs:sys.h.types
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:machine.h.endian
o.mnsg:	TCPIPLibs:machine.h.ansi
o.mnsg:	TCPIPLibs:machine.h.types
o.mnsg:	TCPIPLibs:sys.h.uio
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:sys.h.socket
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:sys.h.time
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:sys.h.ioctl
o.mnsg:	TCPIPLibs:sys.h.ttycom
o.mnsg:	TCPIPLibs:sys.h.ioccom
o.mnsg:	TCPIPLibs:sys.h.ioccom
o.mnsg:	TCPIPLibs:sys.h.filio
o.mnsg:	TCPIPLibs:sys.h.ioccom
o.mnsg:	TCPIPLibs:sys.h.sockio
o.mnsg:	TCPIPLibs:sys.h.ioccom
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:netinet.h.in
o.mnsg:	TCPIPLibs:net.h.if
o.mnsg:	TCPIPLibs:sys.h.socket
o.mnsg:	TCPIPLibs:sys.h.time
o.mnsg:	TCPIPLibs:net.h.if_arp
o.mnsg:	TCPIPLibs:sys.h.socket
o.mnsg:	TCPIPLibs:sys.h.dcistructs
o.mnsg:	TCPIPLibs:protocols.h.routed
o.mnsg:	h.netasm
o.mnsg:	h.module
o.mnsg:	h.mnscommon
o.mnsg:	h.configure
o.mnsg:	h.debug
o.mnsg:	h.inetfn
o.mnsg:	h.io
o.mnsg:	h.showrt
o.mnsg:	TCPIPLibs:h.socklib
o.mnsg:	TCPIPLibs:sys.h.types
o.mnsg:	TCPIPLibs:sys.h.socket
o.mnsg:	TCPIPLibs:sys.h.stat
o.mnsg:	TCPIPLibs:sys.h.time
o.mnsg:	TCPIPLibs:sys.h.cdefs
o.mnsg:	TCPIPLibs:sys.h.time
o.mnsg:	TCPIPLibs:sys.h.uio
o.mnsg:	C:h.kernel
o.mnsg:	h.swicode
o.mnsg:	h.text
o.mnsg:	h.mns
o.mnsg:	h.mnsg
o.mnsg:	gwroute.h.startup
o.mnsg:	gwroute.h.input
o.mnsg:	gwroute.h.trace
o.af:	gwroute.c.af
o.af:	TCPIPLibs:sys.h.types
o.af:	TCPIPLibs:sys.h.cdefs
o.af:	TCPIPLibs:machine.h.endian
o.af:	TCPIPLibs:machine.h.ansi
o.af:	TCPIPLibs:machine.h.types
o.af:	TCPIPLibs:sys.h.socket
o.af:	TCPIPLibs:sys.h.cdefs
o.af:	TCPIPLibs:netinet.h.in
o.af:	TCPIPLibs:net.h.route
o.af:	TCPIPLibs:net.h.radix
o.af:	TCPIPLibs:protocols.h.routed
o.af:	h.module
o.af:	TCPIPLibs:h.socklib
o.af:	TCPIPLibs:sys.h.types
o.af:	TCPIPLibs:sys.h.socket
o.af:	TCPIPLibs:sys.h.stat
o.af:	TCPIPLibs:sys.h.time
o.af:	TCPIPLibs:sys.h.cdefs
o.af:	TCPIPLibs:sys.h.cdefs
o.af:	TCPIPLibs:sys.h.time
o.af:	TCPIPLibs:sys.h.uio
o.af:	TCPIPLibs:sys.h.cdefs
o.af:	C:h.kernel
o.af:	gwroute.h.inetcode
o.af:	gwroute.h.table
o.af:	gwroute.h.data
o.af:	gwroute.h.af
o.if:	gwroute.c.if
o.if:	TCPIPLibs:sys.h.types
o.if:	TCPIPLibs:sys.h.cdefs
o.if:	TCPIPLibs:machine.h.endian
o.if:	TCPIPLibs:machine.h.ansi
o.if:	TCPIPLibs:machine.h.types
o.if:	TCPIPLibs:sys.h.socket
o.if:	TCPIPLibs:sys.h.cdefs
o.if:	TCPIPLibs:net.h.route
o.if:	TCPIPLibs:net.h.radix
o.if:	TCPIPLibs:netinet.h.in
o.if:	TCPIPLibs:protocols.h.routed
o.if:	TCPIPLibs:net.h.if
o.if:	TCPIPLibs:sys.h.socket
o.if:	TCPIPLibs:sys.h.time
o.if:	TCPIPLibs:sys.h.cdefs
o.if:	TCPIPLibs:net.h.if_arp
o.if:	TCPIPLibs:sys.h.socket
o.if:	gwroute.h.interface
o.if:	gwroute.h.table
o.if:	gwroute.h.data
o.input:	gwroute.c.input
o.input:	TCPIPLibs:sys.h.types
o.input:	TCPIPLibs:sys.h.cdefs
o.input:	TCPIPLibs:machine.h.endian
o.input:	TCPIPLibs:machine.h.ansi
o.input:	TCPIPLibs:machine.h.types
o.input:	TCPIPLibs:sys.h.socket
o.input:	TCPIPLibs:sys.h.cdefs
o.input:	TCPIPLibs:net.h.if
o.input:	TCPIPLibs:sys.h.socket
o.input:	TCPIPLibs:sys.h.time
o.input:	TCPIPLibs:sys.h.cdefs
o.input:	TCPIPLibs:net.h.if_arp
o.input:	TCPIPLibs:sys.h.socket
o.input:	TCPIPLibs:netinet.h.in
o.input:	TCPIPLibs:net.h.route
o.input:	TCPIPLibs:net.h.radix
o.input:	TCPIPLibs:protocols.h.routed
o.input:	h.mnsg
o.input:	h.debug
o.input:	gwroute.h.interface
o.input:	gwroute.h.table
o.input:	gwroute.h.data
o.input:	gwroute.h.startup
o.input:	gwroute.h.trace
o.input:	gwroute.h.defs
o.input:	gwroute.h.input
o.output:	gwroute.c.output
o.output:	TCPIPLibs:sys.h.types
o.output:	TCPIPLibs:sys.h.cdefs
o.output:	TCPIPLibs:machine.h.endian
o.output:	TCPIPLibs:machine.h.ansi
o.output:	TCPIPLibs:machine.h.types
o.output:	TCPIPLibs:sys.h.socket
o.output:	TCPIPLibs:sys.h.cdefs
o.output:	TCPIPLibs:net.h.if
o.output:	TCPIPLibs:sys.h.socket
o.output:	TCPIPLibs:sys.h.time
o.output:	TCPIPLibs:sys.h.cdefs
o.output:	TCPIPLibs:net.h.if_arp
o.output:	TCPIPLibs:sys.h.socket
o.output:	TCPIPLibs:netinet.h.in
o.output:	TCPIPLibs:net.h.route
o.output:	TCPIPLibs:net.h.radix
o.output:	TCPIPLibs:protocols.h.routed
o.output:	h.module
o.output:	h.mnsg
o.output:	gwroute.h.interface
o.output:	gwroute.h.table
o.output:	gwroute.h.data
o.output:	gwroute.h.trace
o.output:	gwroute.h.output
o.startup:	gwroute.c.startup
o.startup:	TCPIPLibs:sys.h.types
o.startup:	TCPIPLibs:sys.h.cdefs
o.startup:	TCPIPLibs:machine.h.endian
o.startup:	TCPIPLibs:machine.h.ansi
o.startup:	TCPIPLibs:machine.h.types
o.startup:	TCPIPLibs:sys.h.time
o.startup:	TCPIPLibs:sys.h.cdefs
o.startup:	TCPIPLibs:sys.h.ioctl
o.startup:	TCPIPLibs:sys.h.ttycom
o.startup:	TCPIPLibs:sys.h.ioccom
o.startup:	TCPIPLibs:sys.h.ioccom
o.startup:	TCPIPLibs:sys.h.filio
o.startup:	TCPIPLibs:sys.h.ioccom
o.startup:	TCPIPLibs:sys.h.sockio
o.startup:	TCPIPLibs:sys.h.ioccom
o.startup:	TCPIPLibs:sys.h.cdefs
o.startup:	TCPIPLibs:sys.h.socket
o.startup:	TCPIPLibs:sys.h.cdefs
o.startup:	TCPIPLibs:netinet.h.in
o.startup:	TCPIPLibs:net.h.if
o.startup:	TCPIPLibs:sys.h.socket
o.startup:	TCPIPLibs:sys.h.time
o.startup:	TCPIPLibs:net.h.if_arp
o.startup:	TCPIPLibs:sys.h.socket
o.startup:	TCPIPLibs:net.h.route
o.startup:	TCPIPLibs:net.h.radix
o.startup:	TCPIPLibs:protocols.h.routed
o.startup:	h.mnsg
o.startup:	h.module
o.startup:	h.debug
o.startup:	gwroute.h.table
o.startup:	gwroute.h.data
o.startup:	gwroute.h.inetcode
o.startup:	gwroute.h.output
o.startup:	gwroute.h.interface
o.startup:	gwroute.h.startup
o.tables:	gwroute.c.tables
o.tables:	TCPIPLibs:sys.h.types
o.tables:	TCPIPLibs:sys.h.cdefs
o.tables:	TCPIPLibs:machine.h.endian
o.tables:	TCPIPLibs:machine.h.ansi
o.tables:	TCPIPLibs:machine.h.types
o.tables:	TCPIPLibs:sys.h.socket
o.tables:	TCPIPLibs:sys.h.cdefs
o.tables:	TCPIPLibs:net.h.route
o.tables:	TCPIPLibs:net.h.radix
o.tables:	TCPIPLibs:netinet.h.in
o.tables:	TCPIPLibs:sys.h.ioctl
o.tables:	TCPIPLibs:sys.h.ttycom
o.tables:	TCPIPLibs:sys.h.ioccom
o.tables:	TCPIPLibs:sys.h.ioccom
o.tables:	TCPIPLibs:sys.h.filio
o.tables:	TCPIPLibs:sys.h.ioccom
o.tables:	TCPIPLibs:sys.h.sockio
o.tables:	TCPIPLibs:sys.h.ioccom
o.tables:	TCPIPLibs:sys.h.cdefs
o.tables:	TCPIPLibs:protocols.h.routed
o.tables:	h.mnsg
o.tables:	h.debug
o.tables:	gwroute.h.af
o.tables:	gwroute.h.trace
o.tables:	gwroute.h.table
o.tables:	gwroute.h.data
o.tables:	gwroute.h.startup
o.tables:	gwroute.h.defs
o.tables:	gwroute.h.interface
o.timer:	gwroute.c.timer
o.timer:	h.mnsg
o.timer:	gwroute.h.output
o.timer:	gwroute.h.timer
o.trace:	gwroute.c.trace
o.trace:	C:h.kernel
o.trace:	TCPIPLibs:sys.h.types
o.trace:	TCPIPLibs:sys.h.cdefs
o.trace:	TCPIPLibs:machine.h.endian
o.trace:	TCPIPLibs:machine.h.ansi
o.trace:	TCPIPLibs:machine.h.types
o.trace:	TCPIPLibs:sys.h.socket
o.trace:	TCPIPLibs:sys.h.cdefs
o.trace:	TCPIPLibs:sys.h.time
o.trace:	TCPIPLibs:sys.h.cdefs
o.trace:	TCPIPLibs:net.h.route
o.trace:	TCPIPLibs:net.h.radix
o.trace:	TCPIPLibs:netinet.h.in
o.trace:	TCPIPLibs:protocols.h.routed
o.trace:	h.module
o.trace:	h.inetfn
o.trace:	h.showrt
o.trace:	h.mnscommon
o.trace:	h.debug
o.trace:	gwroute.h.table
o.trace:	gwroute.h.data
o.trace:	gwroute.h.trace
o.trace:	gwroute.h.startup
o.inet:	gwroute.c.inet
o.inet:	TCPIPLibs:sys.h.types
o.inet:	TCPIPLibs:sys.h.cdefs
o.inet:	TCPIPLibs:machine.h.endian
o.inet:	TCPIPLibs:machine.h.ansi
o.inet:	TCPIPLibs:machine.h.types
o.inet:	TCPIPLibs:sys.h.socket
o.inet:	TCPIPLibs:sys.h.cdefs
o.inet:	TCPIPLibs:netinet.h.in
o.inet:	TCPIPLibs:net.h.route
o.inet:	TCPIPLibs:net.h.radix
o.inet:	TCPIPLibs:protocols.h.routed
o.inet:	gwroute.h.table
o.inet:	gwroute.h.data
o.inet:	gwroute.h.interface
o.inet:	gwroute.h.inetcode
o.data:	gwroute.c.data
o.data:	TCPIPLibs:sys.h.types
o.data:	TCPIPLibs:sys.h.cdefs
o.data:	TCPIPLibs:machine.h.endian
o.data:	TCPIPLibs:machine.h.ansi
o.data:	TCPIPLibs:machine.h.types
o.data:	TCPIPLibs:sys.h.socket
o.data:	TCPIPLibs:sys.h.cdefs
o.data:	TCPIPLibs:sys.h.time
o.data:	TCPIPLibs:sys.h.cdefs
o.data:	TCPIPLibs:net.h.route
o.data:	TCPIPLibs:net.h.radix
o.data:	TCPIPLibs:protocols.h.routed
o.data:	gwroute.h.interface
o.data:	gwroute.h.table
o.data:	gwroute.h.data
