# Makefile for LanManFS
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


AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -pcc -ff -zps1 -zM -I${INCLUDES},. ${DFLAGS} -UTML -DCOMPAT_INET4


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
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm

#
# Include files
#
LEVEL=		^
INCLUDES=	TCPIPLibs:,C:

#DFLAGS   = -dDEBUG
DFLAGS   =

COMPONENT = LanManFS
TARGET    = rm.LanManFS
ROMTARGET = aof.LanManFS
OBJS      = LanMan_MH.o LanMan.o Omni.o Logon.o CoreFn.o Printers.o Xlate.o Interface.o RMInfo.o buflib.o LLC.o NetBIOS.o SMB.o Errors.o Attr.o RPC.o NBIP.o Stats.o

#
# Rule patterns
#
#  .SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build target
#
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
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${ROMTARGET}
        ${RM} map.${COMPONENT}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS}
        ${LD} -o $@ -rmf ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $(TARGET)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
        
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
o.Interface: s.Interface
o.Interface: Hdr.RegNames
o.Interface: Hdr.SwiNames
o.Interface: Hdr.ResourceFS
o.RMInfo: s.RMInfo
o.Errors: s.Errors
