## Makefile for ICAClient project
# RISC OS client for Citrix ICA protocol
# SJM 17 Dec 1997

# ----------------------------------------------------------------------------------------------------

# Tools

CC=		armcc
AS=		armasm
LIBFILE=	armlib
LD=		armlink
CP=		cp
MKDIR=		mkdir
RM=		rm -f
RMDIR=		rmdir

# ----------------------------------------------------------------------------------------------------

# Flags
# disable warnings on non-ansi headers, narrowing casts and = in comparisons

CCFLAGS=	-c -fa -wanp ${INCLUDES} ${DFLAGS}
ASFLAGS=	-Stamp -quit ${ASINCLUDES}
LIBFILEFLAGS=	-c
CPFLAGS=	-p
LDFLAGS=

MCFLAGS=	-DMemCheck_MEMCHECK -zpc1 -fw

# ----------------------------------------------------------------------------------------------------

# -DINCL_ENUM -DINCL_MODEM -DINCL_SCRIPT -DINCL_PRINTER -DINCL_ASYNC -DINCL_TCPIP -DINCL_REDUCER -DINCL_AUDIO

DFLAGS=		-DRISCOS -DBETA=1 -DREMOTE_DEBUG -DCOMPAT_INET4 -DVESA=0 -DINCL_PRINTER -DINCL_TCPIP -DINCL_AUDIO
INCLUDES=	-Isrc/sys_inc \
		-Isrc/sdk \
		-Isrc/inc \
		-Isrc/dll/vd/vdtwinc \
		-I${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs \
		-I$(RISCOS_BUILD_EXPORT)/Lib \
		-I$(RISCOS_BUILD_EXPORT)/C \
		-J$(RISCOS_BUILD_EXPORT)/Lib/CLib

ASINCLUDES=	-I${RISCOS_BUILD_EXPORT}/Hdr/Global -I${RISCOS_BUILD_EXPORT}/Hdr/Interface -I${RISCOS_BUILD_EXPORT}/Hdr/Interface2 -I../..

# ----------------------------------------------------------------------------------------------------

COUNTRY=	UK
INSTALLDIR=	.
RISCOS_BUILD_EXPORT=../../../../Export
COMPONENT=	ICAClient
APPNAME=	!${COMPONENT}
ROMDIR=		../WinFrameRO

# ----------------------------------------------------------------------------------------------------

# Buildable objects

CITRIXLIB=	src/lib/citrixlib.o
CITRIXLIBD=	src/lib/citrixlib.od
CITRIXLIBZ=	src/lib/citrixlib.oz
WENGINELIB=	src/dll/wengine.o
WENGINELIBD=	src/dll/wengine.od
WENGINELIBZ=	src/dll/wengine.oz
TDLIB=		src/dll/tdlib.o
TDLIBD=		src/dll/tdlib.od
TDLIBZ=		src/dll/tdlib.oz
PDLIB=		src/dll/pdlib.o
PDLIBD=		src/dll/pdlib.od
PDLIBZ=		src/dll/pdlib.oz
WDLIB=		src/dll/wdlib.o
WDLIBD=		src/dll/wdlib.od
WDLIBZ=		src/dll/wdlib.oz
VDLIB=		src/dll/vdlib.o
VDLIBD=		src/dll/vdlib.od
VDLIBZ=		src/dll/vdlib.oz
NRLIB=		src/dll/nrlib.o
NRLIBD=		src/dll/nrlib.od
NRLIBZ=		src/dll/nrlib.oz
NELIB=		src/dll/nelib.o
NELIBD=		src/dll/nelib.od
NELIBZ=		src/dll/nelib.oz
SDLIB=		src/dll/sdlib.o
SDLIBD=		src/dll/sdlib.od
SDLIBZ=		src/dll/sdlib.oz
REDUCERLIB=	src/dll/reducerlib.o
REDUCERLIBD=	src/dll/reducerlib.od
REDUCERLIBZ=	src/dll/reducerlib.oz

APP=		abs/icaclient
APPD=		abs/icaclientd
APPZ=		aof/icaclient
APPRM=		rm/icaclient

# ----------------------------------------------------------------------------------------------------

COMMONLIBS=	$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
		${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/inetlib.o \
		${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/socklib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/wimplib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/toolboxlib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/eventlib.o

COMMONLIBSZ=	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/inetlibzm.o \
		${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/socklibzm.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/wimplib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/toolboxlib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/eventlibm.o

APPLIBS=	${WENGINELIB}  ${WDLIB}  ${PDLIB}  ${TDLIB}  ${NRLIB} ${NELIB}  ${SDLIB} \
		${VDLIB} \
		${CITRIXLIB} \
		${REDUCERLIB}

APPLIBSD=	${WENGINELIBD} ${WDLIBD} ${PDLIBD} ${TDLIBD} ${NRLIBD} ${NELIBD} ${SDLIBD} \
		${VDLIBD} \
		${CITRIXLIBD} \
		${REDUCERLIBD}

APPLIBSZ=	${WENGINELIBZ} ${WDLIBZ} ${PDLIBZ} ${TDLIBZ} ${NRLIBZ} ${NELIBZ} ${SDLIBZ} \
		${VDLIBZ} \
		${CITRIXLIBZ} \
		${REDUCERLIBZ}

BASELIBS=	${COMMONLIBS} \
		$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlib.o

BASELIBSD=	$(RISCOS_BUILD_EXPORT)/Lib/debug/lremote.o \
		${COMMONLIBS} \
		$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibd.o \
		$(RISCOS_BUILD_EXPORT)/Lib/MemCheck/MemCheck.o

BASELIBSZ=	${COMMONLIBSZ} \
		$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o \
		$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/romcstubs.o

BASELIBSRM=	${COMMONLIBSZ} \
		$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o \
		$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o

SRC=		${APPSRC} ${CITRIXLIBSRC} ${NRLIBSRC} ${TDLIBSRC} ${PDLIBSRC} ${WDLIBSRC} ${VDLIBSRC} ${REDUCERLIBSRC}

# ----------------------------------------------------------------------------------------------------

# Rules

.SUFFIXES:	.c .s .o .oz .od .or

.c.o:	@echo Compiling $<
	@$(CC) $< -o $@     -DROM=0             $(CCFLAGS)
#.c.o:;	$(CC) $<     -E -C -DROM=0             $(CCFLAGS) > $@

.c.od:  @echo Compiling $< for debug
	$(CC) $< -o $@     -DROM=0 -DDEBUG -Fn $(CCFLAGS)
#.c.od:;	$(CC) $< -o $@     -DROM=0 -DDEBUG -Fn $(CCFLAGS) ${MCFLAGS}

.c.oz:  @echo Compiling $< for ROM
	$(CC) $< -o $@ -zM -DROM=1             $(CCFLAGS)

.s.o:	@echo Assembling $<
	$(AS) $(ASFLAGS) -from $< -to $@

.s.od:	@echo Assembling $< for debug
	$(AS) $(ASFLAGS) -from $< -to $@ -Predefine "DEBUG SETL {TRUE}"

.s.oz:	@echo Assembling $< for ROM
	$(AS) $(ASFLAGS) -from $< -to $@

.s.or:	@echo Assembling $< for RAM module
	$(AS) $(ASFLAGS) -from $< -to $@ -predefine "REMOVE_RAMLOAD_CHECK SETL {TRUE}"

# ----------------------------------------------------------------------------------------------------

# Top level targets

all:	${APP}

clean:	cleanlib cleandll cleanapp
	@echo cleaned all

realclean: clean cleanmisc cleaninstall cleanmakepass cleanicaenum
	@echo realcleaned all

cleanmisc:
	${RM} TAGS
	${RM} srclist

tags:	srclist
	etags `cat srclist`

srclist:
	find . -name '*.[ch]' > srclist

refreshtags: cleanmisc srclist tags
	@echo refreshed tags

depends: ${SRC}
	armdepend -M Makefile $(CCFLAGS) -DDEBUG ${SRC}
	chmod a+w Makefile
	@echo ${COMPONENT} dependencies generated

touchsession:
	-${RM} src/app/session.o
	-${RM} src/app/session.oz
	-${RM} src/app/session.od
	@echo session object files removed

local_dirs: 
	@-${MKDIR} abs
	@-${MKDIR} aof
	@-${MKDIR} rm

install_base:
	@-${MKDIR} ${INSTALLDIR}/${APPNAME}
	@-${MKDIR} ${INSTALLDIR}/${APPNAME}/ini
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Boot,feb	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Help	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Run,feb	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Sprites,ff9	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/EnsureTB,feb	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Messages	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Splash,ff9	${INSTALLDIR}/${APPNAME}/
	@${CP} ${CPFLAGS} Resources/ini/appsrv			${INSTALLDIR}/${APPNAME}/ini/AppSrv
	@${CP} ${CPFLAGS} Resources/ini/module			${INSTALLDIR}/${APPNAME}/ini/Module
	@${CP} ${CPFLAGS} Resources/ini/wfclient		${INSTALLDIR}/${APPNAME}/ini/WFClient

install_res_desktop:
	@restool merge ${INSTALLDIR}/${APPNAME}/Res,fae Resources/app/${COUNTRY}/Res_base,fae Resources/app/${COUNTRY}/Res_desktop,fae
	@echo Installed Res for desktop

install_res_nc2:
	@restool merge ${INSTALLDIR}/${APPNAME}/Res,fae Resources/app/${COUNTRY}/Res_base,fae Resources/app/${COUNTRY}/Res_nc2,fae
	@echo Installed Res for nc2

install: ${APP} install_base install_res_desktop
	@${CP} ${CPFLAGS} ${APP}				${INSTALLDIR}/${APPNAME}/!RunImage,ff8
	@echo Installed ${COMPONENT}

installd: ${APPD} install_base install_res_desktop
	@${CP} ${CPFLAGS} ${APPD}				${INSTALLDIR}/${APPNAME}/!RunImage,ff8
	@restool merge ${INSTALLDIR}/${APPNAME}/Res,fae Resources/app/${COUNTRY}/Res_base,fae Resources/app/${COUNTRY}/Res_desktop,fae
	@echo Installed ${COMPONENT} debug

installrm: ${APPRM} install_base install_res_desktop
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!RunRAM,feb	${INSTALLDIR}/${APPNAME}/!Run,feb
	@${CP} ${CPFLAGS} ${APPRM}				${INSTALLDIR}/${APPNAME}/!RunImage,ffa
	@echo Installed ${COMPONENT} module

install_rom_dirs:
	@-${MKDIR} ${ROMDIR}
	@-${MKDIR} ${ROMDIR}/h
	@-${MKDIR} ${ROMDIR}/Resources/
	@-${MKDIR} ${ROMDIR}/Resources/${COUNTRY}
	@-${MKDIR} ${ROMDIR}/Resources/${COUNTRY}/ini
	@echo Installed ROM dirs

installrom: ${APPZ} install_rom_dirs
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!BootROM,feb	${ROMDIR}/Resources/${COUNTRY}/!Boot,feb
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Help	${ROMDIR}/Resources/${COUNTRY}
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!RunROM,feb	${ROMDIR}/Resources/${COUNTRY}/!Run,feb
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Sprites,ff9	${ROMDIR}/Resources/${COUNTRY}
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/EnsureTB,feb	${ROMDIR}/Resources/${COUNTRY}
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Messages	${ROMDIR}/Resources/${COUNTRY}
	@${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Splash,ff9	${ROMDIR}/Resources/${COUNTRY}
	@${CP} ${CPFLAGS} Resources/ini/appsrv			${ROMDIR}/Resources/${COUNTRY}/ini
	@${CP} ${CPFLAGS} Resources/ini/module			${ROMDIR}/Resources/${COUNTRY}/ini
	@${CP} ${CPFLAGS} Resources/ini/wfclient		${ROMDIR}/Resources/${COUNTRY}/ini
	@restool merge ${ROMDIR}/Resources/${COUNTRY}/Res,fae Resources/app/${COUNTRY}/Res_base,fae Resources/app/${COUNTRY}/Res_nc2,fae
	@${CP} ${CPFLAGS} ${APPZ}				${ROMDIR}/aof/${COMPONENT}
	@echo Installed ${COMPONENT} aof in ${ROMDIR}
	@${CP} ${CPFLAGS} src/app/icaclient.h			${ROMDIR}/h/icaclient

cleaninstall:
	-${RM}	  ${APPNAME}/ini/*
	-${RMDIR} ${APPNAME}/ini
	-${RM}	  ${APPNAME}/*
	-${RMDIR} ${APPNAME}
	@echo Cleaned install

installnc: install install_res_nc2
	@rcp -pr ${INSTALLDIR}/${APPNAME} 'bungle:/export/ftp/Custom/Apps'
	@echo Installed ${COMPONENT} on NC

installncd: installd
	@rcp -pr ${INSTALLDIR}/${APPNAME} 'bungle:/export/ftp/Custom/Apps'
	@echo Installed ${COMPONENT} on NC

# ----------------------------------------------------------------------------------------------------

makepass: makepass,ff8

makepass,ff8: src/app/makepass.o ${CITRIXLIB}
	${LD} ${LDFLAGS} -o 'makepass,ff8' src/app/makepass.o ${CITRIXLIB} $(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o
	@ls -l '$@'


cleanmakepass:
	@${RM} src/app/makepass.o 'makepass,ff8'
	@echo cleaned makepass

# ----------------------------------------------------------------------------------------------------

icaenum: icaenum,ff8
icaenumd: icaenumd,ff8

ICAENUMLIBS=${NELIB} ${CITRIXLIB} \
	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/inetlib.o \
	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/socklib.o

ICAENUMLIBSD=${NELIBD} ${CITRIXLIBD} \
	$(RISCOS_BUILD_EXPORT)/Lib/debug/lremote.o \
	$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibd.o \
	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/inetlib.o \
	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/socklib.o

icaenum,ff8: src/app/icaenum.o ${ICAENUMLIBS}
	${LD} ${LDFLAGS} -v -o 'icaenum,ff8' src/app/icaenum.o ${ICAENUMLIBS} $(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o
	@ls -l '$@'

icaenumd,ff8: src/app/icaenum.od ${ICAENUMLIBSD}
	${LD} ${LDFLAGS} -v -o 'icaenumd,ff8' src/app/icaenum.od ${ICAENUMLIBSD} $(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o
	@ls -l '$@'


cleanicaenum:
	@${RM} src/app/icaenum.o src/app/icaenum.od 'icaenum,ff8' 'icaenumd,ff8'
	@echo cleaned icaenum

# ----------------------------------------------------------------------------------------------------

# Components

# LIBRARY

CITRIXLIBSRC=	src/lib/cfgini/cfgini.c \
		src/lib/cfgload/cfgload.c \
		src/lib/clib/delay.c src/lib/clib/fileio.c src/lib/clib/loadstr.c src/lib/clib/machine.c \
		src/lib/clib/mem.c src/lib/clib/misc.c src/lib/clib/string.c src/lib/clib/upcall.s src/lib/clib/vdu.c \
		src/lib/encrypt/encrypt.c \
		src/lib/ini/iniapi.c src/lib/ini/helpers.c \
		src/lib/io/icabrows/icabrows.c src/lib/io/tcpro/tcpro.c \
		src/lib/log/logapi.c \
		src/lib/memini/memini.c \
		src/lib/riscos/dll/dll.c \
		src/lib/riscos/kbd/kbdapi.c \
		src/lib/riscos/mou/mouapi.c \
		src/lib/riscos/vio/viocfg.c src/lib/riscos/vio/viocur.c src/lib/riscos/vio/viomisc.c \
		src/lib/riscos/vio/vioread.c src/lib/riscos/vio/vioscrll.c src/lib/riscos/vio/viowrt.c \
		src/lib/riscos/vio/vioro.c src/lib/riscos/vio/viochars.c 

CITRIXLIBOBJS=	src/lib/cfgini/cfgini.o \
		src/lib/cfgload/cfgload.o \
		src/lib/clib/delay.o src/lib/clib/fileio.o src/lib/clib/loadstr.o src/lib/clib/machine.o \
		src/lib/clib/mem.o src/lib/clib/misc.o src/lib/clib/string.o src/lib/clib/upcall.o src/lib/clib/vdu.o \
		src/lib/encrypt/encrypt.o \
		src/lib/ini/iniapi.o src/lib/ini/helpers.o \
		src/lib/io/icabrows/icabrows.o src/lib/io/tcpro/tcpro.o \
		src/lib/log/logapi.o \
		src/lib/memini/memini.o \
		src/lib/riscos/dll/dll.o \
		src/lib/riscos/kbd/kbdapi.o \
		src/lib/riscos/mou/mouapi.o \
		src/lib/riscos/vio/viocfg.o src/lib/riscos/vio/viocur.o src/lib/riscos/vio/viomisc.o \
		src/lib/riscos/vio/vioread.o src/lib/riscos/vio/vioscrll.o src/lib/riscos/vio/viowrt.o \
		src/lib/riscos/vio/vioro.o src/lib/riscos/vio/viochars.o

CITRIXLIBOBJSD=	src/lib/cfgini/cfgini.od \
		src/lib/cfgload/cfgload.od \
		src/lib/clib/delay.od src/lib/clib/fileio.od src/lib/clib/loadstr.od src/lib/clib/machine.od \
		src/lib/clib/mem.od src/lib/clib/misc.od src/lib/clib/string.od src/lib/clib/upcall.od src/lib/clib/vdu.od \
		src/lib/encrypt/encrypt.od \
		src/lib/ini/iniapi.od src/lib/ini/helpers.od \
		src/lib/io/icabrows/icabrows.od src/lib/io/tcpro/tcpro.od \
		src/lib/log/logapi.od \
		src/lib/memini/memini.od  \
		src/lib/riscos/dll/dll.od \
		src/lib/riscos/kbd/kbdapi.od \
		src/lib/riscos/mou/mouapi.od \
		src/lib/riscos/vio/viocfg.od src/lib/riscos/vio/viocur.od src/lib/riscos/vio/viomisc.od \
		src/lib/riscos/vio/vioread.od src/lib/riscos/vio/vioscrll.od src/lib/riscos/vio/viowrt.od \
		src/lib/riscos/vio/vioro.od src/lib/riscos/vio/viochars.od


CITRIXLIBOBJSZ=	src/lib/cfgini/cfgini.oz \
		src/lib/cfgload/cfgload.oz \
		src/lib/clib/delay.oz src/lib/clib/fileio.oz src/lib/clib/loadstr.oz src/lib/clib/machine.oz \
		src/lib/clib/mem.oz src/lib/clib/misc.oz src/lib/clib/string.oz src/lib/clib/upcall.oz src/lib/clib/vdu.oz \
		src/lib/encrypt/encrypt.oz \
		src/lib/ini/iniapi.oz src/lib/ini/helpers.oz \
		src/lib/io/icabrows/icabrows.oz src/lib/io/tcpro/tcpro.oz \
		src/lib/log/logapi.oz \
		src/lib/memini/memini.oz \
		src/lib/riscos/dll/dll.oz \
		src/lib/riscos/kbd/kbdapi.oz \
		src/lib/riscos/mou/mouapi.oz \
		src/lib/riscos/vio/viocfg.oz src/lib/riscos/vio/viocur.oz src/lib/riscos/vio/viomisc.oz \
		src/lib/riscos/vio/vioread.oz src/lib/riscos/vio/vioscrll.oz src/lib/riscos/vio/viowrt.oz \
		src/lib/riscos/vio/vioro.oz src/lib/riscos/vio/viochars.oz

${CITRIXLIB}: ${CITRIXLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIB} ${CITRIXLIBOBJS}

${CITRIXLIBD}: ${CITRIXLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIBD} ${CITRIXLIBOBJSD}

${CITRIXLIBZ}: ${CITRIXLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIBZ} ${CITRIXLIBOBJSZ}

cleanlib:
	-${RM} ${CITRIXLIB}
	-${RM} ${CITRIXLIBOBJS}
	-${RM} ${CITRIXLIBD}
	-${RM} ${CITRIXLIBOBJSD}
	-${RM} ${CITRIXLIBZ}
	-${RM} ${CITRIXLIBOBJSZ}
	@echo cleaned lib

src/lib/cfgini/cfgini.o src/lib/cfgini/cfgini.od src/lib/cfgini/cfgini.oz: src/lib/bini/biniguts.c src/lib/ini/helpers.c
src/lib/ini/iniapi.o src/lib/ini/iniapi.od src/lib/ini/iniapi.oz: src/lib/ini/helpers.c

# ----------------------------------------------------------------------------------------------------

# DLLs

# NR

NRLIBSRC=	src/dll/nr/common/nrapi.c  src/dll/nr/nrtcpro/nrtcpro.c src/dll/nr/nrica/nrica.c
NRLIBOBJS=	src/dll/nr/common/nrapi.o  src/dll/nr/nrtcpro/nrtcpro.o src/dll/nr/nrica/nrica.o
NRLIBOBJSD=	src/dll/nr/common/nrapi.od src/dll/nr/nrtcpro/nrtcpro.od src/dll/nr/nrica/nrica.od
NRLIBOBJSZ=	src/dll/nr/common/nrapi.oz src/dll/nr/nrtcpro/nrtcpro.oz src/dll/nr/nrica/nrica.oz

${NRLIB}:	${NRLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIB} ${NRLIBOBJS}

${NRLIBD}:	${NRLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIBD} ${NRLIBOBJSD}

${NRLIBZ}:	${NRLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIBZ} ${NRLIBOBJSZ}

cleannr:
	-${RM} ${NRLIB}
	-${RM} ${NRLIBOBJS}
	-${RM} ${NRLIBD}
	-${RM} ${NRLIBOBJSD}
	-${RM} ${NRLIBZ}
	-${RM} ${NRLIBOBJSZ}
	@echo cleaned nr libs

# NE

NELIBSRC=	src/dll/ne/common/neapi.c  src/dll/ne/neica/neica.c
NELIBOBJS=	src/dll/ne/common/neapi.o  src/dll/ne/neica/neica.o
NELIBOBJSD=	src/dll/ne/common/neapi.od src/dll/ne/neica/neica.od
NELIBOBJSZ=	src/dll/ne/common/neapi.oz src/dll/ne/neica/neica.oz

${NELIB}:	${NELIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NELIB} ${NELIBOBJS}

${NELIBD}:	${NELIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NELIBD} ${NELIBOBJSD}
${NELIBZ}:	${NELIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NELIBZ} ${NELIBOBJSZ}

cleanne:
	-${RM} ${NELIB}
	-${RM} ${NELIBOBJS}
	-${RM} ${NELIBD}
	-${RM} ${NELIBOBJSD}
	-${RM} ${NELIBZ}
	-${RM} ${NELIBOBJSZ}
	@echo cleaned ne libs

# REDUCER

REDUCERLIBSRC=	src/dll/reducer/icareduc.c  src/dll/reducer/reduce.c
REDUCERLIBOBJS=	src/dll/reducer/icareduc.o  src/dll/reducer/reduce.o
REDUCERLIBOBJSD=src/dll/reducer/icareduc.od src/dll/reducer/reduce.od
REDUCERLIBOBJSZ=src/dll/reducer/icareduc.oz src/dll/reducer/reduce.oz

${REDUCERLIB}:	${REDUCERLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${REDUCERLIB} ${REDUCERLIBOBJS}

${REDUCERLIBD}:	${REDUCERLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${REDUCERLIBD} ${REDUCERLIBOBJSD}
${REDUCERLIBZ}:	${REDUCERLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${REDUCERLIBZ} ${REDUCERLIBOBJSZ}

cleanreducer:
	-${RM} ${REDUCERLIB}
	-${RM} ${REDUCERLIBOBJS}
	-${RM} ${REDUCERLIBD}
	-${RM} ${REDUCERLIBOBJSD}
	-${RM} ${REDUCERLIBZ}
	-${RM} ${REDUCERLIBOBJSZ}
	@echo cleaned reducer libs

# WENGINE

WENGINELIBSRC=	src/dll/wengine/dosonly.c src/dll/wengine/wengine.c \
		src/dll/wengine/worker.c

WENGINELIBOBJS=	src/dll/wengine/dosonly.o src/dll/wengine/wengine.o \
		src/dll/wengine/worker.o

WENGINELIBOBJSD=src/dll/wengine/dosonly.od src/dll/wengine/wengine.od \
		src/dll/wengine/worker.od

WENGINELIBOBJSZ=src/dll/wengine/dosonly.oz src/dll/wengine/wengine.oz \
		src/dll/wengine/worker.oz

${WENGINELIB}: ${WENGINELIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIB} ${WENGINELIBOBJS}

${WENGINELIBD}: ${WENGINELIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIBD} ${WENGINELIBOBJSD}

${WENGINELIBZ}: ${WENGINELIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIBZ} ${WENGINELIBOBJSZ}

cleanwengine:
	-${RM} ${WENGINELIB}
	-${RM} ${WENGINELIBOBJS}
	-${RM} ${WENGINELIBD}
	-${RM} ${WENGINELIBOBJSD}
	-${RM} ${WENGINELIBZ}
	-${RM} ${WENGINELIBOBJSZ}
	@echo cleaned wengine libs


# TD

TDCOMMONLIBSRC=		src/dll/td/common/tdapi.c
TDCOMMONLIBOBJS=	src/dll/td/common/tdapi.o
TDCOMMONLIBOBJSD=	src/dll/td/common/tdapi.od
TDCOMMONLIBOBJSZ=	src/dll/td/common/tdapi.oz

TDTCPROLIBSRC=		src/dll/td/comtcp/tdtcpcom.c  src/dll/td/tdtcpro/tdtcpro.c  src/dll/td/tdtcpro/sock.c
TDTCPROLIBOBJS=		src/dll/td/comtcp/tdtcpcom.o  src/dll/td/tdtcpro/tdtcpro.o  src/dll/td/tdtcpro/sock.o
TDTCPROLIBOBJSD=	src/dll/td/comtcp/tdtcpcom.od src/dll/td/tdtcpro/tdtcpro.od src/dll/td/tdtcpro/sock.od
TDTCPROLIBOBJSZ=	src/dll/td/comtcp/tdtcpcom.oz src/dll/td/tdtcpro/tdtcpro.oz src/dll/td/tdtcpro/sock.oz

TDASYNCLIBSRC=		src/dll/td/tdasync/tdasync.c src/dll/td/tdasync/roserial.c
TDASYNCLIBOBJS=		src/dll/td/tdasync/tdasync.o src/dll/td/tdasync/roserial.o
TDASYNCLIBOBJSD=	src/dll/td/tdasync/tdasync.od src/dll/td/tdasync/roserial.od
TDASYNCLIBOBJSZ=	src/dll/td/tdasync/tdasync.oz src/dll/td/tdasync/roserial.oz

TDLIBSRC=	${TDCOMMONLIBSRC} ${TDTCPROLIBSRC} ${TDASYNCLIBSRC}
TDLIBOBJS=	${TDCOMMONLIBOBJS} ${TDTCPROLIBOBJS} ${TDASYNCLIBOBJS}
TDLIBOBJSD=	${TDCOMMONLIBOBJSD} ${TDTCPROLIBOBJSD} ${TDASYNCLIBOBJSD}
TDLIBOBJSZ=	${TDCOMMONLIBOBJSZ} ${TDTCPROLIBOBJSZ} ${TDASYNCLIBOBJSZ}

${TDLIB}:	${TDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIB} ${TDLIBOBJS}

${TDLIBD}:	${TDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIBD} ${TDLIBOBJSD}

${TDLIBZ}:	${TDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIBZ} ${TDLIBOBJSZ}

cleantd:
	-${RM} ${TDLIB}
	-${RM} ${TDLIBOBJS}
	-${RM} ${TDLIBD}
	-${RM} ${TDLIBOBJSD}
	-${RM} ${TDLIBZ}
	-${RM} ${TDLIBOBJSZ}
	@echo cleaned td libs

# PD

PDCOMMONLIBSRC=	src/dll/pd/common/pdapi.c src/dll/pd/common/buffer.c
PDCOMMONLIBOBJS=src/dll/pd/common/pdapi.o src/dll/pd/common/buffer.o
PDCOMMONLIBOBJSD=src/dll/pd/common/pdapi.od src/dll/pd/common/buffer.od
PDCOMMONLIBOBJSZ=src/dll/pd/common/pdapi.oz src/dll/pd/common/buffer.oz

PDRFRAMELIBSRC=	src/dll/pd/pdrframe/pdrframe.c src/dll/pd/pdrframe/callup.c src/dll/pd/pdrframe/input.c
PDRFRAMELIBOBJS=src/dll/pd/pdrframe/pdrframe.o src/dll/pd/pdrframe/callup.o src/dll/pd/pdrframe/input.o
PDRFRAMELIBOBJSD=src/dll/pd/pdrframe/pdrframe.od src/dll/pd/pdrframe/callup.od src/dll/pd/pdrframe/input.od
PDRFRAMELIBOBJSZ=src/dll/pd/pdrframe/pdrframe.oz src/dll/pd/pdrframe/callup.oz src/dll/pd/pdrframe/input.oz

PDFRAMELIBSRC=	src/dll/pd/pdframe/pdframe.c src/dll/pd/pdframe/callup.c src/dll/pd/pdframe/crc16.c src/dll/pd/pdframe/input.c
PDFRAMELIBOBJS= src/dll/pd/pdframe/pdframe.o src/dll/pd/pdframe/callup.o src/dll/pd/pdframe/crc16.o src/dll/pd/pdframe/input.o
PDFRAMELIBOBJSD=src/dll/pd/pdframe/pdframe.od src/dll/pd/pdframe/callup.od src/dll/pd/pdframe/crc16.od src/dll/pd/pdframe/input.od
PDFRAMELIBOBJSZ=src/dll/pd/pdframe/pdframe.oz src/dll/pd/pdframe/callup.oz src/dll/pd/pdframe/crc16.oz src/dll/pd/pdframe/input.oz

PDCRYPT1LIBSRC=	src/dll/pd/pdcrypt1/pdcrypt.c src/dll/pd/pdcrypt1/callup.c src/dll/pd/pdcrypt1/input.c src/dll/pd/pdcrypt1/encrypt.c
PDCRYPT1LIBOBJS=src/dll/pd/pdcrypt1/pdcrypt.o src/dll/pd/pdcrypt1/callup.o src/dll/pd/pdcrypt1/input.o src/dll/pd/pdcrypt1/encrypt.o
PDCRYPT1LIBOBJSD=src/dll/pd/pdcrypt1/pdcrypt.od src/dll/pd/pdcrypt1/callup.od src/dll/pd/pdcrypt1/input.od src/dll/pd/pdcrypt1/encrypt.od
PDCRYPT1LIBOBJSZ=src/dll/pd/pdcrypt1/pdcrypt.oz src/dll/pd/pdcrypt1/callup.oz src/dll/pd/pdcrypt1/input.oz src/dll/pd/pdcrypt1/encrypt.oz

PDMODEMLIBSRC=	src/dll/pd/pdmodem/pdmodem.c src/dll/pd/pdmodem/callup.c src/dll/pd/pdmodem/input.c src/dll/pd/pdmodem/dsm.c src/dll/pd/pdmodem/util.c
PDMODEMLIBOBJS=	src/dll/pd/pdmodem/pdmodem.o src/dll/pd/pdmodem/callup.o src/dll/pd/pdmodem/input.o src/dll/pd/pdmodem/dsm.o src/dll/pd/pdmodem/util.o
PDMODEMLIBOBJSD=src/dll/pd/pdmodem/pdmodem.od src/dll/pd/pdmodem/callup.od src/dll/pd/pdmodem/input.od src/dll/pd/pdmodem/dsm.od src/dll/pd/pdmodem/util.od
PDMODEMLIBOBJSZ=src/dll/pd/pdmodem/pdmodem.oz src/dll/pd/pdmodem/callup.oz src/dll/pd/pdmodem/input.oz src/dll/pd/pdmodem/dsm.oz src/dll/pd/pdmodem/util.oz

PDRELILIBSRC=  src/dll/pd/pdreli/pdreli.c src/dll/pd/pdreli/callup.c src/dll/pd/pdreli/buffer.c src/dll/pd/pdreli/input.c src/dll/pd/pdreli/output.c src/dll/pd/pdreli/timers.c
PDRELILIBOBJS= src/dll/pd/pdreli/pdreli.o src/dll/pd/pdreli/callup.o src/dll/pd/pdreli/buffer.o src/dll/pd/pdreli/input.o src/dll/pd/pdreli/output.o src/dll/pd/pdreli/timers.o
PDRELILIBOBJSD=src/dll/pd/pdreli/pdreli.od src/dll/pd/pdreli/callup.od src/dll/pd/pdreli/buffer.od src/dll/pd/pdreli/input.od src/dll/pd/pdreli/output.od src/dll/pd/pdreli/timers.od
PDRELILIBOBJSZ=src/dll/pd/pdreli/pdreli.oz src/dll/pd/pdreli/callup.oz src/dll/pd/pdreli/buffer.oz src/dll/pd/pdreli/input.oz src/dll/pd/pdreli/output.oz src/dll/pd/pdreli/timers.oz

PDLIBSRC=	${PDCOMMONLIBSRC} ${PDRFRAMELIBSRC} ${PDFRAMELIBSRC} ${PDCRYPT1LIBSRC} ${PDMODEMLIBSRC} ${PDRELILIBSRC}
PDLIBOBJS=	${PDCOMMONLIBOBJS} ${PDRFRAMELIBOBJS}  ${PDFRAMELIBOBJS} ${PDCRYPT1LIBOBJS} ${PDMODEMLIBOBJS} ${PDRELILIBOBJS}
PDLIBOBJSD=	${PDCOMMONLIBOBJSD} ${PDRFRAMELIBOBJSD} ${PDFRAMELIBOBJSD} ${PDCRYPT1LIBOBJSD} ${PDMODEMLIBOBJSD} ${PDRELILIBOBJSD}
PDLIBOBJSZ=	${PDCOMMONLIBOBJSZ} ${PDRFRAMELIBOBJSZ} ${PDFRAMELIBOBJSZ} ${PDCRYPT1LIBOBJSZ} ${PDMODEMLIBOBJSZ} ${PDRELILIBOBJSZ}

${PDLIB}:	${PDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIB} ${PDLIBOBJS}

${PDLIBD}:	${PDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIBD} ${PDLIBOBJSD}

${PDLIBZ}:	${PDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIBZ} ${PDLIBOBJSZ}

cleanpd:
	-${RM} ${PDLIB}
	-${RM} ${PDLIBD}
	-${RM} ${PDLIBZ}
	-${RM} ${PDLIBOBJS}
	-${RM} ${PDLIBOBJSD}
	-${RM} ${PDLIBOBJSZ}
	@echo cleaned pd libs

# WD

WDCOMMONLIBSRC=		src/dll/wd/common/wdapi.c src/dll/wd/common/buffer.c
WDCOMMONLIBOBJS=	src/dll/wd/common/wdapi.o src/dll/wd/common/buffer.o
WDCOMMONLIBOBJSD=	src/dll/wd/common/wdapi.od src/dll/wd/common/buffer.od
WDCOMMONLIBOBJSZ=	src/dll/wd/common/wdapi.oz src/dll/wd/common/buffer.oz

WDTTYLIBSRC=		src/dll/wd/wdtty/wdtty.c src/dll/wd/wdtty/callup.c
WDTTYLIBOBJS=		src/dll/wd/wdtty/wdtty.o src/dll/wd/wdtty/callup.o
WDTTYLIBOBJSD=		src/dll/wd/wdtty/wdtty.od src/dll/wd/wdtty/callup.od
WDTTYLIBOBJSZ=		src/dll/wd/wdtty/wdtty.oz src/dll/wd/wdtty/callup.oz

WDICA30LIBSRC=		src/dll/wd/wdica30/callup.c src/dll/wd/wdica30/cursor.c src/dll/wd/wdica30/init.c \
			src/dll/wd/wdica30/input.c src/dll/wd/wdica30/misc.c src/dll/wd/wdica30/mode.c \
			src/dll/wd/wdica30/mouse.c src/dll/wd/wdica30/output.c src/dll/wd/wdica30/scroll.c \
			src/dll/wd/wdica30/text.c src/dll/wd/wdica30/virtual.c src/dll/wd/wdica30/wdica.c

WDICA30LIBOBJS=		src/dll/wd/wdica30/callup.o src/dll/wd/wdica30/cursor.o src/dll/wd/wdica30/init.o \
			src/dll/wd/wdica30/input.o src/dll/wd/wdica30/misc.o src/dll/wd/wdica30/mode.o \
			src/dll/wd/wdica30/mouse.o src/dll/wd/wdica30/output.o src/dll/wd/wdica30/scroll.o \
			src/dll/wd/wdica30/text.o src/dll/wd/wdica30/virtual.o src/dll/wd/wdica30/wdica.o

WDICA30LIBOBJSD=	src/dll/wd/wdica30/callup.od src/dll/wd/wdica30/cursor.od src/dll/wd/wdica30/init.od \
			src/dll/wd/wdica30/input.od src/dll/wd/wdica30/misc.od src/dll/wd/wdica30/mode.od \
			src/dll/wd/wdica30/mouse.od src/dll/wd/wdica30/output.od src/dll/wd/wdica30/scroll.od \
			src/dll/wd/wdica30/text.od src/dll/wd/wdica30/virtual.od src/dll/wd/wdica30/wdica.od

WDICA30LIBOBJSZ=	src/dll/wd/wdica30/callup.oz src/dll/wd/wdica30/cursor.oz src/dll/wd/wdica30/init.oz \
			src/dll/wd/wdica30/input.oz src/dll/wd/wdica30/misc.oz src/dll/wd/wdica30/mode.oz \
			src/dll/wd/wdica30/mouse.oz src/dll/wd/wdica30/output.oz src/dll/wd/wdica30/scroll.oz \
			src/dll/wd/wdica30/text.oz src/dll/wd/wdica30/virtual.oz src/dll/wd/wdica30/wdica.oz


WDLIBSRC=	${WDCOMMONLIBSRC}  ${WDTTYLIBSRC}  ${WDICA30LIBSRC}
WDLIBOBJS=	${WDCOMMONLIBOBJS}  ${WDTTYLIBOBJS}  ${WDICA30LIBOBJS}
WDLIBOBJSD=	${WDCOMMONLIBOBJSD} ${WDTTYLIBOBJSD} ${WDICA30LIBOBJSD}
WDLIBOBJSZ=	${WDCOMMONLIBOBJSZ} ${WDTTYLIBOBJSZ} ${WDICA30LIBOBJSZ}

${WDLIB}:	${WDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIB} ${WDLIBOBJS}

${WDLIBD}:	${WDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIBD} ${WDLIBOBJSD}

${WDLIBZ}:	${WDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIBZ} ${WDLIBOBJSZ}

cleanwd:
	-${RM} ${WDLIB}
	-${RM} ${WDLIBD}
	-${RM} ${WDLIBZ}
	-${RM} ${WDLIBOBJS}
	-${RM} ${WDLIBOBJSD}
	-${RM} ${WDLIBOBJSZ}
	@echo cleaned wd libs

# VD

# common files

VDCOMMONLIBSRC	=src/dll/vd/common/vdapi.c
VDCOMMONLIBOBJS	=src/dll/vd/common/vdapi.o
VDCOMMONLIBOBJSD=src/dll/vd/common/vdapi.od
VDCOMMONLIBOBJSZ=src/dll/vd/common/vdapi.oz

# Thinwire driver

VDTW31LIBSRC=	\
		src/dll/vd/vdtw31/palette.c \
		src/dll/vd/vdtw31/pixpack.c \
		src/dll/vd/vdtw31/tw.c \
		src/dll/vd/vdtw31/twbez.c \
		src/dll/vd/vdtw31/twcode.c \
		src/dll/vd/vdtw31/twdata.c \
		src/dll/vd/vdtw31/twro.c \
		src/dll/vd/vdtw31/twroot.c \
		src/dll/vd/vdtw31/twstroke.c \
		src/dll/vd/vdtw31/twtext.c \
		src/dll/vd/vdtw31/twvd.c \
		src/dll/vd/vdtw31/wfbitblt.c \
		src/dll/vd/vdtw31/wfbrush.c \
		src/dll/vd/vdtw31/wfcache.c \
		src/dll/vd/vdtw31/wfcursor.c \
		src/dll/vd/vdtw31/wfdims.c \
		src/dll/vd/vdtw31/wfglobal.c \
		src/dll/vd/vdtw31/wflines.c \
		src/dll/vd/vdtw31/wfmisc.c \
		src/dll/vd/vdtw31/wfsavscr.c \
		src/dll/vd/vdtw31/wfssb256.c \
		src/dll/vd/vdtw31/wfssbtru.c \
		src/dll/vd/vdtw31/wfstrips.c \
		src/dll/vd/vdtw31/winonly.c

VDTW31LIBOBJS=	\
		src/dll/vd/vdtw31/palette.o \
		src/dll/vd/vdtw31/pixpack.o \
		src/dll/vd/vdtw31/tw.o \
		src/dll/vd/vdtw31/twbez.o \
		src/dll/vd/vdtw31/twcode.o \
		src/dll/vd/vdtw31/twdata.o \
		src/dll/vd/vdtw31/twro.o \
		src/dll/vd/vdtw31/twroot.o \
		src/dll/vd/vdtw31/twstroke.o \
		src/dll/vd/vdtw31/twtext.o \
		src/dll/vd/vdtw31/twvd.o \
		src/dll/vd/vdtw31/wfbitblt.o \
		src/dll/vd/vdtw31/wfbrush.o \
		src/dll/vd/vdtw31/wfcache.o \
		src/dll/vd/vdtw31/wfcursor.o \
		src/dll/vd/vdtw31/wfdims.o \
		src/dll/vd/vdtw31/wfglobal.o \
		src/dll/vd/vdtw31/wflines.o \
		src/dll/vd/vdtw31/wfmisc.o \
		src/dll/vd/vdtw31/wfsavscr.o \
		src/dll/vd/vdtw31/wfssb256.o \
		src/dll/vd/vdtw31/wfssbtru.o \
		src/dll/vd/vdtw31/wfstrips.o \
		src/dll/vd/vdtw31/winonly.o

VDTW31LIBOBJSD=	\
		src/dll/vd/vdtw31/palette.od \
		src/dll/vd/vdtw31/pixpack.od \
		src/dll/vd/vdtw31/tw.od \
		src/dll/vd/vdtw31/twbez.od \
		src/dll/vd/vdtw31/twcode.od \
		src/dll/vd/vdtw31/twdata.od \
		src/dll/vd/vdtw31/twro.od \
		src/dll/vd/vdtw31/twroot.od \
		src/dll/vd/vdtw31/twstroke.od \
		src/dll/vd/vdtw31/twtext.od \
		src/dll/vd/vdtw31/twvd.od \
		src/dll/vd/vdtw31/wfbitblt.od \
		src/dll/vd/vdtw31/wfbrush.od \
		src/dll/vd/vdtw31/wfcache.od \
		src/dll/vd/vdtw31/wfcursor.od \
		src/dll/vd/vdtw31/wfdims.od \
		src/dll/vd/vdtw31/wfglobal.od \
		src/dll/vd/vdtw31/wflines.od \
		src/dll/vd/vdtw31/wfmisc.od \
		src/dll/vd/vdtw31/wfsavscr.od \
		src/dll/vd/vdtw31/wfssb256.od \
		src/dll/vd/vdtw31/wfssbtru.od \
		src/dll/vd/vdtw31/wfstrips.od \
		src/dll/vd/vdtw31/winonly.od

VDTW31LIBOBJSZ=	\
		src/dll/vd/vdtw31/palette.oz \
		src/dll/vd/vdtw31/pixpack.oz \
		src/dll/vd/vdtw31/tw.oz \
		src/dll/vd/vdtw31/twbez.oz \
		src/dll/vd/vdtw31/twcode.oz \
		src/dll/vd/vdtw31/twdata.oz \
		src/dll/vd/vdtw31/twro.oz \
		src/dll/vd/vdtw31/twroot.oz \
		src/dll/vd/vdtw31/twstroke.oz \
		src/dll/vd/vdtw31/twtext.oz \
		src/dll/vd/vdtw31/twvd.oz \
		src/dll/vd/vdtw31/wfbitblt.oz \
		src/dll/vd/vdtw31/wfbrush.oz \
		src/dll/vd/vdtw31/wfcache.oz \
		src/dll/vd/vdtw31/wfcursor.oz \
		src/dll/vd/vdtw31/wfdims.oz \
		src/dll/vd/vdtw31/wfglobal.oz \
		src/dll/vd/vdtw31/wflines.oz \
		src/dll/vd/vdtw31/wfmisc.oz \
		src/dll/vd/vdtw31/wfsavscr.oz \
		src/dll/vd/vdtw31/wfssb256.oz \
		src/dll/vd/vdtw31/wfssbtru.oz \
		src/dll/vd/vdtw31/wfstrips.oz \
		src/dll/vd/vdtw31/winonly.oz

# Client drive mapping driver

VDCDMLIBSRC=	src/dll/vd/vdcdm/cdmapi.c \
		src/dll/vd/vdcdm/cdmserv.c \
		src/dll/vd/vdcdm/cdmtrans.c \
		src/dll/vd/vdcdm/context.c \
		src/dll/vd/vdcdm/drive.c \
		src/dll/vd/vdcdm/ringbuf.c \
		src/dll/vd/vdcdm/vdcdm.c

VDCDMLIBOBJS=	src/dll/vd/vdcdm/cdmapi.o \
		src/dll/vd/vdcdm/cdmserv.o \
		src/dll/vd/vdcdm/cdmtrans.o \
		src/dll/vd/vdcdm/context.o \
		src/dll/vd/vdcdm/drive.o \
		src/dll/vd/vdcdm/ringbuf.o \
		src/dll/vd/vdcdm/vdcdm.o

VDCDMLIBOBJSD=	src/dll/vd/vdcdm/cdmapi.od \
		src/dll/vd/vdcdm/cdmserv.od \
		src/dll/vd/vdcdm/cdmtrans.od \
		src/dll/vd/vdcdm/context.od \
		src/dll/vd/vdcdm/drive.od \
		src/dll/vd/vdcdm/ringbuf.od \
		src/dll/vd/vdcdm/vdcdm.od

VDCDMLIBOBJSZ=	src/dll/vd/vdcdm/cdmapi.oz \
		src/dll/vd/vdcdm/cdmserv.oz \
		src/dll/vd/vdcdm/cdmtrans.oz \
		src/dll/vd/vdcdm/context.oz \
		src/dll/vd/vdcdm/drive.oz \
		src/dll/vd/vdcdm/ringbuf.oz \
		src/dll/vd/vdcdm/vdcdm.oz

# Client printer driver (1.0)

VDCPMLIBSRC=	src/dll/vd/vdcpm/common/cpmserv.c \
		src/dll/vd/vdcpm/common/cpmtrans.c \
		src/dll/vd/vdcpm/common/vdcpm.c \
		src/dll/vd/vdcpm/spool/rospool.c

VDCPMLIBOBJS=	src/dll/vd/vdcpm/common/cpmserv.o \
		src/dll/vd/vdcpm/common/cpmtrans.o \
		src/dll/vd/vdcpm/common/vdcpm.o \
		src/dll/vd/vdcpm/spool/rospool.o

VDCPMLIBOBJSD=	src/dll/vd/vdcpm/common/cpmserv.od \
		src/dll/vd/vdcpm/common/cpmtrans.od \
		src/dll/vd/vdcpm/common/vdcpm.od \
		src/dll/vd/vdcpm/spool/rospool.od

VDCPMLIBOBJSZ=	src/dll/vd/vdcpm/common/cpmserv.oz \
		src/dll/vd/vdcpm/common/cpmtrans.oz \
		src/dll/vd/vdcpm/common/vdcpm.oz \
		src/dll/vd/vdcpm/spool/rospool.oz

# Client printer driver (1.5)

VDSPLLIBSRC=	src/dll/vd/vdspl/common/cpmserv.c \
		src/dll/vd/vdspl/common/vdspl.c \
		src/dll/vd/vdspl/spool/rospool.c

VDSPLLIBOBJS=	src/dll/vd/vdspl/common/cpmserv.o \
		src/dll/vd/vdspl/common/vdspl.o \
		src/dll/vd/vdspl/spool/rospool.o

VDSPLLIBOBJSD=	src/dll/vd/vdspl/common/cpmserv.od \
		src/dll/vd/vdspl/common/vdspl.od \
		src/dll/vd/vdspl/spool/rospool.od

VDSPLLIBOBJSZ=	src/dll/vd/vdspl/common/cpmserv.oz \
		src/dll/vd/vdspl/common/vdspl.oz \
		src/dll/vd/vdspl/spool/rospool.oz

# CLient audio driver

VDCAMLIBSRC=	src/dll/vd/vdcam/common/camserv.c \
		src/dll/vd/vdcam/common/ctxadpcm.c \
	 	src/dll/vd/vdcam/common/formats.c \
		src/dll/vd/vdcam/common/lin_pcm.c \
		src/dll/vd/vdcam/common/memmgmt.c \
		src/dll/vd/vdcam/common/sendpkts.c \
		src/dll/vd/vdcam/common/statemch.c \
		src/dll/vd/vdcam/common/vdcam.c \
		src/dll/vd/vdcam/drivers/riscos/audhal.c

VDCAMLIBOBJS=	src/dll/vd/vdcam/common/camserv.o \
		src/dll/vd/vdcam/common/ctxadpcm.o \
	 	src/dll/vd/vdcam/common/formats.o \
		src/dll/vd/vdcam/common/lin_pcm.o \
		src/dll/vd/vdcam/common/memmgmt.o \
		src/dll/vd/vdcam/common/sendpkts.o \
		src/dll/vd/vdcam/common/statemch.o \
		src/dll/vd/vdcam/common/vdcam.o \
		src/dll/vd/vdcam/drivers/riscos/audhal.o

VDCAMLIBOBJSD=	src/dll/vd/vdcam/common/camserv.od \
		src/dll/vd/vdcam/common/ctxadpcm.od \
	 	src/dll/vd/vdcam/common/formats.od \
		src/dll/vd/vdcam/common/lin_pcm.od \
		src/dll/vd/vdcam/common/memmgmt.od \
		src/dll/vd/vdcam/common/sendpkts.od \
		src/dll/vd/vdcam/common/statemch.od \
		src/dll/vd/vdcam/common/vdcam.od \
		src/dll/vd/vdcam/drivers/riscos/audhal.od

VDCAMLIBOBJSZ=	src/dll/vd/vdcam/common/camserv.oz \
		src/dll/vd/vdcam/common/ctxadpcm.oz \
	 	src/dll/vd/vdcam/common/formats.oz \
		src/dll/vd/vdcam/common/lin_pcm.oz \
		src/dll/vd/vdcam/common/memmgmt.oz \
		src/dll/vd/vdcam/common/sendpkts.oz \
		src/dll/vd/vdcam/common/statemch.oz \
		src/dll/vd/vdcam/common/vdcam.oz \
		src/dll/vd/vdcam/drivers/riscos/audhal.oz


VDLIBSRC=	${VDCOMMONLIBSRC} ${VDTW31LIBSRC} ${VDCPMLIBSRC} ${VDSPLLIBSRC}
VDLIBOBJS=	${VDCOMMONLIBOBJS} ${VDTW31LIBOBJS} ${VDCPMLIBOBJS} ${VDSPLLIBOBJS}
VDLIBOBJSD=	${VDCOMMONLIBOBJSD} ${VDTW31LIBOBJSD} ${VDCPMLIBOBJSD} ${VDSPLLIBOBJSD} ${VDCAMLIBOBJSD}
VDLIBOBJSZ=	${VDCOMMONLIBOBJSZ} ${VDTW31LIBOBJSZ} ${VDCPMLIBOBJSZ} ${VDSPLLIBOBJSZ}

${VDLIB}:	${VDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIB} ${VDLIBOBJS}

${VDLIBD}:	${VDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIBD} ${VDLIBOBJSD}

${VDLIBZ}:	${VDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIBZ} ${VDLIBOBJSZ}

cleanvd:
	-${RM} ${VDLIB}
	-${RM} ${VDLIBD}
	-${RM} ${VDLIBZ}
	-${RM} ${VDLIBOBJS}
	-${RM} ${VDLIBOBJSD}
	-${RM} ${VDLIBOBJSZ}
	@echo cleaned vd libs

# SD

SDLIBSRC=	src/dll/sd/common/sdapi.c  \
		src/dll/sd/script/input.c src/dll/sd/script/parse.c src/dll/sd/script/script.c \
		src/dll/sd/script/send.c src/dll/sd/script/set.c src/dll/sd/script/wait.c

SDLIBOBJS=	src/dll/sd/common/sdapi.o \
		src/dll/sd/script/input.o src/dll/sd/script/parse.o src/dll/sd/script/script.o \
		src/dll/sd/script/send.o src/dll/sd/script/set.o src/dll/sd/script/wait.o

SDLIBOBJSD=	src/dll/sd/common/sdapi.od \
		src/dll/sd/script/input.od src/dll/sd/script/parse.od src/dll/sd/script/script.od \
		src/dll/sd/script/send.od src/dll/sd/script/set.od src/dll/sd/script/wait.od

SDLIBOBJSZ=	src/dll/sd/common/sdapi.oz \
		src/dll/sd/script/input.oz src/dll/sd/script/parse.oz src/dll/sd/script/script.oz \
		src/dll/sd/script/send.oz src/dll/sd/script/set.oz src/dll/sd/script/wait.oz

${SDLIB}:	${SDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${SDLIB} ${SDLIBOBJS}

${SDLIBD}:	${SDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${SDLIBD} ${SDLIBOBJSD}

${SDLIBZ}:	${SDLIBOBJSZ}
	${LIBFILE} ${LIBFILEFLAGS} -o ${SDLIBZ} ${SDLIBOBJSZ}

cleansd:
	-${RM} ${SDLIB}
	-${RM} ${SDLIBOBJS}
	-${RM} ${SDLIBD}
	-${RM} ${SDLIBOBJSD}
	-${RM} ${SDLIBZ}
	-${RM} ${SDLIBOBJSZ}
	@echo cleaned sd libs

# generic dll

cleandll: cleanwengine cleantd cleanpd cleanwd cleanvd cleannr cleanne cleansd cleanreducer
	@echo cleaned dll files

# ----------------------------------------------------------------------------------------------------

# APP

MODULEWRAP_ROM=	src/app/ModuleWrap.oz
MODULEWRAP_RAM=	src/app/ModuleWrap.or

APPSRC=		src/app/connect.c src/app/main.c \
		src/app/session.c src/app/utils.c src/app/server.c src/app/clientname.c

APPOBJS=	src/app/connect.o src/app/main.o \
		src/app/session.o src/app/utils.o src/app/server.o src/app/clientname.o

APPOBJSD=	src/app/connect.od src/app/main.od \
		src/app/session.od src/app/utils.od src/app/server.od src/app/clientname.od

APPOBJSZ=	src/app/connect.oz src/app/main.oz \
		src/app/session.oz src/app/utils.oz src/app/server.oz src/app/clientname.oz

${APP}: ${APPOBJS} ${APPLIBS} ${BASELIBS} local_dirs
	${LD} ${LDFLAGS} -o ${APP} ${APPOBJS} ${APPLIBS} ${BASELIBS}
	ls -l $@

${APPD}: ${APPOBJSD} ${APPLIBSD} ${BASELIBSD} local_dirs
	${LD} ${LDFLAGS} -o ${APPD} ${APPOBJSD} ${APPLIBSD} ${BASELIBSD}
	ls -l $@

${APPZ}: ${APPOBJSZ} ${MODULEWRAP_ROM} ${APPLIBSZ} ${BASELIBSZ} local_dirs
	${LD} -aof -o ${APPZ} ${APPOBJSZ} ${MODULEWRAP_ROM} ${APPLIBSZ} ${BASELIBSZ}
	ls -l $@

${APPRM}: ${APPOBJSZ} ${MODULEWRAP_RAM} ${APPLIBSZ} ${BASELIBSRM} local_dirs
	@echo Current armlink cannot generate relocatable modules correctly. Use RISC OS instead.
	@echo ${LD} -rmf -o ${APPRM} ${APPOBJSZ} ${MODULEWRAP_RAM} ${APPLIBSZ} ${BASELIBSRM}
	@echo ls -l $@

cleanapp:
	-${RM} ${APPOBJS}
	-${RM} ${APPOBJSD}
	-${RM} ${APPOBJSZ}
	-${RM} ${MODULEWRAP_RAM}
	-${RM} ${MODULEWRAP_ROM}
	-${RM} ${APP}
	-${RM} ${APPD}
	-${RM} ${APPZ}
	-${RM} ${APPRM}
	@echo cleaned app files

# ----------------------------------------------------------------------------------------------------

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
# DO NOT DELETE THIS LINE - armdepend dependencies follow

