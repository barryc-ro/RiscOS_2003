# Makefile for PPP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1998-01-19	BAL	Created
#

#
# Paths
#
EXP_HDR	=	<export$dir>
LIBDIR		= <Lib$Dir>

#
# Generic options:
#
MKDIR =		cdir
AS =		objasm
CC =		${MEMCHECKCC} cc
CMHG =		cmhg
CP =		copy
LD =		${MEMCHECKLD} link
RM =		remove
MODSQZ =	modsqz
WIPE =		-wipe
STRIPDEPEND =	stripdepnd

ASMFLAGS =	-depend !Depend ${THROWBACK} -predefine "BlockDrivers SETL ${ASMBLOCKDRIVERS}"
CFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -strict -wP -ffah ${CCEXTRA}
CMHGFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -p
CPFLAGS =	~cfr~v
WFLAGS =	~c~v

DFLAGS_COMMON = -DINET -DCOMPAT_INET4 -DVJC
DFLAGS =	${DFLAGS_COMMON} -DNDEBUG
DDFLAGS =       ${DFLAGS_COMMON} -DDEBUG -DDEBUGLIB

#
# Libraries
#
CLIB =		CLIB:o.stubs
RLIB =		RISCOSLIB:o.risc_oslib
RSTUBS =	RISCOSLIB:o.rstubs
ROMSTUBS =	RISCOSLIB:o.romstubs
ROMCSTUBS =	RISCOSLIB:o.romcstubs
ABSSYM  =	RISC_OSLib:o.c_AbsSym

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
DEBUGLIB	= $(LIBDIR).DebugLib.o.DebugLibzm
ASMUTILS	= $(LIBDIR).AsmUtils.o.AsmUtilszm

LIBS =\
 $(ASMUTILS)\
 ${INETLIB}\
 ${SOCKLIB}\
 ${UNIXLIB}\

# MemCheck:o.MC_M\


LIBSD =\
 $(DEBUGLIB)\
 $(ASMUTILS)\
 ${INETLIB}\
 ${SOCKLIB}\
 ${UNIXLIB}\


#
# Include files
#
INCLUDES =	-ITCPIPLibs:,C:

#
# Program specific options:
#
COMPONENT =	PPP
RAMTARGET =	rm.PPP
ROMTARGET =	aof.PPP
DEBUGTARGET =	rm.PPPD
EXPORTS =	${EXP_DIR}.PPP


#
# Export Paths for Messages module
#
#RESDIR = 	<resource$dir>.Resources2.PPP


OBJS =\
 o.blockdrive\
 o.if_ppp\
 o.mbuf\
 o.ppp_io\
 o.ppphdr\
 o.pppmodule\
 o.serial\
 o.slcompress\


OBJSD =\
 od.blockdrive\
 od.if_ppp\
 od.mbuf\
 od.ppp_io\
 od.ppphdr\
 od.pppmodule\
 od.serial\
 od.slcompress\


OBJSCP =\
 cp.if_ppp\
 cp.mbuf\
 o.ppphdr\
 cp.pppmodule\
 cp.serial\
 cp.slcompress\


HDRS =	h.ppphdr

#
# Rule patterns
#
.SUFFIXES:	.o .od .cp .h .s .c .cmhg
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}       -c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}      -c -zM -zps1 -fn -g -o $@ $<
.c.cp:;		${CC} ${CFLAGS} ${DFLAGS}       -c -C -E $< >> $@
.s.o:;		${AS} ${ASMFLAGS}               -o $@ $<
.s.od:;		${AS} ${ASMFLAGS}               -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}  -o $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}  -d $@ $<

#
# Main rules:
#
ram: ${RAMTARGET} local_dirs
        @echo ${COMPONENT}: all complete

rom: ${ROMTARGET} local_dirs
        @echo ${COMPONENT}: rom module built

debug: ${DEBUGTARGET} local_dirs
        @echo ${COMPONENT}: debug build complete

analyse: pppd ${OBJSCP} ${HDRS}
	@echo ${COMPONENT}: analysis build complete

export:
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @echo ${COMPONENT}: resource files copied

clean:	pppd
	${STRIPDEPEND}
	${RM} ${HDRS}
	${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	${WIPE} cp.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
	${WIPE} aof.* ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${RM} ${RAMTARGET}
	${RM} ${ROMTARGET}
	${RM} ${DEBUGTARGET}
	${RM} aof
	@echo ${COMPONENT}: cleaned

pppd:
        Dir pppd
        amu ${MFLAGS} ${MAKECMDGOALS}
        Up

local_dirs:
        ${MKDIR} aof
        ${MKDIR} rm

#
# Target 
#
${RAMTARGET}: pppd ${OBJS} ${HDRS} ${CLIB}
	${LD} -rmf -o $@ pppd.o.pppd ${OBJS} ${LIBS} ${CLIB}

#	${MODSQZ} $@

#
# ROM Target (re-linked at ROM Image build time)
#
${ROMTARGET}: pppd ${OBJS} ${HDRS} ${ROMCSTUBS}
	${LD} -o $@ -aof pppd.o.pppd ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Debug target
#
${DEBUGTARGET}: pppd ${OBJSD} ${HDRS} ${CLIB}
	${LD} -rmf -o $@ pppd.od.pppd ${OBJSD} ${LIBSD} ${CLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
