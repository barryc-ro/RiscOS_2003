# Makefile for NetTime
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 17-Jun-96	JRH	Created
# 24-Nov-98	JMF	Switch to DebugLib for debugging
# 12-Jan-99	JMF	Add support for EUROPEAN_DST_BODGE
# 09-Jun-99     SBF	Added support for StripDepnd tool

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
STRIP   = -StripDepnd

ASFLAGS = -depend !Depend -Stamp -quit -module
CCFLAGS  = -c ${THROWBACK} -depend !Depend ${INCLUDES} -ffa -zps1 -zM -DCOMPAT_INET4 -Wp ${DFLAGS}
# Flags for debugging ...
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DCOMPAT_INET4 -Wp -Throwback -DDEBUG
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
UNIXLIB	   = TCPIPLibs:o.unixlibzm

#
# DebugLib support.
# Uncomment these or pass to AMU to enable debug build.
#
#DFLAGS	= -fn -DDEBUGLIB
#LIBSD = <Lib$Dir>.DebugLib.o.debuglibzm


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
OBJS      = extralong.o mul64.o machine.o module.o header.o nt_nvram.o dns.o \
            edst.o ntp.o rtcadjust.o dhcp.o dst.o adst.o

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
        ${RM} h.header
        @echo Stripping dependencies from makefile...
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        TokenCheck LocalRes:Messages
	${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGETSA}: ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${LIBSD}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${LIBSD}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB} ${UNIXLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB} ${UNIXLIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

module.o: header.h
machine.o: header.h

#
# Dynamic dependencies:
