# Makefile for PPP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Jul-96  SMC          Created
#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm -tb
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

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm

#
# Program specific options:
#
COMPONENT = PPP
TARGET = rm.PPP
ROMTARGET = aof.PPP

# To compile a version of PPP which uses the blockdrivers uncomment the next
# line, ensure that o.blockdrive is in the list of object files and change
# ASMFLAGS so that the symbol BlockDrivers is predefined to {TRUE}
# 
# The Makefile in pppd must also be changed

BLOCKDRIVERS=	-DBLOCKDRIVERS


# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

DEBUG=		#-DDEBUG

COMPAT=		-DCOMPAT_INET4

CFLAGS=		-ffah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -depend !Depend -c -throwback -DVJC ${TINYSTUBS}\
		${BLOCKDRIVERS}

ASMFLAGS=       -PreDefine "BlockDrivers SETL {TRUE}"

OBJS=		o.if_ppp o.slcompress o.pppmodule \
		o.serial o.mbuf o.ppphdr o.ppp_io o.blockdrive \
		${SOCKLIB} ${INETLIB} ${UNIXLIB} \
		#o.svcprint

#
# Rule patterns
#
.SUFFIXES:	.o .s .c
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<

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
		$(WIPE) pppd.o.* $(WFLAGS)
                $(WIPE) rm.*     $(WFLAGS)
		$(RM) link/sym

#
# Target 
#
${TARGET}:	$(OBJS) pppd
		$(LD) -rmf -s link/sym -o rm.PPPu $(OBJS) $(STUBS) pppd.o.pppd
		ModSqz rm.PPPu $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} pppd
        ${LD} -s link/sym -o $@ -aof ${OBJS} ${ROMCSTUBS} pppd.o.pppd

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

#
# Some rules that need explicitly declaring
#
pppd:
        Dir pppd
        Amu
        Up

s.blockdrive::	Internal.Driver
	
o.ppphdr:	cmhg.ppphdr
		cmhg -p ${BLOCKDRIVERS} -depend !Depend -throwback \
		cmhg.ppphdr o.ppphdr h.ppphdr


# Dynamic dependencies:
