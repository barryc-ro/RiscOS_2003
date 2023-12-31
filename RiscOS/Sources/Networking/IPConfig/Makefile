# Makefile for IPConfig
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Oct-98  SNB          Created

COMPONENT = IPConfig
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}D

# Build options
#
# The module has the following build options accessed by ensuring that OPTIONS
# is set to something that can be passed directly as a parameter to the C compiler
# Do not set OPTIONS in this file (or if you do, make it additive)
#
# -DDNS_SUPPORT_ADDRESS_TO_NAME     - include the domain name retrieval code
# -DDNS_SUPPORT_NAME_TO_ADDRESS     - include the name->IP address code
# -DINHIBIT_AUTO_START              - wait for a prod before doing anything
# -DINHIBIT_CMOS_EDITOR             - stop editor running (for STB400)
# -DUSENVRAM                        - use NVRAM module and not CMOS directly
# -DINHIBIT_ALT_KEYS                - Alt keys have no effect on behaviour
#
# None of these are included by default.


#
# Paths
#
EXP_HDR = <export$dir>

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}
RESFSDIR   = Resources.${COMPONENT}

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
TOUCH	= stamp
AWK     = awk

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

AWKVARS  = -v cpflags="${CPFLAGS}" -v resdir="${RESDIR}" -v resfsdir="${RESFSDIR}"
AWKFLAGS = -f LocalRes:^.awk-script ${AWKVARS}
AWKTMP1   = awkexec
AWKTMP2   = awkramres

#
# Include files
#
INCLUDES=	-ITCPIPLibs:,C:

#
# Libraries
#
CLIB		= Clib:o.Stubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
INETLIB		= TCPIPLibs:o.inetlibzm
SOCKLIB		= TCPIPLibs:o.socklibzm

DEPEND		= -depend !Depend
DBGFLAGS        = -DTRACE
DEFINES         = -DCOMPAT_INET4
CENV		= ${DEPEND} ${THROWBACK} ${OPTIONS}

ASFLAGS		= ${DEPEND} ${THROWBACK}
CFLAGS		= -c -ffah -zM -zps1 -Wp ${INCLUDES} ${DEFINES} ${CENV}
CMHGFLAGS	= ${DEPEND} ${THROWBACK} -p

ROM_OBJS	= o.iicpoke or.module or.ifrconfig or.editconfig or.utils\
                  or.nvram or.nvram_cmos\
		  or.lineedit or.Strncmp_ci o.atmconfig  o.dns o.ipconfhdr
RAM_OBJS	= o.iicpoke o.module  o.ifrconfig  o.editconfig  o.utils \
                  o.nvram o.nvram_cmos\
		  o.lineedit  o.Strncmp_ci  o.atmconfig  o.dns o.ipconfhdr  ${RESF}
DBG_OBJS	= o.iicpoke od.module od.ifrconfig od.editconfig od.utils\
		  od.nvram od.nvram_cmos\
		  od.lineedit od.Strncmp_ci od.atmconfig od.dns od.ipconfhdr ${DBGRESF}

RESF		= o.msgs

DBGRESF		= od.msgs

#
# Rule patterns
#
.SUFFIXES:	.o .od .or .s .c .cmhg .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.c.or:;		${CC} ${CFLAGS} -DROM -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $< -d h.IPConfHdr
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} -o $@ $< -d h.IPConfHdrD ${DBGFLAGS}
.s.o:;		${AS} ${ASFLAGS} -o $@ $<

IPConfHdr.h: o.IPConfHdr
	${TOUCH} $@
	
IPConfHdrD.h: od.IPConfHdr
	${TOUCH} $@
	
o.IPConfHdr: VersionNum

od.IPConfHdr: VersionNum

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_File Resources.awk-script

#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (RAM)

install: ${RAM_MODULE} o.dirs dirs
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module built (RAM)

dirs:
	${MKDIR} ${INSTDIR}

ramtrace: ${DBG_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages
        TokenCheck LocalRes:Messages
        IfThere LocalSystemRes:Custom Then TokenCheck LocalSystemRes:Custom
        ${MKDIR} ${RESDIR}
        ${CP} LocalRes:Messages ${RESDIR}.Messages ${CPFLAGS}
        IfThere LocalSystemRes:Custom Then\
        	${CP} LocalSystemRes:Custom ${RESDIR}.Custom
        ${AWK} ${AWKFLAGS} -v phase=$@ -v awkexec="${AWKTMP1}"
        SetType ${AWKTMP1} Obey
        Run ${AWKTMP1}
        @echo ${COMPONENT}: resource files copied to Messages module

cleanres:
        ${RM}   ${AWKTMP1}
        ${RM}   ${AWKTMP2}
        @echo ${COMPONENT}: resource via files cleaned

clean: cleanres
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} or ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.IPConfHdr* ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} or
	create $@

export:
        @echo ${COMPONENT}: export complete


#
# Targets
#
${RAM_MODULE}:	${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}:	${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
	${MKDIR} aof
	$(LD) -o $@ -aof ${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}

${DBG_MODULE}:	${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        Access $@ RW/R

${AWKTMP2}: LocalRes:^.awk-script
        ${AWK} ${AWKFLAGS} -v phase=ramresources -v awkexec="${AWKTMP2}"

${RESF}: LocalRes:Messages ${AWKTMP2}
	TokenCheck LocalRes:Messages
	resgen messages_file $@ \
		LocalRes:Messages ${RESFSDIR}.Messages\
		-via ${AWKTMP2}

${DBGRESF}: LocalRes:Messages ${AWKTMP2}
	TokenCheck LocalRes:Messages
	resgen messages_file $@ \
		LocalRes:Messages ${RESFSDIR}.Messages \
		Inet:bin.IfConfig IfConfig \
		Inet:bin.InetStat InetStat \
		Inet:bin.Route Route \
		-via ${AWKTMP2}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
