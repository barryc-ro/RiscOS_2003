# Makefile for ICAClient project
# RISC OS client for Citrix ICA protocol
# SJM 17 Dec 1997

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!ICAClient
RESRES = <Resource$Dir>.Resources.ICAClient
SRC = Resources.${Locale}

#
# Generic options:
#
MKDIR   = @-cdir
AS      = objasm
CP      = -copy
RM      = @-remove
RMDIR   = @-remove
LD	= link
LIBFILE = libfile
CC	= cc
WIPE	= @-wipe

CCFLAGS = -c -depend !Depend ${INCLUDES} ${DFLAGS}
ASFLAGS = -depend !Depend ${ASINCLUDES} -predefine "HOST SETS \"RISCOS\""
CPFLAGS = ~cfr~v
WIPEFLAGS = ~c
LIBFILEFLAGS = -c

#
# Program specific options:
#
COMPONENT = ICAClient
TARGET    = aof.${COMPONENT}

ABSSYM    = RISC_OSLib:o.AbsSym

#
# Generic rules:
#
rom:    ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.ini
	${CP} ${SRC}.!Boot       ${RESAPP}.!Boot    ${CPFLAGS}
	${CP} ${SRC}.!Help       ${RESAPP}.!Help    ${CPFLAGS}
	${CP} ${SRC}.!Run        ${RESAPP}.!Run     ${CPFLAGS}
	${CP} ${SRC}.!Sprites    ${RESAPP}.!Sprites ${CPFLAGS}
	${CP} ${SRC}.EnsureTB    ${RESRES}.EnsureTB ${CPFLAGS}
	${CP} ${SRC}.Messages    ${RESRES}.Messages ${CPFLAGS}
	${CP} ${SRC}.Res         ${RESRES}.Res      ${CPFLAGS}
	${CP} ${SRC}.Splash      ${RESRES}.Splash   ${CPFLAGS}
	${CP} ${SRC}.ini.AppSrv   ${RESRES}.ini.AppSrv   ${CPFLAGS}
	${CP} ${SRC}.ini.Module   ${RESRES}.ini.Module   ${CPFLAGS}
	${CP} ${SRC}.ini.WFClient ${RESRES}.ini.WFClient ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#	${CP} ${SRC}.Sprites     ${RESRES}.Sprites  ${CPFLAGS}

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
        @echo ${COMPONENT}: export complete(hdrs)

#
# Final link for the ROM Image (using given base address)
#
rom_link: 
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

dirs_rom:
	${MKDIR} linked

# ----------------------------------------------------------------------------------------------------

# -DINCL_ENUM -DINCL_MODEM -DINCL_SCRIPT -DINCL_PRINTER -DINCL_ASYNC -DINCL_TCPIP -DINCL_REDUCER -DINCL_AUDIO

DFLAGS=		-DRISCOS -DBETA=1 -DVESA=0 -DREMOTE_DEBUG -DCOMPAT_INET4 -DINCL_PRINTER -DINCL_TCPIP

INCLUDES=	-Isrc.sys_inc \
		-Isrc.sdk \
		-Isrc.inc \
		-Isrc.dll.vd.vdtwinc \
		-I${RISCOS_BUILD_EXPORT}TCPIPLibs \
		-I$(RISCOS_BUILD_EXPORT) \
		-I$(RISCOS_BUILD_EXPORT)C \
		-i$(RISCOS_BUILD_EXPORT)CLib

ASINCLUDES=

# ----------------------------------------------------------------------------------------------------

COUNTRY=	UK
INSTALLDIR=	@
RISCOS_BUILD_EXPORT=C:
COMPONENT=	ICAClient
APPNAME=	!${COMPONENT}
ROMDIR=		^.WinFrameRO

# ----------------------------------------------------------------------------------------------------

# Buildable objects

CITRIXLIB=	src.lib.o.citrixlib
CITRIXLIBZ=	src.lib.oz.citrixlib
WENGINELIB=	src.dll.o.wenginelib
WENGINELIBZ=	src.dll.oz.wenginelib
TDLIB=		src.dll.o.tdlib
TDLIBZ=		src.dll.oz.tdlib
PDLIB=		src.dll.o.pdlib
PDLIBZ=		src.dll.oz.pdlib
WDLIB=		src.dll.o.wdlib
WDLIBZ=		src.dll.oz.wdlib
VDLIB=		src.dll.o.vdlib
VDLIBZ=		src.dll.oz.vdlib
NRLIB=		src.dll.o.nrlib
NRLIBZ=		src.dll.oz.nrlib
NELIB=		src.dll.o.nelib
NELIBZ=		src.dll.oz.nelib
SDLIB=		src.dll.o.sdlib
SDLIBZ=		src.dll.oz.sdlib
REDUCERLIB=	src.dll.o.reducerlib
REDUCERLIBZ=	src.dll.oz.reducerlib

APP=		abs.icaclient
APPZ=		aof.icaclient
APPRM=		rm.icaclient

# ----------------------------------------------------------------------------------------------------

COMMONLIBS=	$(RISCOS_BUILD_EXPORT)CLib.o.stubs \
		${RISCOS_BUILD_EXPORT}TCPIPLibs.o.inetlib \
		${RISCOS_BUILD_EXPORT}TCPIPLibs.o.socklib \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.wimplib \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.toolboxlib \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.eventlib

COMMONLIBSZ=	${RISCOS_BUILD_EXPORT}TCPIPLibs.o.inetlibzm \
		${RISCOS_BUILD_EXPORT}TCPIPLibs.o.socklibzm \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.wimplib \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.toolboxlib \
		$(RISCOS_BUILD_EXPORT)tboxlibs.o.eventlibm

APPLIBS=	${WENGINELIB}  ${WDLIB}  ${PDLIB}  ${TDLIB}  ${NRLIB} \
		${VDLIB} \
		${CITRIXLIB} \
		${REDUCERLIB}

APPLIBSZ=	${WENGINELIBZ} ${WDLIBZ} ${PDLIBZ} ${TDLIBZ} ${NRLIBZ} \
		${VDLIBZ} \
		${CITRIXLIBZ} \
		${REDUCERLIB}

BASELIBS=	${COMMONLIBS} \
		$(RISCOS_BUILD_EXPORT)MemLib.o.memlib

BASELIBSZ=	${COMMONLIBSZ} \
		$(RISCOS_BUILD_EXPORT)MemLib.o.memlibz \
		$(RISCOS_BUILD_EXPORT)RISC_OSLib.o.romcstubs

BASELIBSRM=	${COMMONLIBSZ} \
		$(RISCOS_BUILD_EXPORT)MemLib.o.memlibz \
		$(RISCOS_BUILD_EXPORT)CLib.o.stubs

# ----------------------------------------------------------------------------------------------------

# Rules

.SUFFIXES:	.c .s .o .oz .or

.c.o:;	@echo Compiling $<
	@$(CC) $< -o $@ -DROM=0 $(CCFLAGS)

.c.oz:;	@echo Compiling $< ROM
	@$(CC) $< -o $@ -zM -DROM=1 $(CCFLAGS)

.s.o:;	$(AS) $(ASFLAGS) -from $< -to $@

.s.oz:;	$(AS) $(ASFLAGS) -from $< -to $@

.s.or:;	$(AS) $(ASFLAGS) -from $< -to $@ -predefine "REMOVE_RAMLOAD_CHECK SETL {TRUE}"

# ----------------------------------------------------------------------------------------------------

# Top level targets

all:	${APP}

clean:	cleanlib cleandll cleanapp
	@echo cleaned all

dirs:	dirslib dirsdll dirsapp local_dirs dirs_rom
	@echo make all dirs

realclean: clean cleaninstall
	@echo realcleaned all

local_dirs: 
	${MKDIR} abs
	${MKDIR} aof
	${MKDIR} rm

install_base:
	${MKDIR} ${INSTALLDIR}.${APPNAME}
	${MKDIR} ${INSTALLDIR}.${APPNAME}.ini
	${CP} Resources.app.${COUNTRY}.!Boot	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS}
	${CP} Resources.app.${COUNTRY}.!Help	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.app.${COUNTRY}.!Run	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.app.${COUNTRY}.!Sprites	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.app.${COUNTRY}.EnsureTB	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.app.${COUNTRY}.Messages	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.app.${COUNTRY}.Splash	${INSTALLDIR}.${APPNAME}.* ${CPFLAGS} 
	${CP} Resources.ini.appsrv			${INSTALLDIR}.${APPNAME}.ini.AppSrv ${CPFLAGS} 
	${CP} Resources.ini.module			${INSTALLDIR}.${APPNAME}.ini.Module ${CPFLAGS} 
	${CP} Resources.ini.wfclient			${INSTALLDIR}.${APPNAME}.ini.WFClient ${CPFLAGS} 

install: ${APP} install_base
	${CP} ${APP}					${INSTALLDIR}.${APPNAME}.!RunImage ${CPFLAGS}
	restool merge ${INSTALLDIR}.${APPNAME}.Res Resources.app.${COUNTRY}.Res_base Resources.app.${COUNTRY}.Res_nc2
	@echo Installed ${COMPONENT}

installrm: install_base ${APPRM}
	${CP} Resources.app.${COUNTRY}.!RunRAM	${INSTALLDIR}.${APPNAME}.!Run		${CPFLAGS}
	${CP} ${APPRM}				${INSTALLDIR}.${APPNAME}.!RunImage	${CPFLAGS}
	restool merge ${INSTALLDIR}.${APPNAME}.Res Resources.app.${COUNTRY}.Res_base Resources.app.${COUNTRY}.Res_nc2
	@echo Installed ${COMPONENT} module

cleaninstall:
	${WIPE}	 ${APPNAME}.ini ${WIPEFLAGS}
	${WIPE}	 ${APPNAME} ${WIPEFLAGS}
	${RM}	 abs
	${RM}	 aof
	${RM}	 rm
	${RM}	 linked
	@echo Cleaned install

# ----------------------------------------------------------------------------------------------------

# Components

# LIBRARY

CITRIXLIBOBJS=	src.lib.cfgini.o.cfgini \
		src.lib.cfgload.o.cfgload \
		src.lib.clib.o.delay src.lib.clib.o.fileio src.lib.clib.o.loadstr src.lib.clib.o.machine \
		src.lib.clib.o.mem src.lib.clib.o.misc src.lib.clib.o.string src.lib.clib.o.upcall src.lib.clib.o.vdu \
		src.lib.encrypt.o.encrypt \
		src.lib.ini.o.iniapi src.lib.ini.o.helpers \
		src.lib.io.icabrows.o.icabrows src.lib.io.tcpro.o.tcpro \
		src.lib.log.o.logapi \
		src.lib.memini.o.memini \
		src.lib.riscos.dll.o.dll \
		src.lib.riscos.kbd.o.kbdapi \
		src.lib.riscos.mou.o.mouapi \
		src.lib.riscos.vio.o.viocfg src.lib.riscos.vio.o.viocur src.lib.riscos.vio.o.viomisc \
		src.lib.riscos.vio.o.vioread src.lib.riscos.vio.o.vioscrll src.lib.riscos.vio.o.viowrt \
		src.lib.riscos.vio.o.vioro src.lib.riscos.vio.o.viochars

CITRIXLIBOBJSZ=	src.lib.cfgini.oz.cfgini \
		src.lib.cfgload.oz.cfgload \
		src.lib.clib.oz.delay src.lib.clib.oz.fileio src.lib.clib.oz.loadstr src.lib.clib.oz.machine \
		src.lib.clib.oz.mem src.lib.clib.oz.misc src.lib.clib.oz.string src.lib.clib.oz.upcall src.lib.clib.oz.vdu \
		src.lib.encrypt.oz.encrypt \
		src.lib.ini.oz.iniapi src.lib.ini.oz.helpers \
		src.lib.io.icabrows.oz.icabrows src.lib.io.tcpro.oz.tcpro \
		src.lib.log.oz.logapi \
		src.lib.memini.oz.memini \
		src.lib.riscos.dll.oz.dll \
		src.lib.riscos.kbd.oz.kbdapi \
		src.lib.riscos.mou.oz.mouapi \
		src.lib.riscos.vio.oz.viocfg src.lib.riscos.vio.oz.viocur src.lib.riscos.vio.oz.viomisc \
		src.lib.riscos.vio.oz.vioread src.lib.riscos.vio.oz.vioscrll src.lib.riscos.vio.oz.viowrt \
		src.lib.riscos.vio.oz.vioro src.lib.riscos.vio.oz.viochars

src.lib.o.citrixlib: ${CITRIXLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIB} ${CITRIXLIBOBJS}

src.lib.oz.citrixlib: ${CITRIXLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIBZ} ${CITRIXLIBOBJSZ}

dirslib:	
	${MKDIR} src.lib.cfgini.o
	${MKDIR} src.lib.cfgload.o
	${MKDIR} src.lib.clib.o
	${MKDIR} src.lib.encrypt.o
	${MKDIR} src.lib.ini.o
	${MKDIR} src.lib.io.icabrows.o
	${MKDIR} src.lib.io.tcpro.o
	${MKDIR} src.lib.log.o
	${MKDIR} src.lib.memini.o
	${MKDIR} src.lib.riscos.dll.o
	${MKDIR} src.lib.riscos.kbd.o
	${MKDIR} src.lib.riscos.mou.o
	${MKDIR} src.lib.riscos.vio.o
	${MKDIR} src.lib.o
	${MKDIR} src.lib.cfgini.oz
	${MKDIR} src.lib.cfgload.oz
	${MKDIR} src.lib.clib.oz
	${MKDIR} src.lib.encrypt.oz
	${MKDIR} src.lib.ini.oz
	${MKDIR} src.lib.io.icabrows.oz
	${MKDIR} src.lib.io.tcpro.oz
	${MKDIR} src.lib.log.oz
	${MKDIR} src.lib.memini.oz
	${MKDIR} src.lib.riscos.dll.oz
	${MKDIR} src.lib.riscos.kbd.oz
	${MKDIR} src.lib.riscos.mou.oz
	${MKDIR} src.lib.riscos.vio.oz
	${MKDIR} src.lib.oz
	@echo created citrixlib dirs

cleanlib:
	${WIPE} src.lib.cfgini.o ${WIPEFLAGS}
	${WIPE} src.lib.cfgload.o ${WIPEFLAGS}
	${WIPE} src.lib.clib.o ${WIPEFLAGS}
	${WIPE} src.lib.encrypt.o ${WIPEFLAGS}
	${WIPE} src.lib.ini.o ${WIPEFLAGS}
	${WIPE} src.lib.io.icabrows.o ${WIPEFLAGS}
	${WIPE} src.lib.io.tcpro.o ${WIPEFLAGS}
	${WIPE} src.lib.log.o ${WIPEFLAGS}
	${WIPE} src.lib.memini.o ${WIPEFLAGS}
	${WIPE} src.lib.riscos.dll.o ${WIPEFLAGS}
	${WIPE} src.lib.riscos.kbd.o ${WIPEFLAGS}
	${WIPE} src.lib.riscos.mou.o ${WIPEFLAGS}
	${WIPE} src.lib.riscos.vio.o ${WIPEFLAGS}
	${WIPE} src.lib.o ${WIPEFLAGS}
	${WIPE} src.lib.cfgini.oz ${WIPEFLAGS}
	${WIPE} src.lib.cfgload.oz ${WIPEFLAGS}
	${WIPE} src.lib.clib.oz ${WIPEFLAGS}
	${WIPE} src.lib.encrypt.oz ${WIPEFLAGS}
	${WIPE} src.lib.ini.oz ${WIPEFLAGS}
	${WIPE} src.lib.io.icabrows.oz ${WIPEFLAGS}
	${WIPE} src.lib.io.tcpro.oz ${WIPEFLAGS}
	${WIPE} src.lib.log.oz ${WIPEFLAGS}
	${WIPE} src.lib.memini.oz ${WIPEFLAGS}
	${WIPE} src.lib.riscos.dll.oz ${WIPEFLAGS}
	${WIPE} src.lib.riscos.kbd.oz ${WIPEFLAGS}
	${WIPE} src.lib.riscos.mou.oz ${WIPEFLAGS}
	${WIPE} src.lib.riscos.vio.oz ${WIPEFLAGS}
	${WIPE} src.lib.oz ${WIPEFLAGS}
	@echo cleaned lib

src.lib.cfgini.o.cfgini src.lib.cfgini.oz.cfgini: src.lib.bini.c.biniguts src.lib.ini.c.helpers
src.lib.ini.o.iniapi src.lib.ini.oz.iniapi: src.lib.ini.c.helpers

# ----------------------------------------------------------------------------------------------------

# DLLs

# NR

NRLIBOBJS=	src.dll.nr.common.o.nrapi  src.dll.nr.nrtcpro.o.nrtcpro src.dll.nr.nrica.o.nrica
NRLIBOBJSZ=	src.dll.nr.common.oz.nrapi src.dll.nr.nrtcpro.oz.nrtcpro src.dll.nr.nrica.oz.nrica

src.dll.o.nrlib:	${NRLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIB} ${NRLIBOBJS}

src.dll.oz.nrlib:	${NRLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIBZ} ${NRLIBOBJSZ}

cleannr:
	${WIPE} src.dll.nr.common.o ${WIPEFLAGS}
	${WIPE} src.dll.nr.nrtcpro.o ${WIPEFLAGS}
	${WIPE} src.dll.nr.nrica.o ${WIPEFLAGS}
	${WIPE} src.dll.nr.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.nr.nrtcpro.oz ${WIPEFLAGS}
	${WIPE} src.dll.nr.nrica.oz ${WIPEFLAGS}
	@echo cleaned nr libs

dirsnr:
	${MKDIR} src.dll.nr.common.o
	${MKDIR} src.dll.nr.nrtcpro.o
	${MKDIR} src.dll.nr.nrica.o
	${MKDIR} src.dll.nr.common.oz
	${MKDIR} src.dll.nr.nrtcpro.oz
	${MKDIR} src.dll.nr.nrica.oz
	@echo created nr dirs

# NE

NELIBOBJS=	src.dll.ne.common.o.neapi  src.dll.ne.neica.o.neica
NELIBOBJSZ=	src.dll.ne.common.oz.neapi src.dll.ne.neica.oz.neica

src.dll.o.nelib:	${NELIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NELIB} ${NELIBOBJS}
src.dll.oz.nelib:	${NELIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NELIBZ} ${NELIBOBJSZ}

cleanne:
	${RM} ${NELIB}
	${RM} ${NELIBZ}
	${WIPE} src.dll.ne.common.o ${WIPEFLAGS}
	${WIPE} src.dll.ne.neica.o ${WIPEFLAGS}
	${WIPE} src.dll.ne.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.ne.neica.oz ${WIPEFLAGS}
	@echo cleaned ne libs

dirsne:
	${MKDIR} src.dll.ne.common.o
	${MKDIR} src.dll.ne.neica.o
	${MKDIR} src.dll.ne.common.oz
	${MKDIR} src.dll.ne.neica.oz
	@echo created ne dirs

# REDUCER

REDUCERLIBOBJS=	src.dll.reducer.o.icareduc   src.dll.reducer.o.reduce
REDUCERLIBOBJSZ=src.dll.reducer.oz.icareducz src.dll.reducer.oz.reduce

src.dll.o.reducerlib:	${REDUCERLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${REDUCERLIB} ${REDUCERLIBOBJS}
src.dll.oz.reducerlib:	${REDUCERLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${REDUCERLIBZ} ${REDUCERLIBOBJSZ}

dirsreducer:
	${MKDIR} src.dll.reducer.o
	${MKDIR} src.dll.reducer.oz
	@echo created reducer dirs

cleanreducer:
	${RM} ${REDUCERLIB}
	${RM} ${REDUCERLIBZ}
	${WIPE} src.dll.reducer.o  ${WIPEFLAGS}
	${WIPE} src.dll.reducer.oz ${WIPEFLAGS}
	@echo cleaned reducer libs

# WENGINE

WENGINELIBOBJS=	src.dll.wengine.o.dosonly src.dll.wengine.o.wengine \
		src.dll.wengine.o.worker

WENGINELIBOBJSZ=src.dll.wengine.oz.dosonly src.dll.wengine.oz.wengine \
		src.dll.wengine.oz.worker

src.dll.o.wenginelib: ${WENGINELIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIB} ${WENGINELIBOBJS}

src.dll.oz.wenginelib: ${WENGINELIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIBZ} ${WENGINELIBOBJSZ}

cleanwengine:
	${RM} ${WENGINELIB}
	${RM} ${WENGINELIBZ}
	${WIPE} src.dll.wengine.o ${WIPEFLAGS}
	${WIPE} src.dll.wengine.oz ${WIPEFLAGS}
	@echo cleaned wengine libs

dirswengine:
	${MKDIR} src.dll.wengine.o
	${MKDIR} src.dll.wengine.oz
	@echo created wengine dirs

# TD

TDCOMMONLIBOBJS=	src.dll.td.common.o.tdapi
TDCOMMONLIBOBJSZ=	src.dll.td.common.oz.tdapi

TDTCPROLIBOBJS=		src.dll.td.comtcp.o.tdtcpcom  src.dll.td.tdtcpro.o.tdtcpro  src.dll.td.tdtcpro.o.sock
TDTCPROLIBOBJSZ=	src.dll.td.comtcp.oz.tdtcpcom src.dll.td.tdtcpro.oz.tdtcpro src.dll.td.tdtcpro.oz.sock

TDASYNCLIBOBJS=		src.dll.td.tdasync.o.tdasync
TDASYNCLIBOBJSZ=	src.dll.td.tdasync.oz.tdasync

TDLIBOBJS=	${TDCOMMONLIBOBJS} ${TDTCPROLIBOBJS} ${TDASYNCLIBOBJS}
TDLIBOBJSZ=	${TDCOMMONLIBOBJSZ} ${TDTCPROLIBOBJSZ} ${TDASYNCLIBOBJSZ}

src.dll.o.tdlib:	${TDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIB} ${TDLIBOBJS}

src.dll.oz.tdlib:	${TDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIBZ} ${TDLIBOBJSZ}

cleantd:
	${RM} ${TDLIB}
	${RM} ${TDLIBZ}
	${WIPE} src.dll.td.common.o ${WIPEFLAGS}
	${WIPE} src.dll.td.comtcp.o ${WIPEFLAGS}
	${WIPE} src.dll.td.tdtcpro.o ${WIPEFLAGS}
	${WIPE} src.dll.td.tdasync.o ${WIPEFLAGS}
	${WIPE} src.dll.td.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.td.comtcp.oz ${WIPEFLAGS}
	${WIPE} src.dll.td.tdtcpro.oz ${WIPEFLAGS}
	${WIPE} src.dll.td.tdasync.oz ${WIPEFLAGS}
	@echo cleaned td libs

dirstd:
	${MKDIR} src.dll.td.common.o
	${MKDIR} src.dll.td.comtcp.o
	${MKDIR} src.dll.td.tdtcpro.o
	${MKDIR} src.dll.td.tdasync.o
	${MKDIR} src.dll.td.common.oz
	${MKDIR} src.dll.td.comtcp.oz
	${MKDIR} src.dll.td.tdtcpro.oz
	${MKDIR} src.dll.td.tdasync.oz
	@echo created td dirs

# PD

PDCOMMONLIBOBJS=src.dll.pd.common.o.pdapi src.dll.pd.common.o.buffer
PDCOMMONLIBOBJSZ=src.dll.pd.common.oz.pdapi src.dll.pd.common.oz.buffer

PDRFRAMELIBOBJS=src.dll.pd.pdrframe.o.pdrframe src.dll.pd.pdrframe.o.callup src.dll.pd.pdrframe.o.input
PDRFRAMELIBOBJSZ=src.dll.pd.pdrframe.oz.pdrframe src.dll.pd.pdrframe.oz.callup src.dll.pd.pdrframe.oz.input

PDFRAMELIBOBJS= src.dll.pd.pdframe.o.pdframe src.dll.pd.pdframe.o.callup src.dll.pd.pdframe.o.crc16 src.dll.pd.pdframe.o.input
PDFRAMELIBOBJSZ=src.dll.pd.pdframe.oz.pdframe src.dll.pd.pdframe.oz.callup src.dll.pd.pdframe.oz.crc16 src.dll.pd.pdframe.oz.input

PDCRYPT1LIBOBJS=src.dll.pd.pdcrypt1.o.pdcrypt src.dll.pd.pdcrypt1.o.callup src.dll.pd.pdcrypt1.o.input src.dll.pd.pdcrypt1.o.encrypt
PDCRYPT1LIBOBJSZ=src.dll.pd.pdcrypt1.oz.pdcrypt src.dll.pd.pdcrypt1.oz.callup src.dll.pd.pdcrypt1.oz.input src.dll.pd.pdcrypt1.oz.encrypt

PDRELILIBOBJS= src.dll.pd.pdreli.o.pdreli src.dll.pd.pdreli.o.callup src.dll.pd.pdreli.o.buffer src.dll.pd.pdreli.o.input src.dll.pd.pdreli.o.output src.dll.pd.pdreli.o.timers
PDRELILIBOBJSZ=src.dll.pd.pdreli.oz.pdreli src.dll.pd.pdreli.oz.callup src.dll.pd.pdreli.oz.buffer src.dll.pd.pdreli.oz.input src.dll.pd.pdreli.oz.output src.dll.pd.pdreli.oz.timers

PDLIBOBJS=	${PDCOMMONLIBOBJS} ${PDRFRAMELIBOBJS}  ${PDFRAMELIBOBJS} ${PDCRYPT1LIBOBJS} ${PDRELILIBOBJS}
PDLIBOBJSZ=	${PDCOMMONLIBOBJSZ} ${PDRFRAMELIBOBJSZ} ${PDFRAMELIBOBJSZ} ${PDCRYPT1LIBOBJSZ} ${PDRELILIBOBJSZ}

src.dll.o.pdlib:	${PDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIB} ${PDLIBOBJS}

src.dll.oz.pdlib:	${PDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIBZ} ${PDLIBOBJSZ}

cleanpd:
	${RM} ${PDLIB}
	${RM} ${PDLIBZ}
	${WIPE} src.dll.pd.common.o ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdrframe.o ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdframe.o ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdcrypt1.o ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdreli.o ${WIPEFLAGS}
	${WIPE} src.dll.pd.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdrframe.oz ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdframe.oz ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdcrypt1.oz ${WIPEFLAGS}
	${WIPE} src.dll.pd.pdreli.oz ${WIPEFLAGS}
	@echo cleaned pd libs

dirspd:
	${MKDIR} src.dll.pd.common.o
	${MKDIR} src.dll.pd.pdrframe.o
	${MKDIR} src.dll.pd.pdframe.o
	${MKDIR} src.dll.pd.pdcrypt1.o
	${MKDIR} src.dll.pd.pdreli.o
	${MKDIR} src.dll.pd.common.oz
	${MKDIR} src.dll.pd.pdrframe.oz
	${MKDIR} src.dll.pd.pdframe.oz
	${MKDIR} src.dll.pd.pdcrypt1.oz
	${MKDIR} src.dll.pd.pdreli.oz
	@echo created pd dirs

# WD

WDCOMMONLIBOBJS=	src.dll.wd.common.o.wdapi src.dll.wd.common.o.buffer
WDCOMMONLIBOBJSZ=	src.dll.wd.common.oz.wdapi src.dll.wd.common.oz.buffer

WDTTYLIBOBJS=		src.dll.wd.wdtty.o.wdtty src.dll.wd.wdtty.o.callup
WDTTYLIBOBJSZ=		src.dll.wd.wdtty.oz.wdtty src.dll.wd.wdtty.oz.callup

WDICA30LIBOBJS=		src.dll.wd.wdica30.o.callup src.dll.wd.wdica30.o.cursor src.dll.wd.wdica30.o.init \
			src.dll.wd.wdica30.o.input src.dll.wd.wdica30.o.misc src.dll.wd.wdica30.o.mode \
			src.dll.wd.wdica30.o.mouse src.dll.wd.wdica30.o.output src.dll.wd.wdica30.o.scroll \
			src.dll.wd.wdica30.o.text src.dll.wd.wdica30.o.virtual src.dll.wd.wdica30.o.wdica

WDICA30LIBOBJSZ=	src.dll.wd.wdica30.oz.callup src.dll.wd.wdica30.oz.cursor src.dll.wd.wdica30.oz.init \
			src.dll.wd.wdica30.oz.input src.dll.wd.wdica30.oz.misc src.dll.wd.wdica30.oz.mode \
			src.dll.wd.wdica30.oz.mouse src.dll.wd.wdica30.oz.output src.dll.wd.wdica30.oz.scroll \
			src.dll.wd.wdica30.oz.text src.dll.wd.wdica30.oz.virtual src.dll.wd.wdica30.oz.wdica


WDLIBOBJS=	${WDCOMMONLIBOBJS}  ${WDTTYLIBOBJS}  ${WDICA30LIBOBJS}
WDLIBOBJSZ=	${WDCOMMONLIBOBJSZ} ${WDTTYLIBOBJSZ} ${WDICA30LIBOBJSZ}

src.dll.o.wdlib:	${WDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIB} ${WDLIBOBJS}

src.dll.oz.wdlib:	${WDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIBZ} ${WDLIBOBJSZ}

cleanwd:
	${RM} ${WDLIB}
	${RM} ${WDLIBZ}
	${WIPE} src.dll.wd.common.o ${WIPEFLAGS}
	${WIPE} src.dll.wd.wdtty.o ${WIPEFLAGS}
	${WIPE} src.dll.wd.wdica30.o ${WIPEFLAGS}
	${WIPE} src.dll.wd.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.wd.wdtty.oz ${WIPEFLAGS}
	${WIPE} src.dll.wd.wdica30.oz ${WIPEFLAGS}
	@echo cleaned wd libs

dirswd:
	${MKDIR} src.dll.wd.common.o
	${MKDIR} src.dll.wd.wdtty.o
	${MKDIR} src.dll.wd.wdica30.o
	${MKDIR} src.dll.wd.common.oz
	${MKDIR} src.dll.wd.wdtty.oz
	${MKDIR} src.dll.wd.wdica30.oz
	@echo created wd dirs

# VD

# common files

VDCOMMONLIBOBJS	=src.dll.vd.common.o.vdapi
VDCOMMONLIBOBJSZ=src.dll.vd.common.oz.vdapi

# Thinwire driver

VDTW31LIBOBJS=	\
		src.dll.vd.vdtw31.o.palette \
		src.dll.vd.vdtw31.o.pixpack \
		src.dll.vd.vdtw31.o.tw \
		src.dll.vd.vdtw31.o.twbez \
		src.dll.vd.vdtw31.o.twcode \
		src.dll.vd.vdtw31.o.twdata \
		src.dll.vd.vdtw31.o.twro \
		src.dll.vd.vdtw31.o.twroot \
		src.dll.vd.vdtw31.o.twstroke \
		src.dll.vd.vdtw31.o.twtext \
		src.dll.vd.vdtw31.o.twvd \
		src.dll.vd.vdtw31.o.wfbitblt \
		src.dll.vd.vdtw31.o.wfbrush \
		src.dll.vd.vdtw31.o.wfcache \
		src.dll.vd.vdtw31.o.wfcursor \
		src.dll.vd.vdtw31.o.wfdims \
		src.dll.vd.vdtw31.o.wfglobal \
		src.dll.vd.vdtw31.o.wflines \
		src.dll.vd.vdtw31.o.wfmisc \
		src.dll.vd.vdtw31.o.wfsavscr \
		src.dll.vd.vdtw31.o.wfssb256 \
		src.dll.vd.vdtw31.o.wfssbtru \
		src.dll.vd.vdtw31.o.wfstrips \
		src.dll.vd.vdtw31.o.winonly

VDTW31LIBOBJSZ=	\
		src.dll.vd.vdtw31.oz.palette \
		src.dll.vd.vdtw31.oz.pixpack \
		src.dll.vd.vdtw31.oz.tw \
		src.dll.vd.vdtw31.oz.twbez \
		src.dll.vd.vdtw31.oz.twcode \
		src.dll.vd.vdtw31.oz.twdata \
		src.dll.vd.vdtw31.oz.twro \
		src.dll.vd.vdtw31.oz.twroot \
		src.dll.vd.vdtw31.oz.twstroke \
		src.dll.vd.vdtw31.oz.twtext \
		src.dll.vd.vdtw31.oz.twvd \
		src.dll.vd.vdtw31.oz.wfbitblt \
		src.dll.vd.vdtw31.oz.wfbrush \
		src.dll.vd.vdtw31.oz.wfcache \
		src.dll.vd.vdtw31.oz.wfcursor \
		src.dll.vd.vdtw31.oz.wfdims \
		src.dll.vd.vdtw31.oz.wfglobal \
		src.dll.vd.vdtw31.oz.wflines \
		src.dll.vd.vdtw31.oz.wfmisc \
		src.dll.vd.vdtw31.oz.wfsavscr \
		src.dll.vd.vdtw31.oz.wfssb256 \
		src.dll.vd.vdtw31.oz.wfssbtru \
		src.dll.vd.vdtw31.oz.wfstrips \
		src.dll.vd.vdtw31.oz.winonly

# Client drive mapping driver


VDCDMLIBOBJS=	src.dll.vd.vdcdm.o.cdmapi \
		src.dll.vd.vdcdm.o.cdmserv \
		src.dll.vd.vdcdm.o.cdmtrans \
		src.dll.vd.vdcdm.o.context \
		src.dll.vd.vdcdm.o.drive \
		src.dll.vd.vdcdm.o.ringbuf \
		src.dll.vd.vdcdm.o.vdcdm

VDCDMLIBOBJSZ=	src.dll.vd.vdcdm.oz.cdmapi \
		src.dll.vd.vdcdm.oz.cdmserv \
		src.dll.vd.vdcdm.oz.cdmtrans \
		src.dll.vd.vdcdm.oz.context \
		src.dll.vd.vdcdm.oz.drive \
		src.dll.vd.vdcdm.oz.ringbuf \
		src.dll.vd.vdcdm.oz.vdcdm

# Client printer driver (1.0)

VDCPMLIBOBJS=	src.dll.vd.vdcpm.common.o.cpmserv \
		src.dll.vd.vdcpm.common.o.cpmtrans \
		src.dll.vd.vdcpm.common.o.vdcpm \
		src.dll.vd.vdcpm.spool.o.rospool

VDCPMLIBOBJSZ=	src.dll.vd.vdcpm.common.oz.cpmserv \
		src.dll.vd.vdcpm.common.oz.cpmtrans \
		src.dll.vd.vdcpm.common.oz.vdcpm \
		src.dll.vd.vdcpm.spool.oz.rospool

# Client printer driver (1.5)

VDSPLLIBOBJS=	src.dll.vd.vdspl.common.o.cpmserv \
		src.dll.vd.vdspl.common.o.vdspl \
		src.dll.vd.vdspl.spool.o.rospool

VDSPLLIBOBJSZ=	src.dll.vd.vdspl.common.oz.cpmserv \
		src.dll.vd.vdspl.common.oz.vdspl \
		src.dll.vd.vdspl.spool.oz.rospool

# CLient audio driver

VDCAMLIBOBJS=	src.dll.vd.vdcam.common.o.camserv \
		src.dll.vd.vdcam.common.o.ctxadpcm \
	 	src.dll.vd.vdcam.common.o.formats \
		src.dll.vd.vdcam.common.o.lin_pcm \
		src.dll.vd.vdcam.common.o.memmgmt \
		src.dll.vd.vdcam.common.o.sendpkts \
		src.dll.vd.vdcam.common.o.statemch \
		src.dll.vd.vdcam.common.o.vdcam \
		src.dll.vd.vdcam.drivers.riscos.o.audhal

VDCAMLIBOBJSZ=	src.dll.vd.vdcam.common.o.camserv \
		src.dll.vd.vdcam.common.o.ctxadpcm \
	 	src.dll.vd.vdcam.common.o.formats \
		src.dll.vd.vdcam.common.o.lin_pcm \
		src.dll.vd.vdcam.common.o.memmgmt \
		src.dll.vd.vdcam.common.o.sendpkts \
		src.dll.vd.vdcam.common.o.statemch \
		src.dll.vd.vdcam.common.o.vdcam \
		src.dll.vd.vdcam.drivers.riscos.oz.audhal

VDLIBOBJS=	${VDCOMMONLIBOBJS} ${VDTW31LIBOBJS} ${VDCPMLIBOBJS} ${VDSPLLIBOBJS} ${VDCAMLIBOBJS}
VDLIBOBJSZ=	${VDCOMMONLIBOBJSZ} ${VDTW31LIBOBJSZ} ${VDCPMLIBOBJSZ} ${VDSPLLIBOBJSZ} ${VDCAMLIBOBJSZ}

src.dll.o.vdlib:	${VDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIB} ${VDLIBOBJS}

src.dll.oz.vdlib:	${VDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIBZ} ${VDLIBOBJSZ}

cleanvd:
	${RM} ${VDLIB}
	${RM} ${VDLIBZ}
	${WIPE} src.dll.vd.common.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcam.common.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcam.drivers.riscos.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcpm.common.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcpm.spool.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdspl.common.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdspl.spool.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdtw31.o ${WIPEFLAGS}
	${WIPE} src.dll.vd.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcam.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcam.drivers.riscos.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcpm.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdcpm.spool.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdspl.common.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdspl.spool.oz ${WIPEFLAGS}
	${WIPE} src.dll.vd.vdtw31.oz ${WIPEFLAGS}
	@echo cleaned vd libs

dirsvd:
	${MKDIR} src.dll.vd.common.o
	${MKDIR} src.dll.vd.vdcam.common.o
	${MKDIR} src.dll.vd.vdcam.drivers.riscos.o
	${MKDIR} src.dll.vd.vdcpm.common.o
	${MKDIR} src.dll.vd.vdcpm.spool.o
	${MKDIR} src.dll.vd.vdspl.common.o
	${MKDIR} src.dll.vd.vdspl.spool.o
	${MKDIR} src.dll.vd.vdtw31.o
	${MKDIR} src.dll.vd.common.oz
	${MKDIR} src.dll.vd.vdcam.common.oz
	${MKDIR} src.dll.vd.vdcam.drivers.riscos.oz
	${MKDIR} src.dll.vd.vdcpm.common.oz
	${MKDIR} src.dll.vd.vdcpm.spool.oz
	${MKDIR} src.dll.vd.vdspl.common.oz
	${MKDIR} src.dll.vd.vdspl.spool.oz
	${MKDIR} src.dll.vd.vdtw31.oz
	@echo created vd dirs

# generic dll

cleandll: cleanwengine cleantd cleanpd cleanwd cleanvd cleannr cleanne cleanreducer
	${WIPE} src.dll.o ${WIPEFLAGS}
	${WIPE} src.dll.oz ${WIPEFLAGS}
	@echo cleaned dll files

dirsdll: dirswengine dirstd dirspd dirswd dirsvd dirsnr dirsne dirsreducer
	${MKDIR} src.dll.o
	${MKDIR} src.dll.oz
	@echo created dll dirs

# ----------------------------------------------------------------------------------------------------

# APP

# The useful buildable objects

#  abs - Application build
#  aof - Image for ROM build
#  rm  = softload module build

MODULEWRAP_ROM=	src.app.oz.ModuleWrap
MODULEWRAP_RAM=	src.app.or.ModuleWrap

APPOBJS=	src.app.o.connect src.app.o.main \
		src.app.o.session src.app.o.utils src.app.o.server src.app.o.clientname

APPOBJSZ=	src.app.oz.connect src.app.oz.main \
		src.app.oz.session src.app.oz.utils src.app.oz.server src.app.oz.clientname

abs.icaclient: ${APPOBJS} ${APPLIBS} ${BASELIBS}
	${LD} ${LDFLAGS} -o ${APP} ${APPOBJS} ${APPLIBS} ${BASELIBS}

aof.icaclient: ${APPOBJSZ} ${MODULEWRAP_ROM} ${APPLIBSZ} ${BASELIBSZ} dirs
	${LD} -aof -o ${APPZ} ${APPOBJSZ} ${MODULEWRAP_ROM} ${APPLIBSZ} ${BASELIBSZ}

rm.icaclient: ${APPOBJSZ} ${MODULEWRAP_RAM} ${APPLIBSZ} ${BASELIBSRM}
	${LD} -rmf -o ${APPRM} ${APPOBJSZ} ${MODULEWRAP_RAM} ${APPLIBSZ} ${BASELIBSRM}

cleanapp:
	${RM} ${MODULEWRAP_RAM}
	${RM} ${MODULEWRAP_ROM}
	${RM} ${APP}
	${RM} ${APPZ}
	${RM} ${APPRM}
	${WIPE} src.app.o ${WIPEFLAGS}
	${WIPE} src.app.oz ${WIPEFLAGS}
	${WIPE} src.app.or ${WIPEFLAGS}
	@echo cleaned app files

dirsapp:
	${MKDIR} src.app.o
	${MKDIR} src.app.oz
	${MKDIR} src.app.or
	@echo created app dirs

# ----------------------------------------------------------------------------------------------------

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
