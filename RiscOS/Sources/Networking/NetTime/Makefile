# Makefile for NetTime
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 17-Jun-96	JRH	Created
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
DEFMOD  = DefMod
AR	= LibFile

ASFLAGS = -depend !Depend -Stamp -quit -module
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DCOMPAT_INET4
# Falgs for debugging ...
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -DDEBUG
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: -ITCPIPLibs:

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

SOCKLIB	   = TCPIPLibs:o.socklibzm
INETLIB	   = TCPIPLibs:o.inetlibzm

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = NetTime
TARGET    = aof.NetTime
TARGETSA  = rm.NetTime
EXPORTS   = 
OBJS      = mul64.o machine.o module.o header.o nt_nvram.o

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: all complete

rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGETSA}: ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


#
# Dynamic dependencies:
o.mul64: s.mul64
o.machine:	c.machine
o.machine:	TCPIPLibs:h.socklib
o.machine:	TCPIPLibs:sys.h.types
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:machine.h.endian
o.machine:	TCPIPLibs:machine.h.ansi
o.machine:	TCPIPLibs:machine.h.types
o.machine:	TCPIPLibs:sys.h.socket
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.stat
o.machine:	TCPIPLibs:sys.h.time
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.time
o.machine:	TCPIPLibs:sys.h.uio
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	C:h.kernel
o.machine:	TCPIPLibs:h.inetlib
o.machine:	TCPIPLibs:sys.h.types
o.machine:	TCPIPLibs:netinet.h.in
o.machine:	TCPIPLibs:sys.h.ioctl
o.machine:	TCPIPLibs:sys.h.ttycom
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.filio
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.sockio
o.machine:	TCPIPLibs:sys.h.ioccom
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	TCPIPLibs:sys.h.errno
o.machine:	C:h.kernel
o.machine:	TCPIPLibs:h.netdb
o.machine:	TCPIPLibs:sys.h.cdefs
o.machine:	h.nettime
o.machine:	C:h.swis
o.machine:	h.nt_nvram
o.module:	c.module
o.module:	h.nettime
o.module:	C:h.swis
o.module:	C:h.kernel
o.module:	h.nt_nvram
o.nt_nvram:	c.nt_nvram
o.nt_nvram:	C:h.kernel
o.nt_nvram:	C:h.swis
o.nt_nvram:	h.nt_nvram
