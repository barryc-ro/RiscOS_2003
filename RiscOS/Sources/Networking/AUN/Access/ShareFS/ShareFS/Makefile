# Makefile for ShareFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 21-Nov-94  AMcC   Created
# 05-Jan-95  AMcC   Added resources: rule and paths
# 21-Feb-95  AMcC    Resources now copied to AUNMsgs 
# 01-Oct-99  SNB    Recreated
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Component specific options:
#
COMPONENT  = ShareFS
EXPORTS    = ${EXP_HDR}.ShareD
RAM_MODULE = rm.Share+
ROM_MODULE = aof.ShareFS
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Generic options:
#
AS      = objasm
CC      = cc
CMHG    = cmhg -b
CP      = copy
LD      = link
MKDIR   = cdir
RM      = remove
WIPE    = -wipe
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

INCLUDES = ^.rmalib,^.fslib,TCPIPLibs:,RISC_OSLib:,C:
DFLAGS   = -DDCI4 -DCOMPAT_INET4 -DACCESSPLUS -DLOOKUPERR -UUSE_OWN_RESFS "-DFSNAME=\"Remote\""
CFLAGS   = -I${INCLUDES} -zM -Wp -zps1 -ffah ${DFLAGS} -wd
AFLAGS   = -I<Hdr$Dir>.Global.,<Hdr$Dir>.Interface. -Stamp -Quit

#
# Libraries
#
ANSILIB   = C:o.ansilib
CLIB      = C:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
C_ABSSYM  = RISCOSLIB:o.c_abssym

FSLIB     = ^.fslib.o.fslib
RMALIB    = ^.rmalib.o.rmalib
TCPLIB	  = TCPIPLibs:o.socklibzm TCPIPLibs:o.inetlibzm

OBJS =\
 irqs.o\
 ShareFShdr.o\
 resourcefs.o\
 sharep.o\
 shared.o\
 sharefiler.o\
 sharefs.o\
 sharehow.o\
 shareinet.o\
 window.o

#
# Rule patterns
#
.c.o:;      ${CC} -depend !Depend ${THROWBACK} ${CFLAGS} -c -o $@ $<
.s.o:;      ${AS} -depend !Depend ${THROWBACK} ${AFLAGS} $< $@
.cmhg.o:;   ${CMHG} -depend !Depend ${THROWBACK} -p -I^,C: -o $@ $<

#
# Main rules:
#
rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

all: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: resource export complete (Resources in AUNMsgs)

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

${EXP_HDR}.ShareD: hdr.ShareD
        ${CP} hdr.ShareD $@ ${CPFLAGS}

clean:
        dir ^.fslib; amu clean; back
        dir ^.rmalib; amu clean; back
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${ROM_MODULE}
        ${RM} ${RAM_MODULE}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${RMALIB} ${FSLIB} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${RMALIB} ${FSLIB} ${TCPLIB} ${ROMCSTUBS}

${RAM_MODULE}: ${OBJS} ${RMALIB} ${FSLIB} ${CLIB}
        ${LD} -o $@ -rmf ${OBJS} ${RMALIB} ${FSLIB} ${TCPLIB} ${CLIB}

${FSLIB}:
        dir ^.fslib; amu fslib.o; back

${RMALIB}:
        dir ^.rmalib; amu rmalib.o; back

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${C_ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
