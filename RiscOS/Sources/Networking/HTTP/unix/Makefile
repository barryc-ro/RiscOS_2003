# Makefile for http

#
# Paths
#
CPATH = ${RISCOS_BUILD_EXPORT}
EXP_HDR = ${CPATH}/C
#EXP_HDR = ${RISCOS_BUILD_EXPORT}

#
# Generic options:
#
MKDIR   = mkdir
AS      = armasm
CC      = armcc
CMHG    = armcmhg
CP      = cp
LD      = armlink
RM      = -rm
WIPE    = -rm

AFLAGS = 
CFLAGS  = -c -zM -zps1 -ff -wpc ${INCLUDES} ${DFLAGS}
CPFLAGS = -p
WFLAGS  = 
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

#
# Libraries
#
CLIB      = ${CPATH}/Lib/CLib/stubs.o
RLIB      = ${RISCOSLIB}/risc_oslib.o
RSTUBS    = ${RISCOSLIB}/rstubs.o
ROMSTUBS  = ${RISCOSLIB}/romstubs.o
ROMCSTUBS = ${RISCOSLIB}/romcstubs.o
ABSSYM    = ${RISCOSLIB}/AbsSym.o
INETLIB   = ${CPATH}/Lib/TCPIPLibs/inetlibzm.o
SOCKLIB   = ${CPATH}/Lib/TCPIPLibs/socklibzm.o
UNIXLIB   = ${CPATH}/Lib/TCPIPLibs/unixlibzm.o
SSLDIR    = ${CPATH}/Lib/SSLeay
REMOTEDB  = ${CPATH}/Lib/debug/lremotezm.o

INSECURELIBS= ${INETLIB} ${SOCKLIB}
BASESECURELIBS=$(SSLDIR)/libcrypto.o ${UNIXLIB} ${INSECURELIBS}
SECURELIBS= $(SSLDIR)/ssl.o ${BASESECURELIBS}
SECUREXLIBS= $(SSLDIR)/sslx.o ${BASESECURELIBS}
PKCS11LIB=${CPATH}/Lib/CryptoBox/cryptolib.o
#
# Include files
#
INCLUDES  = -I${CPATH}/Lib -I${CPATH}/Lib/TCPIPLibs -I${SSLDIR}
DFLAGS =  ${DB} -DCOMPAT_INET4 -DFLAVOUR_TRANSPORT_MODULES=0 -DHTTPMOD -DRISCOS -DACORN_BUILD ${D}
#
# Program specific options:
#
COMPONENT = http
TARGET    = ${COMPONENT}.aof
OBJS      = ModHead.o mimemap.o files.o file.o http.o module.o csfs.o certs.o layershttp.o
SECUREOBJS= ModHead.os mimemap.os files.os file.os http.os module.os csfs.os certs.os predirect.os layershttp.os client.os
PKCS11OBJS= ModHead.op mimemap.op files.op file.op http.op module.op csfs.op certs.op predirect.op layershttp.op client.op

PREPROOBJS= mimemap.p files.p file.p http.p module.p csfs.p certs.p predirect.p layershttp.p client.p

EXPORTS   = httppub.h pads.h

.SUFFIXES: .os .op .cmhg

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $< 
.c.op:;     ${CC} ${CFLAGS} -o $@ $< -DFLAVOUR_PKCS11=1 -I${CPATH}/Lib/CryptoBox
.c.p:;      ${CC} ${CFLAGS} -E -C $< > $*.p
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -DNO_SECURE
.cmhg.os:;  ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.op:;  ${CMHG} ${CMHGFLAGS} -o $@ $< -DFLAVOUR_PKCS11=1 
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${COMPONENT}mod,ffa ${COMPONENT}smod,ffa ${COMPONENT}sxmod,ffa
# ${COMPONENT}spmod,ffa ${COMPONENT}spxmod,ffa

prepro: ${PREPROOBJS}
	@echo preprocessed module made

#
# RISC OS ROM build rules:
#
rom:	rom_${TYPE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
	${CP} ${CPFLAGS} httppub.h ${EXP_HDR}/http.h
	${CP} ${CPFLAGS} pads.h ${EXP_HDR}/pads.h
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
	${CP} ${CPFLAGS} ${TARGET} ${INSTDIR}/${COMPONENT}
        @echo ${COMPONENT}: rom module installed

install: ${COMPONENT}mod,ffa ${COMPONENT}smod,ffa
	${CP} ${CPFLAGS} ${COMPONENT}mod,ffa ${INSTDIR}/${COMPONENT}mod,ffa
	${CP} ${CPFLAGS} ${COMPONENT}smod,ffa ${INSTDIR}/${COMPONENT}smod,ffa
        @echo ${COMPONENT}: installed

clean:
	${WIPE} *.o ${WFLAGS}
	${WIPE} *.os ${WFLAGS}
	${WIPE} *.op ${WFLAGS}
	${WIPE} *,ffa ${WFLAGS}
	${WIPE} *.linked ${WFLAGS}
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied


#
# ROM target (re-linked at ROM Image build time)
#

rom_pkcs11_strong: ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECURELIBS}
	${LD} -o ${TARGET} -aof ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECURELIBS}

rom_pkcs11_secure: ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECUREXLIBS}
	${LD} -o ${TARGET} -aof ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECUREXLIBS}

rom_strong: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}
	${LD} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}

rom_secure: ${SECUREOBJS} ${ROMCSTUBS} ${SECUREXLIBS}
	${LD} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECUREXLIBS}

rom_ rom_insecure: ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}
	${LD} -o ${TARGET} -aof ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link: dirs
	${LD} -o ${COMPONENT}.linked -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
	${CP} ${CPFLAGS} ${COMPONENT}.linked ${LINKDIR}/${COMPONENT}
	@echo ${COMPONENT}: rom_link complete

${COMPONENT}mod,ffa: ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}
	${LD} -o $@ -rmf ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}

${COMPONENT}smod,ffa: ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}
	${LD} -o $@ -rmf ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}

${COMPONENT}sxmod,ffa: ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECUREXLIBS}
	${LD} -o $@ -rmf ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECUREXLIBS}

${COMPONENT}spmod,ffa: ${PKCS11OBJS} ${REMOTEDB} ${CLIB} ${PKCS11LIB} ${SECURELIBS}
	${LD} -o $@ -rmf ${PKCS11OBJS} ${REMOTEDB} ${CLIB} ${PKCS11LIB} ${SECURELIBS}

${COMPONENT}spxmod,ffa: ${PKCS11OBJS} ${REMOTEDB} ${CLIB} ${PKCS11LIB} ${SECUREXLIBS}
	${LD} -o $@ -rmf ${PKCS11OBJS} ${REMOTEDB} ${CLIB} ${PKCS11LIB} ${SECUREXLIBS}

# Dynamic dependencies:
