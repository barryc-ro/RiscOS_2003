# Makefile for Internet resources
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 03-Jul-96      KJB    Derived from Scrap makefile
# 05-Oct-99      SNB    Updated to allow slimmed down STB !Internet to be built.
#                       Passing OPTIONS=STB builds a cut down structure.
#

#
# Program specific options:
#
COMPONENT  = Internet
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
SDIR       = Sources
INSTAPP    = ${INSTDIR}.${APP}
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


COMMONFILES=\
 ${SDIR}.arp.ARP\
 ${SDIR}.ifconfig.IfConfig\
 ${SDIR}.inetstat.InetStat\
 ${SDIR}.ipvars.IPVars\
 ${SDIR}.ping.Ping\
 ${SDIR}.route.Route\
 ${SDIR}.showstat.ShowStat\
 ${SDIR}.sysctl.SysCtl\
 ${SDIR}.traceroute.TraceRoute\
 ${SDIR}.utils.utl.X\

DISCFILES= ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${RDIR}.!Run\
 ${RDIR}.!Sprites\
 ${RDIR}.!Sprites22\
 ${RDIR}.bin.GetHost\
 ${RDIR}.bin.Host\
 ${SDIR}.map-mbone.Map-MBone\
 ${SDIR}.mrinfo.MRInfo\
 ${SDIR}.mtrace.MTrace\
 ${SDIR}.pong.Pong\
 ${SDIR}.utils.utl.RMFind\
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

FILES=\
  ${COMMONFILES} \
  ${DISCFILES}
  
NETUTILS = \
        ${SDIR}.arp.ARP              \
        ${SDIR}.ifconfig.IfConfig    \
        ${SDIR}.inetstat.InetStat    \
        ${SDIR}.ipvars.IPVars        \
        ${SDIR}.map-mbone.Map-MBone  \
        ${SDIR}.mrinfo.MRInfo        \
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

install: install_${OPTIONS} install_common dirs
        ${ATTR} -directories +wr -ww ${INSTAPP}
        ${ATTR} -files +or +ow +wr -ww ${INSTAPP}
        @echo ${COMPONENT}: All installed (Disc)

install_common: ${COMMONFILES}
        ${CP} ${SDIR}.arp.ARP               ${INSTAPP}.bin.ARP          ${CPFLAGS}
        ${CP} ${SDIR}.ifconfig.IfConfig     ${INSTAPP}.bin.IfConfig     ${CPFLAGS}
        ${CP} ${SDIR}.inetstat.InetStat     ${INSTAPP}.bin.InetStat     ${CPFLAGS}
        ${CP} ${SDIR}.ipvars.IPVars         ${INSTAPP}.bin.IPVars       ${CPFLAGS}
        ${CP} ${SDIR}.ping.Ping             ${INSTAPP}.bin.Ping         ${CPFLAGS}
        ${CP} ${SDIR}.route.Route           ${INSTAPP}.bin.Route        ${CPFLAGS}
        ${CP} ${SDIR}.showstat.ShowStat     ${INSTAPP}.bin.ShowStat     ${CPFLAGS}
        ${CP} ${SDIR}.sysctl.SysCtl         ${INSTAPP}.bin.SysCtl       ${CPFLAGS}
        ${CP} ${SDIR}.traceroute.TraceRoute ${INSTAPP}.bin.TraceRoute   ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.X           ${INSTAPP}.bin.X            ${CPFLAGS}

install_STB:
        ${MKDIR} ${INSTAPP}.etc
        ${CP} LocalUserIFRes:!Boot     ${INSTAPP}.!Boot         ${CPFLAGS}
        ${CP} ${RDIR}.files.Hosts      ${INSTAPP}.etc.hosts     ${CPFLAGS}
        ${CP} ${RDIR}.files.protocols  ${INSTAPP}.etc.protocols ${CPFLAGS}
        ${CP} ${RDIR}.files.services   ${INSTAPP}.etc.services  ${CPFLAGS}

install_: ${DISCFILES} dirs
        ${CP} ${RDIR}.!Boot      ${INSTAPP}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTAPP}.!Help      ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${INSTAPP}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${INSTAPP}.!Sprites   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
        ${CP} ${RDIR}.bin        ${INSTAPP}.bin        ${CPFLAGS}
        ${CP} ${RDIR}.files      ${INSTAPP}.files      ${CPFLAGS}
        ${CP} ${RDIR}.utils      ${INSTAPP}.utils      ${CPFLAGS}
        |
        ${CP} ${SDIR}.map-mbone.Map-MBone   ${INSTAPP}.bin.Map-MBone    ${CPFLAGS}
        ${CP} ${SDIR}.mrinfo.MRInfo         ${INSTAPP}.bin.MRInfo       ${CPFLAGS}
        ${CP} ${SDIR}.mtrace.MTrace         ${INSTAPP}.bin.MTrace       ${CPFLAGS}
        ${CP} ${SDIR}.pong.Pong             ${INSTAPP}.bin.Pong         ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.RMFind      ${INSTAPP}.bin.RMFind       ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.CheckMem    ${INSTAPP}.utils.CheckMem   ${CPFLAGS}
        ${CP} ${SDIR}.newfiler.NewFiler     ${INSTAPP}.utils.NewFiler   ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.ReadCMOSIP  ${INSTAPP}.utils.ReadCMOSIP ${CPFLAGS}
        ${CP} ${SDIR}.utils.utl.TriggerCBs  ${INSTAPP}.utils.TriggerCBs ${CPFLAGS}

dirs:
        ${MKDIR} ${INSTAPP}
        ${MKDIR} ${INSTAPP}.bin

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
        ${CP} ${SDIR}.ipvars.IPVars         ${RESD}.bin.IPVars       ${CPFLAGS}
        ${CP} ${SDIR}.inetstat.InetStat     ${RESD}.bin.InetStat     ${CPFLAGS}
        ${CP} ${SDIR}.map-mbone.Map-MBone   ${RESD}.bin.Map-MBone    ${CPFLAGS}
        ${CP} ${SDIR}.mrinfo.MRInfo         ${RESD}.bin.MRInfo       ${CPFLAGS}
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

${SDIR}.ipvars.IPVars: ${SDIR}.ipvars.c.ipvars
	dir ${SDIR}.ipvars
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

${SDIR}.mrinfo.MRInfo: ${SDIR}.mrinfo.c.MRInfo \
	${SDIR}.mrinfo.c.igmp \
	${SDIR}.mrinfo.c.inet \
	${SDIR}.mrinfo.c.kern
	dir ${SDIR}.mrinfo
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
