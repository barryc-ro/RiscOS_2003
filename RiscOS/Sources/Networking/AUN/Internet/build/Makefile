# Makefile for Internet
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 18-Nov-94  AMcC   Created
# 05-Jan-95  AMcC   Added resources: rule and paths
# 21-Feb-95  AMcC   Resources now copied to AUNMsgs
# 22-Feb-95  AMcC   Added rules to build from sources
#

#
# Component specific options:
#
COMPONENT  = Internet
ROM_MODULE = aof.Internet
TARGET     = rm.Internet
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
RESDIR    = ^.^.AUNMsgs.Resources.${Locale}.Resources.${COMPONENT}

# amu version 5.02 can't cope with @ in VPATH so we have to use ^.build
#
VPATH     = ^.build ^.kern ^.net ^.netinet ^.sys ^.riscos ^.lib ^.whoami

#
# Generic options:
#
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
MKDIR   = cdir
MODSQZ  = modsqz
RM      = remove
WIPE    = -wipe

AFLAGS   = -depend !Depend -I<Hdr$Dir>.Global.,<Hdr$Dir>.Interface. -Stamp -quit
CFLAGS   = -depend !Depend -zps1 -zM -ffa -Wanp ${INCLUDES} ${DFLAGS}
DFLAGS   = -DKERNEL -DINET -DGATEWAY -DCOMPAT_43 -DCOMPAT_OLDSOCK \
           -DDIRECTED_BROADCAST -DMULTICAST -DMROUTING -DIPFIREWALL \
           -DIPFIREWALL_VERBOSE #-DFLASHY_SCROLLLOCK -DTCPTDEBUG -DTCPDEBUG \
           #-DDIAGNOSTIC -DDEBUG_MTUDISC
INCLUDES = -ITCPIPLibs:
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym

OBJS =\
 InetHdr.o\
 poduleirq.o\
 kern_subr.o\
 sysctl.o\
 sys_socket.o\
 uipc_mbuf.o\
 kvm.o\
 unixenv.o\
 tick_entry.o\
 if.o\
 if_loop.o\
 if_module.o\
 radix.o\
 raw_cb.o\
 raw_usrreq.o\
 route.o\
 rtsock.o\
 if_ether.o\
 igmp.o\
 in.o\
 in_cksum.o\
 in_pcb.o\
 in_proto.o\
 in_rmx.o\
 ip_fw.o\
 ip_icmp.o\
 ip_input.o\
 ip_mroute.o\
 ip_output.o\
 raw_ip.o\
 tcp_debug.o\
 tcp_input.o\
 tcp_output.o\
 tcp_subr.o\
 tcp_timer.o\
 tcp_usrreq.o\
 udp_usrreq.o\
 globdata.o\
 mbuf.o\
 module.o\
 setsoft.o\
 socket_swi.o\
 init.o\
 swiveneers.o\
 domain.o\
 socket.o\
 socket1.o\
 whoami.o
#svcprint.o\
#tcp_debug.o

#
# Rule patterns
#
.c.o:;      ${CC} -c ${CFLAGS} -o $@ $<
.c.s:;      ${CC} -S ${CFLAGS} $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Main rules:
#
all: ${TARGET}
	@echo ${COMPONENT}: Module built (Relocatable)

${TARGET}: ${OBJS} ${CLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} -map
        ${MODSQZ} $@

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: Resources in AUNMsgs

clean:
        ${RM} ${ROM_MODULE}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        |truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
