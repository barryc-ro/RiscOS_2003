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
	tar cf dist-server.tar \
	Unix/Sources/Scripts/blconf \
	Unix/Sources/Scripts/blbuild \
	Unix/Sources/Scripts/blswinst \
	Unix/Sources/Scripts/S94mtftpd \
	Unix/Sources/Scripts/mtftpd.conf \
	Tools/Sources/ResGen/ports/solaris/modgen \
	Tools/Sources/modsqz/ports/solaris/modsqz \
	RiscOS/Sources/Networking/TFTPserver/ports/solaris/mtftpd 
	gzip dist-server.tar
	@echo All done

scripts:

modgen resgen:
	(cd Tools/Sources/ResGen/ports/solaris; ./mklinks; make)

modsqz:
	(cd Tools/Sources/ModSqz/ports/solaris; ./mklinks; make)

mtftpd:
	(cd RiscOS/Sources/Networking/TFTPserver/ports/solaris; ./mklinks; make)
 
