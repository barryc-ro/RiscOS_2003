# Makefile for Freeway
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 21-Nov-94  AMcC   Created
# 05-Jan-95  AMcC   Added resources: rule and paths
#

#
# Component specific options:
#
COMPONENT  = Freeway
ROM_MODULE = aof.Freeway
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
RESDIR     = ^.^.AUNMsgs.Resources.${Locale}.Resources.${COMPONENT}

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

AFLAGS  = -depend !Depend -I<Hdr$Dir>.Global.,<Hdr$Dir>.Interface. -Stamp -quit
CFLAGS  = -ansi -zM -zps1 -ffah -Wp ${INCLUDES} ${DFLAGS}
DFLAGS  = -DAUTH32 -DCOMPAT_INET4
# -DDEBUG -DDEBUGSER
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

INCLUDES  = -ITCPIPLibs:,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = C:o.stubs
RLIB      = RISC_OSLIB:o.risc_oslib
#ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
C_ABSSYM  = RISCOSLIB:o.c_abssym

SOCKLIBZM = TCPIPLibs:o.socklibzm
INETLIBZM = TCPIPLibs:o.inetlibzm

OBJS =\
 o.fwhdr\
 o.irqs\
 o.module\
 o.objects

# o.inetfn\

#
# Rule patterns
#
.c.o:;      ${CC} -c -depend !Depend ${CFLAGS} -o $@ $<
.c.s:;      ${CC} -S ${CFLAGS} $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.cmhg.o:;   ${CMHG} -o $@ $<

#
# Main rules:
#
#
rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: Resources in AUNMsgs

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${RM} ${ROM_MODULE}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: imported objects removed

${ROM_MODULE}: ${OBJS} ${SOCKLIBZM} ${INETLIBZM} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${SOCKLIBZM} ${INETLIBZM} ${ROMCSTUBS}

rm.Freeway: ${OBJS} ${SOCKLIBZM} ${INETLIBZM} ${CLIB}
        ${LD} -o $@ -rmf ${OBJS} ${SOCKLIBZM} ${INETLIBZM} ${CLIB} 


# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE} ${C_ABSSYM} \
              -map > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
