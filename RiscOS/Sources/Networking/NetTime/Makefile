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
CCFLAGS  = -c -depend !Depend ${INCLUDES} -ffa -zM -DCOMPAT_INET4 -Wp
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
.cmhg.h:;   ${CMHG} -p -d $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
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
        ${RM} h.header
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

module.o: header.h
machine.o: header.h

#
# Dynamic dependencies:
