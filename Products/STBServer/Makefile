#
# Makefile for STBServer components for Solaris.
#
# Copyright (C) Pace Micro Technology plc. 2000
#
# $Id$
#
# Makefile for extracting and building all the server-side software
# for the TFTP baseline support.

ifeq "${ARCH}" "linux"
OSDIR=linux
else
OSDIR=solaris
endif

TARFILE=mtftpd-${ARCH}${VERSIONNUM}

all: scripts modgen modsqz mtftpd unmodsqz
	(cd ${TARDIR}; tar cf ${TARFILE}.tar blconf blbuild blswinst modgen modsqz mtftpd unmodsqz mtftpd.conf mtftpd.sh)
	compress ${TARDIR}/${TARFILE}.tar
	cp ${TARDIR}/${TARFILE}.tar.Z ${TARFILE}.tar.Z
	@echo All done

scripts:
	cp Unix/Sources/Scripts/blconf ${TARDIR}/blconf
	cp Unix/Sources/Scripts/blbuild ${TARDIR}/blbuild
	cp Unix/Sources/Scripts/blswinst ${TARDIR}/blswinst

modgen resgen: clx
	(cd RiscOS/Tools/Sources/ResGen/ports/solaris; ./mklinks; make)
	cp RiscOS/Tools/Sources/ResGen/ports/solaris/modgen ${TARDIR}/modgen

modsqz: clx
	(cd RiscOS/Tools/Sources/modsqz/ports/solaris; ./mklinks; make)
	cp RiscOS/Tools/Sources/modsqz/ports/solaris/modsqz ${TARDIR}/modsqz

unmodsqz: clx
	(cd RiscOS/Tools/Sources/unmodsqz/ports/solaris; ./mklinks; make)
	cp RiscOS/Tools/Sources/unmodsqz/ports/solaris/unmodsqz ${TARDIR}/unmodsqz

mtftpd:
	(cd Unix/Sources/Networking/mtftpd; acbootstrap --quiet; ./configure --quiet --sysconfdir=/etc; make)
	cp Unix/Sources/Networking/mtftpd/src/mtftpd ${TARDIR}/mtftpd
	cp Unix/Sources/Networking/mtftpd/src/mtftpd.sh ${TARDIR}/mtftpd.sh
	cp Unix/Sources/Networking/mtftpd/src/mtftpd.conf ${TARDIR}/mtftpd.conf
	strip ${TARDIR}/mtftpd
 
clx:
	(cd RiscOS/Sources/Lib/CLX/ccsolaris; ./mklinks; make install;)

