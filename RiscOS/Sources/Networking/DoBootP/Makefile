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

CLIB		= C:o.Stubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
INETLIB		= TCPIPLibs:o.inetlibzm
SOCKLIB		= TCPIPLibs:o.socklibzm

DEBUG=		#-DDEBUG

CFLAGS=		-pcc -ffh -zM -I${INCLUDES} -D_rosmodule -DUSECMOS -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -depend !Depend -c

OBJS=		o.module o.ifrconfig o.irqs o.dobootphdr

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
		$(LD) -rmf -o $@ $(OBJS) $(CLIB) $(SOCKLIB) $(INETLIB)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} $(SOCKLIB) $(INETLIB)
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} $(SOCKLIB) $(INETLIB)

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
