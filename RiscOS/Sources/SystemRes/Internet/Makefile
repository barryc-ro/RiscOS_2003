# Makefile for Internet resources
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 03-Jul-96      KJB    Derived from Scrap makefile
#

#
# Program specific options:
#
COMPONENT  = Internet
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
SDIR       = Sources
INSTDIR    = ${INSTDIR}.${APP}
RESD       = ${RESDIR}.Internet

# Generic options:
#
MKDIR   = cdir
ATTR    = -attr
CP      = copy
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

FILES=\
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${RDIR}.!Run\
 ${RDIR}.!Sprites\
 ${RDIR}.!Sprites22\
 ${SDIR}.arp.ARP\
 ${RDIR}.bin.GetHost\
 ${RDIR}.bin.Host\
 ${SDIR}.ifconfig.IfConfig\
 ${SDIR}.inetstat.InetStat\
 ${SDIR}.map-mbone.Map-MBone\
 ${SDIR}.mtrace.MTrace\
 ${SDIR}.ping.Ping\
 ${SDIR}.pong.Pong\
 ${SDIR}.utils.utl.RMFind\
 ${SDIR}.route.Route\
 ${SDIR}.traceroute.TraceRoute\
 ${SDIR}.showstat.ShowStat\
 ${SDIR}.sysctl.SysCtl\
 ${SDIR}.utils.utl.X\
 ${RDIR}.files.AUNMap\
 ${RDIR}.files.Hosts\
 ${RDIR}.files.Networks\
 ${RDIR}.files.Protocols\
 ${RDIR}.files.Services\
 ${RDIR}.utils.BootNet\
 ${SDIR}.utils.utl.CheckMem\
 ${SDIR}.newfiler.NewFiler\
 ${SDIR}.utils.utl.ReadCMOSIP\
 ${SDIR}.utils.utl.TriggerCBs

NETUTILS = \
        ${SDIR}.arp.ARP              \
        ${SDIR}.ifconfig.IfConfig    \
        ${SDIR}.inetstat.InetStat    \
        ${SDIR}.map-mbone.Map-MBone  \
        ${SDIR}.mtrace.MTrace        \
        ${SDIR}.ping.Ping            \
        ${SDIR}.pong.Pong            \
        ${SDIR}.utils.utl.RMFind     \
        ${SDIR}.route.Route          \
        ${SDIR}.showstat.ShowStat    \
        ${SDIR}.sysctl.SysCtl        \
        ${SDIR}.traceroute.TraceRoute \
        ${SDIR}.utils.utl.X           \
        ${RDIR}.!Boot                 \
        ${RDIR}.!Run                  \
        ${RDIR}.!Sprites              \
        ${RDIR}.!Sprites22

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc)

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} ${RDIR}.bin        ${INSTDIR}.bin        ${CPFLAGS}
        ${CP} ${RDIR}.files      ${INSTDIR}.files      ${CPFLAGS}
        ${CP} ${RDIR}.utils      ${INSTDIR}.utils      ${CPFLAGS}
        |
        ${CP} ${SDIR}.arp.ARP               ${INSTDIR}.bin.ARP          ${CPFLAGS}
        ${CP} ${SDIR}.ifconfig.IfConfig     ${INSTDIR}.bin.IfConfig     ${CPFLAGS}
        ${CP} ${SDIR}.inetstat.InetStat     ${INSTDIR}.bin.InetStat     ${CPFLAGS}
        ${CP} ${SDIR}.map-mbone.Map-MBone   ${INSTDIR}.bin.Map-MBone    ${CPFLAGS}
        ${CP} ${SDIR}.mtrace.MTrace         ${INSTDIR}.bin.MTrace       ${CPFLAGS}
        ${CP} ${SDIR}.ping.Ping             ${INSTDIR}.bin.Ping         ${CPFLAGS}
        ${CP} ${SDIR}.pong.Pong             ${INSTDIR}.bin.Pong         ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.RMFind      ${INSTDIR}.bin.RMFind       ${CPFLAGS}
        ${CP} ${SDIR}.route.Route           ${INSTDIR}.bin.Route        ${CPFLAGS}
        ${CP} ${SDIR}.showstat.ShowStat     ${INSTDIR}.bin.ShowStat     ${CPFLAGS}
        ${CP} ${SDIR}.sysctl.SysCtl         ${INSTDIR}.bin.SysCtl       ${CPFLAGS}
        ${CP} ${SDIR}.traceroute.TraceRoute ${INSTDIR}.bin.TraceRoute   ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.X           ${INSTDIR}.bin.X            ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.CheckMem    ${INSTDIR}.utils.CheckMem   ${CPFLAGS}
        ${CP} ${SDIR}.newfiler.NewFiler     ${INSTDIR}.utils.NewFiler   ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.ReadCMOSIP  ${INSTDIR}.utils.ReadCMOSIP ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.TriggerCBs  ${INSTDIR}.utils.TriggerCBs ${CPFLAGS}
        |
        ${ATTR} -directories +wr -ww ${INSTDIR}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
	dir ${SDIR}
	!MkMods clean
	up
        @echo ${COMPONENT}: cleaned

resources: resources-${TYPE}

resources-:
	@echo ${COMPONENT}: No resource files to copy

resources-NetUtils: ${NETUTILS}
	${MKDIR} ${RESD}
	${MKDIR} ${RESD}.bin
	${MKDIR} ${RESD}.files
        ${CP} ${SDIR}.arp.ARP               ${RESD}.bin.ARP          ${CPFLAGS}
        ${CP} ${SDIR}.ifconfig.IfConfig     ${RESD}.bin.IfConfig     ${CPFLAGS}
        ${CP} ${SDIR}.inetstat.InetStat     ${RESD}.bin.InetStat     ${CPFLAGS}
        ${CP} ${SDIR}.map-mbone.Map-MBone   ${RESD}.bin.Map-MBone    ${CPFLAGS}
        ${CP} ${SDIR}.mtrace.MTrace         ${RESD}.bin.MTrace       ${CPFLAGS}
        ${CP} ${SDIR}.ping.Ping             ${RESD}.bin.Ping         ${CPFLAGS}
        ${CP} ${SDIR}.pong.Pong             ${RESD}.bin.Pong         ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.RMFind      ${RESD}.bin.RMFind       ${CPFLAGS}
        ${CP} ${SDIR}.route.Route           ${RESD}.bin.Route        ${CPFLAGS}
        ${CP} ${SDIR}.sysctl.SysCtl         ${RESD}.bin.SysCtl       ${CPFLAGS}
        ${CP} ${SDIR}.showstat.ShowStat     ${RESD}.bin.ShowStat     ${CPFLAGS}
        ${CP} ${SDIR}.traceroute.TraceRoute ${RESD}.bin.TraceRoute   ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.X           ${RESD}.bin.X            ${CPFLAGS}
        ${CP} ${RDIR}.files.AUNMap          ${RESD}.files.AUNMap     ${CPFLAGS}
        ${CP} ${RDIR}.files.Hosts           ${RESD}.files.Hosts      ${CPFLAGS}
        ${CP} ${RDIR}.files.Networks        ${RESD}.files.Networks   ${CPFLAGS}
        ${CP} ${RDIR}.files.Protocols       ${RESD}.files.Protocols  ${CPFLAGS}
        ${CP} ${RDIR}.files.Services        ${RESD}.files.Services   ${CPFLAGS}
        ${CP} ${RDIR}.!Boot                 ${RESD}.!Boot      ${CPFLAGS}
        ${CP} ${RDIR}.!Run                  ${RESD}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites              ${RESD}.!Sprites   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22            ${RESD}.!Sprites22 ${CPFLAGS}

${SDIR}.arp.ARP: ${SDIR}.arp.c.ARP
	dir ${SDIR}.arp
	@amu_machine
	@up
	@up

${SDIR}.ifconfig.IfConfig: ${SDIR}.ifconfig.c.IfConfig
	dir ${SDIR}.ifconfig
	@amu_machine
	@up
	@up

${SDIR}.inetstat.InetStat: ${SDIR}.inetstat.c.main \
	${SDIR}.inetstat.c.if\
	${SDIR}.inetstat.c.inet\
	${SDIR}.inetstat.c.route\
	${SDIR}.inetstat.c.mroute
	dir ${SDIR}.inetstat
	@amu_machine
	@up
	@up

${SDIR}.map-mbone.Map-MBone: ${SDIR}.map-mbone.c.Map-MBone \
	${SDIR}.map-mbone.c.igmp \
	${SDIR}.map-mbone.c.inet \
	${SDIR}.map-mbone.c.kern
	dir ${SDIR}.map-mbone
	@amu_machine
	@up
	@up

${SDIR}.mtrace.MTrace: ${SDIR}.mtrace.c.MTrace
	dir ${SDIR}.mtrace
	@amu_machine
	@up
	@up

${SDIR}.ping.Ping: ${SDIR}.ping.c.Ping
	dir ${SDIR}.ping
	@amu_machine
	@up
	@up

${SDIR}.pong.Pong: ${SDIR}.pong.c.Pong
	dir ${SDIR}.pong
	@amu_machine
	@up
	@up

${SDIR}.route.Route: ${SDIR}.route.c.Route
	dir ${SDIR}.route
	@amu_machine
	@up
	@up

${SDIR}.showstat.ShowStat:
	dir ${SDIR}.showstat
	@amu_machine 
	@up
	@up

${SDIR}.sysctl.SysCtl: ${SDIR}.sysctl.c.SysCtl
	dir ${SDIR}.sysctl
	@amu_machine
	@up
	@up

${SDIR}.traceroute.TraceRoute: ${SDIR}.traceroute.c.TraceRoute
	dir ${SDIR}.traceroute
	@amu_machine
	@up
	@up

${SDIR}.newfiler.NewFiler: ${SDIR}.newfiler.c.NewFiler
	dir ${SDIR}.newfiler
	@amu_machine
	@up
	@up

${SDIR}.utils.utl.CheckMem: ${SDIR}.utils.s.CheckMem
	dir ${SDIR}.utils
	@amu_machine utl.CheckMem
	@up
	@up

${SDIR}.utils.utl.ReadCMOSIP: ${SDIR}.utils.s.ReadCMOSIP
	dir ${SDIR}.utils
	@amu_machine utl.ReadCMOSIP
	@up
	@up

${SDIR}.utils.utl.RMFind: ${SDIR}.utils.s.RMFind
	dir ${SDIR}.utils
	@amu_machine utl.RMFind
	@up
	@up

${SDIR}.utils.utl.TriggerCBs: ${SDIR}.utils.s.TriggerCBs
	dir ${SDIR}.utils
	@amu_machine utl.TriggerCBs
	@up
	@up

${SDIR}.utils.utl.X: ${SDIR}.utils.s.X
	dir ${SDIR}.utils
	@amu_machine utl.X
	@up
	@up


#---------------------------------------------------------------------------
# Dynamic dependencies:
