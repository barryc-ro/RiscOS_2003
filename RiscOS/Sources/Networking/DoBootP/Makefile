# Makefile for DoBootP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 05-Sep-96  SMC          Created
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
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	TCPIPLibs:,C:

#
# Program specific options:
#
COMPONENT = DoBootP
TARGET = rm.DoBootP
ROMTARGET = aof.DoBootP

# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

STUBS=		C:o.Stubs

DEBUG=		#-DDEBUG

CFLAGS=		-pcc -ffh -zM -I${INCLUDES} -D_rosmodule -DUSECMOS ${DEBUG}\
		-zps1 -depend !Depend -c

OBJS=		o.module o.stubs o.ifrconfig o.inetfn o.irqs o.dobootphdr

#
# Rule patterns
#
.SUFFIXES:	.o .s .c
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.cmhg.o:;	${CMHG} -o $@ $<
.s.o:;		$(AS) -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export:
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
		$(WIPE) o.*      $(WFLAGS)
		$(WIPE) rm.*     $(WFLAGS)
		$(WIPE) aof.*    $(WFLAGS)
		$(WIPE) linked.* $(WFLAGS)

#
# Target 
#
${TARGET}:	$(OBJS)
		$(LD) -rmf -o $@ $(OBJS) $(STUBS)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
o.module:	c.module
o.module:	TCPIPLibs:sys.h.types
o.module:	TCPIPLibs:sys.h.uio
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:sys.h.time
o.module:	TCPIPLibs:sys.h.time
o.module:	TCPIPLibs:sys.h.ioctl
o.module:	TCPIPLibs:sys.h.ttychars
o.module:	TCPIPLibs:sys.h.ttydev
o.module:	TCPIPLibs:netinet.h.in
o.module:	TCPIPLibs:net.h.if
o.module:	TCPIPLibs:sys.h.mbuf
o.module:	TCPIPLibs:net.h.if_arp
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:sys.h.errno
o.module:	TCPIPLibs:sys.^.h.errno
o.module:	C:h.kernel
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.dci4
o.stubs:	c.stubs
o.stubs:	TCPIPLibs:sys.h.types
o.stubs:	TCPIPLibs:sys.h.uio
o.stubs:	TCPIPLibs:sys.h.errno
o.stubs:	TCPIPLibs:sys.^.h.errno
o.stubs:	C:h.kernel
o.stubs:	TCPIPLibs:sys.h.socket
o.stubs:	TCPIPLibs:sys.h.socketvar
o.stubs:	C:h.kernel
o.inetfn:	c.inetfn
o.inetfn:	TCPIPLibs:sys.h.types
o.inetfn:	TCPIPLibs:netinet.h.in
o.ifrconfig:	c.ifrconfig
o.ifrconfig:	TCPIPLibs:sys.h.types
o.ifrconfig:	TCPIPLibs:sys.h.socket
o.ifrconfig:	TCPIPLibs:sys.h.ioctl
o.ifrconfig:	TCPIPLibs:sys.h.ttychars
o.ifrconfig:	TCPIPLibs:sys.h.ttydev
o.ifrconfig:	TCPIPLibs:net.h.route
o.ifrconfig:	TCPIPLibs:netinet.h.in
o.ifrconfig:	TCPIPLibs:net.h.if
o.ifrconfig:	TCPIPLibs:sys.h.mbuf
o.ifrconfig:	TCPIPLibs:net.h.if_arp
o.ifrconfig:	TCPIPLibs:sys.h.socket
o.ifrconfig:	TCPIPLibs:sys.h.errno
o.ifrconfig:	TCPIPLibs:sys.^.h.errno
o.ifrconfig:	C:h.kernel
o.ifrconfig:	C:h.kernel
o.ifrconfig:	C:h.swis
