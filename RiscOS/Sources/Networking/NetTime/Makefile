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
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM
# Falgs for debugging ...
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -DDEBUG
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: -IINetInc: -IINetInc:^.^.libs

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

SOCKLIB	   = InetInc:^.^.libs.socklib.socklibzm
INETLIB	   = InetInc:^.^.libs.inetlib.inetlibzm

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
OBJS      = mul64.o machine.o module.o header.o

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
o.module:	c.module
o.module:	h.nettime
o.module:	C:h.swis
o.module:	C:h.kernel
o.mul64: s.mul64
o.machine:	c.machine
o.machine:	INetInc:^.^.libs.socklib.h.socklib
o.machine:	INetInc:sys.h.types
o.machine:	INetInc:sys.h.socket
o.machine:	INetInc:sys.h.stat
o.machine:	INetInc:sys.h.time
o.machine:	INetInc:sys.h.time
o.machine:	INetInc:sys.h.uio
o.machine:	C:h.kernel
o.machine:	INetInc:^.^.libs.inetlib.h.inetlib
o.machine:	INetInc:sys.h.types
o.machine:	INetInc:netinet.h.in
o.machine:	INetInc:sys.h.ioctl
o.machine:	INetInc:sys.h.ttychars
o.machine:	INetInc:sys.h.ttydev
o.machine:	INetInc:sys.h.errno
o.machine:	INetInc:sys.^.h.errno
o.machine:	C:h.kernel
o.machine:	INetInc:h.netdb
o.machine:	h.nettime
o.machine:	C:h.swis
