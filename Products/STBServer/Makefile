#
# Makefile for STBServer components for Solaris.
#
# Copyright (C) Pace Micro Technology plc. 2000
#
# $Id$
#
# Makefile for extracting and building all the server-side software
# for the TFTP baseline support.

all: scripts modgen modsqz mtftpd 
	(cd ${TARDIR}; tar cf dist-server.tar blconf blbuild blswinst S94mtftpd mtftpd.conf modgen modsqz mtftpd)
	compress ${TARDIR}/dist-server.tar
	cp ${TARDIR}/dist-server.tar.Z dist-server-tar.Z
	@echo All done

scripts:
	cp Unix/Sources/Scripts/blconf ${TARDIR}/blconf
	cp Unix/Sources/Scripts/blbuild ${TARDIR}/blbuild
	cp Unix/Sources/Scripts/blswinst ${TARDIR}/blswinst
	cp Unix/Sources/Scripts/S94mtftpd ${TARDIR}/S94mtftpd
	cp Unix/Sources/Scripts/mtftpd.conf ${TARDIR}/mtftpd.conf

modgen resgen: clx
	(cd RiscOS/Tools/Sources/ResGen/ports/solaris; ./mklinks; make)
	cp RiscOS/Tools/Sources/ResGen/ports/solaris/modgen ${TARDIR}/modgen

modsqz: clx
	(cd RiscOS/Tools/Sources/modsqz/ports/solaris; ./mklinks; make)
	cp RiscOS/Tools/Sources/modsqz/ports/solaris/modsqz ${TARDIR}/modsqz

mtftpd:
	(cd RiscOS/Sources/Networking/TFTPserver/ports/solaris; ./mklinks; make ARCH=${ARCH})
	cp RiscOS/Sources/Networking/TFTPserver/ports/solaris/mtftpd ${TARDIR}/mtftpd
 
clx:
	(cd RiscOS/Sources/Lib/CLX/ccsolaris; ./mklinks; make install;)

