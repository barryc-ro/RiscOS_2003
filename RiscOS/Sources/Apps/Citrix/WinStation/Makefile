# Makefile for WSClient project
# RISC OS client for Citrix WinStation protocol
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

CCFLAGS=	-c -fa -Fn ${INCLUDES} ${DFLAGS}
ASFLAGS=	-Stamp -quit
LIBFILEFLAGS=	-c
CPFLAGS=	-p

# ----------------------------------------------------------------------------------------------------

DFLAGS=		-DRISCOS -DBETA=1 -DREMOTE_DEBUG
INCLUDES=	-Isrc/sys_inc \
		-Isrc/sdk \
		-I${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs \
		-I$(RISCOS_BUILD_EXPORT)/Lib \
		-I$(RISCOS_BUILD_EXPORT)/C \
		-J$(RISCOS_BUILD_EXPORT)/Lib/CLib

# ----------------------------------------------------------------------------------------------------

COUNTRY=	UK
INSTALLDIR=	.
RISCOS_BUILD_EXPORT=../../../../Export
APPNAME=	!WSClient

# ----------------------------------------------------------------------------------------------------

# Buildable objects

CITRIXLIB=	src/lib/citrixlib.o
CITRIXLIBD=	src/lib/citrixlib.od
WENGINELIB=	src/dll/wengine.o
WENGINELIBD=	src/dll/wengine.od
TDLIB=		src/dll/tdlib.o
TDLIBD=		src/dll/tdlib.od
PDLIB=		src/dll/pdlib.o
PDLIBD=		src/dll/pdlib.od
WDLIB=		src/dll/wdlib.o
WDLIBD=		src/dll/wdlib.od
VDLIB=		src/dll/vdlib.o
VDLIBD=		src/dll/vdlib.od
NRLIB=		src/dll/nrlib.o
NRLIBD=		src/dll/nrlib.od

APP=		abs/wsclient
APPD=		abs/wsclientd

# ----------------------------------------------------------------------------------------------------

COMMONLIBS=	${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/inetlib.o \
		${RISCOS_BUILD_EXPORT}/Lib/TCPIPLibs/socklib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/wimplib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/toolboxlib.o \
		$(RISCOS_BUILD_EXPORT)/Lib/tboxlibs/eventlib.o

APPLIBS=	${WENGINELIB}  ${WDLIB}  ${TDLIB}  ${NRLIB}  ${CITRIXLIB}  
APPLIBSD=	${WENGINELIBD} ${WDLIBD} ${TDLIBD} ${NRLIBD} ${CITRIXLIBD}

BASELIBS=	${COMMONLIBS}

BASELIBSD=	$(RISCOS_BUILD_EXPORT)/Lib/debug/lremote.o \
		${COMMONLIBS}

SRC=		${APPSRC} ${CITRIXLIBSRC} ${NRLIBSRC} ${TDLIBSRC} ${WDLIBSRC}

# ----------------------------------------------------------------------------------------------------

# Rules

.SUFFIXES:	.c .s .o .oz .od

.c.o:;	$(CC) $< -o $@     -DROM=0             $(CCFLAGS)

.c.od:;	$(CC) $< -o $@     -DROM=0 -DDEBUG -Fn $(CCFLAGS)

.c.oz:;	$(CC) $< -o $@ -zM -DROM=1             $(CCFLAGS)

.s.o:;	$(AS) $(ASFLAGS) -from $< -to $@

# ----------------------------------------------------------------------------------------------------

# Top level targets

all:	${WENGINELIB}

clean:	cleanlib cleandll cleanapp
	echo cleaned all

realclean: clean cleanmisc cleaninstall
	echo realcleaned all

cleanmisc:
	${RM} TAGS
	${RM} srclist

tags:	srclist
	etags `cat srclist`

srclist:
	find . -name '*.[ch]' > srclist

refreshtags: cleanmisc srclist tags
	echo refreshed tags

depends: ${SRC}
	armdepend -M Makefile $(CCFLAGS) ${SRC}
	chmod a+w Makefile
	@echo ${APPNAME} dependencies generated

install:
	-${MKDIR} ${INSTALLDIR}/${APPNAME}
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Boot,feb	${INSTALLDIR}/${APPNAME}/
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Run,feb	${INSTALLDIR}/${APPNAME}/
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/!Sprites,ff9	${INSTALLDIR}/${APPNAME}/
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Messages	${INSTALLDIR}/${APPNAME}/
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Res,fae	${INSTALLDIR}/${APPNAME}/
	${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Sprites,ff9	${INSTALLDIR}/${APPNAME}/
	-${MKDIR} ${INSTALLDIR}/${APPNAME}/ini
	-${CP} ${CPFLAGS} Resources/ini/appsrv			${INSTALLDIR}/${APPNAME}/ini/
	-${CP} ${CPFLAGS} Resources/ini/module			${INSTALLDIR}/${APPNAME}/ini/
	-${CP} ${CPFLAGS} Resources/ini/wfclient		${INSTALLDIR}/${APPNAME}/ini/
	-${CP} ${CPFLAGS} Resources/app/${COUNTRY}/Splash,ff9	${INSTALLDIR}/${APPNAME}/
	-${CP} ${CPFLAGS} ${APPD}				${INSTALLDIR}/${APPNAME}/!RunImage,ff8
	echo Installed ${APPNAME}

cleaninstall:
	${RM}	 ${APPNAME}/ini/*
	${RMDIR} ${APPNAME}/ini
	${RM}	 ${APPNAME}/*
	${RMDIR} ${APPNAME}
	echo Cleaned install

# ----------------------------------------------------------------------------------------------------

# Components

# LIBRARY

CITRIXLIBSRC=	src/lib/cfgini/cfgini.c \
		src/lib/cfgload/cfgload.c \
		src/lib/clib/delay.c src/lib/clib/fileio.c src/lib/clib/loadstr.c src/lib/clib/string.c \
		src/lib/encrypt/encrypt.c \
		src/lib/ini/iniapi.c \
		src/lib/io/icabrows/icabrows.c src/lib/io/tcpro/tcpro.c \
		src/lib/log/logapi.c \
		src/lib/riscos/dll/dll.c \
		src/lib/riscos/kbd/kbdapi.c \
		src/lib/riscos/vio/viocfg.c src/lib/riscos/vio/viocur.c src/lib/riscos/vio/viomisc.c \
		src/lib/riscos/vio/vioread.c src/lib/riscos/vio/vioscrll.c src/lib/riscos/vio/viowrt.c \
		src/lib/riscos/vio/vioro.c 

CITRIXLIBOBJS=	src/lib/cfgini/cfgini.o \
		src/lib/cfgload/cfgload.o \
		src/lib/clib/delay.o src/lib/clib/fileio.o src/lib/clib/loadstr.o src/lib/clib/string.o \
		src/lib/encrypt/encrypt.o \
		src/lib/ini/iniapi.o \
		src/lib/io/icabrows/icabrows.o src/lib/io/tcpro/tcpro.o \
		src/lib/log/logapi.o \
		src/lib/riscos/dll/dll.o \
		src/lib/riscos/kbd/kbdapi.o \
		src/lib/riscos/vio/viocfg.o src/lib/riscos/vio/viocur.o src/lib/riscos/vio/viomisc.o \
		src/lib/riscos/vio/vioread.o src/lib/riscos/vio/vioscrll.o src/lib/riscos/vio/viowrt.o \
		src/lib/riscos/vio/vioro.o 

CITRIXLIBOBJSD=	src/lib/cfgini/cfgini.od \
		src/lib/cfgload/cfgload.od \
		src/lib/clib/delay.od src/lib/clib/fileio.od src/lib/clib/loadstr.od src/lib/clib/string.od \
		src/lib/encrypt/encrypt.od \
		src/lib/ini/iniapi.od \
		src/lib/io/icabrows/icabrows.od src/lib/io/tcpro/tcpro.od \
		src/lib/log/logapi.od \
		src/lib/riscos/dll/dll.od \
		src/lib/riscos/kbd/kbdapi.od \
		src/lib/riscos/vio/viocfg.od src/lib/riscos/vio/viocur.od src/lib/riscos/vio/viomisc.od \
		src/lib/riscos/vio/vioread.od src/lib/riscos/vio/vioscrll.od src/lib/riscos/vio/viowrt.od \
		src/lib/riscos/vio/vioro.od


${CITRIXLIB}: ${CITRIXLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIB} ${CITRIXLIBOBJS}

${CITRIXLIBD}: ${CITRIXLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${CITRIXLIBD} ${CITRIXLIBOBJSD}

cleanlib:
	-${RM} ${CITRIXLIB}
	-${RM} ${CITRIXLIBOBJS}
	-${RM} ${CITRIXLIBD}
	-${RM} ${CITRIXLIBOBJSD}
	echo cleaned lib

src/lib/cfgini/cfgini.o src/lib/cfgini/cfgini.od: src/lib/bini/biniguts.c src/lib/ini/helpers.c
src/lib/ini/iniapi.o src/lib/ini/iniapi.od: src/lib/ini/helpers.c

# ----------------------------------------------------------------------------------------------------

# DLLs

# NR

NRLIBSRC=	src/dll/nr/common/nrapi.c  src/dll/nr/nrtcpro/nrtcpro.c
NRLIBOBJS=	src/dll/nr/common/nrapi.o  src/dll/nr/nrtcpro/nrtcpro.o
NRLIBOBJSD=	src/dll/nr/common/nrapi.od src/dll/nr/nrtcpro/nrtcpro.od

${NRLIB}:	${NRLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIB} ${NRLIBOBJS}

${NRLIBD}:	${NRLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${NRLIBD} ${NRLIBOBJSD}

cleannr:
	-${RM} ${NRLIB}
	-${RM} ${NRLIBOBJS}
	-${RM} ${NRLIBD}
	-${RM} ${NRLIBOBJSD}
	echo cleaned nr libs

# WENGINE

WENGINELIBSRC=	src/dll/wengine/dosonly.c src/dll/wengine/wengine.c \
		src/dll/wengine/worker.c

WENGINELIBOBJS=	src/dll/wengine/dosonly.o src/dll/wengine/wengine.o \
		src/dll/wengine/worker.o

WENGINELIBOBJSD=src/dll/wengine/dosonly.od src/dll/wengine/wengine.od \
		src/dll/wengine/worker.od

${WENGINELIB}: ${WENGINELIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIB} ${WENGINELIBOBJS}

${WENGINELIBD}: ${WENGINELIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WENGINELIBD} ${WENGINELIBOBJSD}

cleanwengine:
	-${RM} ${WENGINELIB}
	-${RM} ${WENGINELIBOBJS}
	-${RM} ${WENGINELIBD}
	-${RM} ${WENGINELIBOBJSD}
	echo cleaned wengine libs


# TD

TDCOMMONLIBSRC=		src/dll/td/common/tdapi.c
TDCOMMONLIBOBJS=	src/dll/td/common/tdapi.o
TDCOMMONLIBOBJSD=	src/dll/td/common/tdapi.od

TDTCPROLIBSRC=		src/dll/td/comtcp/tdtcpcom.c  src/dll/td/tdtcpro/tdtcpro.c  src/dll/td/tdtcpro/sock.c
TDTCPROLIBOBJS=		src/dll/td/comtcp/tdtcpcom.o  src/dll/td/tdtcpro/tdtcpro.o  src/dll/td/tdtcpro/sock.o
TDTCPROLIBOBJSD=	src/dll/td/comtcp/tdtcpcom.od src/dll/td/tdtcpro/tdtcpro.od src/dll/td/tdtcpro/sock.od

TDLIBSRC=	${TDCOMMONLIBSRC} ${TDTCPROLIBSRC}
TDLIBOBJS=	${TDCOMMONLIBOBJS} ${TDTCPROLIBOBJS}
TDLIBOBJSD=	${TDCOMMONLIBOBJSD} ${TDTCPROLIBOBJSD}

${TDLIB}:	${TDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIB} ${TDLIBOBJS}

${TDLIBD}:	${TDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${TDLIBD} ${TDLIBOBJSD}

cleantd:
	-${RM} ${TDLIB}
	-${RM} ${TDLIBD}
	-${RM} ${TDLIBOBJS}
	-${RM} ${TDLIBOBJSD}
	echo cleaned td libs

# PD

PDCOMMONLIBSRC=src/dll/pd/common/pdapi.c src/dll/pd/common/buffer.c
PDCOMMONLIBOBJS=src/dll/pd/common/pdapi.o src/dll/pd/common/buffer.o
PDCOMMONLIBOBJSD=src/dll/pd/common/pdapi.od src/dll/pd/common/buffer.od

PDRFRAMELIBSRC=src/dll/pd/pdrframe/pdrframe.c src/dll/pd/pdrframe/callup.c src/dll/pd/pdrframe/input.c
PDRFRAMELIBOBJS=src/dll/pd/pdrframe/pdrframe.o src/dll/pd/pdrframe/callup.o src/dll/pd/pdrframe/input.o
PDRFRAMELIBOBJSD=src/dll/pd/pdrframe/pdrframe.od src/dll/pd/pdrframe/callup.od src/dll/pd/pdrframe/input.od

PDCRYPT1LIBSRC=src/dll/pd/pdcrypt1/pdcrypt1.c src/dll/pd/pdcrypt1/callup.c src/dll/pd/pdcrypt1/input.c src/dll/pd/pdcrypt1/encrypt.c
PDCRYPT1LIBOBJS=src/dll/pd/pdcrypt1/pdcrypt1.o src/dll/pd/pdcrypt1/callup.o src/dll/pd/pdcrypt1/input.o src/dll/pd/pdcrypt1/encrypt.o
PDCRYPT1LIBOBJSD=src/dll/pd/pdcrypt1/pdcrypt1.od src/dll/pd/pdcrypt1/callup.od src/dll/pd/pdcrypt1/input.od src/dll/pd/pdcrypt1/encrypt.od

PDLIBSRC=	${PDCOMMONLIBSRC} ${PDRFRAMELIBSRC} ${PDCRYPT1LIBSRC}
PDLIBOBJS=	${PDCOMMONLIBOBJS} ${PDRFRAMELIBOBJS} ${PDCRYPT1LIBOBJS}
PDLIBOBJSD=	${PDCOMMONLIBOBJSD} ${PDRFRAMELIBOBJSD} ${PDCRYPT1LIBOBJSD}

${PDLIB}:	${PDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIB} ${PDLIBOBJS}

${PDLIBD}:	${PDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${PDLIBD} ${PDLIBOBJSD}

cleanpd:
	-${RM} ${PDLIB}
	-${RM} ${PDLIBD}
	-${RM} ${PDLIBOBJS}
	-${RM} ${PDLIBOBJSD}
	echo cleaned pd libs

# WD

WDCOMMONLIBSRC=		src/dll/wd/common/wdapi.c src/dll/wd/common/buffer.c
WDCOMMONLIBOBJS=	src/dll/wd/common/wdapi.o src/dll/wd/common/buffer.o
WDCOMMONLIBOBJSD=	src/dll/wd/common/wdapi.od src/dll/wd/common/buffer.od

WDTTYLIBSRC=		src/dll/wd/wdtty/wdtty.c src/dll/wd/wdtty/callup.c
WDTTYLIBOBJS=		src/dll/wd/wdtty/wdtty.o src/dll/wd/wdtty/callup.o
WDTTYLIBOBJSD=		src/dll/wd/wdtty/wdtty.od src/dll/wd/wdtty/callup.od

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


WDLIBSRC=	${WDCOMMONLIBSRC}  ${WDTTYLIBSRC}  ${WDICA30LIBSRC}
WDLIBOBJS=	${WDCOMMONLIBOBJS}  ${WDTTYLIBOBJS}  ${WDICA30LIBOBJS}
WDLIBOBJSD=	${WDCOMMONLIBOBJSD} ${WDTTYLIBOBJSD} ${WDICA30LIBOBJSD}

${WDLIB}:	${WDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIB} ${WDLIBOBJS}

${WDLIBD}:	${WDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${WDLIBD} ${WDLIBOBJSD}

cleanwd:
	-${RM} ${WDLIB}
	-${RM} ${WDLIBD}
	-${RM} ${WDLIBOBJS}
	-${RM} ${WDLIBOBJSD}
	echo cleaned wd libs

# VD

VDCOMMONLIBSRC	=src/dll/vd/common/vdapi.c
VDCOMMONLIBOBJS	=src/dll/vd/common/vdapi.o
VDCOMMONLIBOBJSD=src/dll/vd/common/vdapi.od

VDTW31LIBSRC=	src/dll/vd/vdtw31/twvd.c
VDTW31LIBOBJS=	src/dll/vd/vdtw31/twvd.o
VDTW31LIBOBJSD=	src/dll/vd/vdtw31/twvd.od

VDLIBSRC=	${VDCOMMONLIBSRC} ${VDTW31LIBSRC}
VDLIBOBJS=	${VDCOMMONLIBOBJS} ${VDTW31LIBOBJS}
VDLIBOBJSD=	${VDCOMMONLIBOBJSD} ${VDTW31LIBOBJSD}

${VDLIB}:	${VDLIBOBJS}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIB} ${VDLIBOBJS}

${VDLIBD}:	${VDLIBOBJSD}
	${LIBFILE} ${LIBFILEFLAGS} -o ${VDLIBD} ${VDLIBOBJSD}

cleanvd:
	-${RM} ${VDLIB}
	-${RM} ${VDLIBD}
	-${RM} ${VDLIBOBJS}
	-${RM} ${VDLIBOBJSD}
	echo cleaned vd libs

# generic dll

cleandll: cleanwengine cleantd cleanpd cleanwd cleanvd
	echo cleaned dll files

# ----------------------------------------------------------------------------------------------------

# APP

APPSRC=		src/app/connect.c src/app/main.c src/app/rdebug.c \
		src/app/session.c src/app/utils.c
APPOBJS=	src/app/connect.o src/app/main.o src/app/rdebug.o \
		src/app/session.o src/app/utils.o
APPOBJSD=	src/app/connect.od src/app/main.od src/app/rdebug.od \
		src/app/session.od src/app/utils.od

${APP}: ${APPOBJS} ${APPLIBS} ${BASELIBS}
	${LD} ${LDFLAGS} -o ${APP} ${APPOBJS} ${APPLIBS} ${BASELIBS}
	ls -l $@

${APPD}: ${APPOBJSD} ${APPLIBSD} ${BASELIBSD}
	${LD} ${LDFLAGS} -o ${APPD} ${APPOBJSD} ${APPLIBSD} ${BASELIBSD}
	ls -l $@

cleanapp:
	-${RM} ${APPOBJS}
	-${RM} ${APPOBJSD}
	-${RM} ${APP}
	-${RM} ${APPD}
	echo cleaned app files

# ----------------------------------------------------------------------------------------------------

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
# DO NOT DELETE THIS LINE - armdepend dependencies follow

