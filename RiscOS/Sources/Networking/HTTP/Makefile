# Makefile for http
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <Cexport$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link -v -map
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -zps1 -ff -wp ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIB   = C:TCPIPLibs.o.inetlibzm
SOCKLIB   = C:TCPIPLibs.o.socklibzm
UNIXLIB   = C:TCPIPLibs.o.unixlibzm
SSLDIR    = C:SSLeay
REMOTEDB  = C:debug.o.lremotezm

INSECURELIBS= ${INETLIB} ${SOCKLIB}
BASESECURELIBS=C:CryptoBox.o.cryptolib $(SSLDIR).o.libcrypto ${UNIXLIB} ${INSECURELIBS}
SECURELIBS= $(SSLDIR).o.ssl ${BASESECURELIBS}
SECUREXLIBS= $(SSLDIR).o.sslx ${BASESECURELIBS}
#
# Include files
#
INCLUDES  = -IC:CryptoBox,C:,C:TCPIPLibs.,${SSLDIR}.
DFLAGS =  ${DB} -DCOMPAT_INET4 -DFLAVOUR_TRANSPORT_MODULES=0 -DHTTPMOD -DRISCOS -DACORN_BUILD ${D}
#
# Program specific options:
#
COMPONENT = http
TARGET    = aof.${COMPONENT}
OBJS      = ModHead.o mimemap.o files.o file.o http.o module.o csfs.o certs.o layershttp.o
BASESECUREOBJS= os.ModHead os.mimemap os.files os.file os.http os.module os.csfs os.certs os.predirect os.layershttp os.client
SECUREOBJS=  ${BASESECUREOBJS}
SECUREXOBJS= ${BASESECUREOBJS}
PREPROOBJS= p.mimemap p.files p.file p.http p.module p.csfs p.certs p.predirect p.layershttp p.client

EXPORTS   = h.httppub h.pads

.SUFFIXES:   .os

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $< 
.c.p:;      ${CC} ${CFLAGS} -E -C $< > p.$*
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -DNO_SECURE
.cmhg.os:;  ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}mod rm.${COMPONENT}smod rm.${COMPONENT}sxmod

prepro: ${PREPROOBJS}
	@echo preprocessed module made

#
# RISC OS ROM build rules:
#
rom:	rom_${TYPE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.httppub ${EXP_HDR}.h.http ${CPFLAGS}
        ${CP} h.pads ${EXP_HDR}.h.pads ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT}mod rm.${COMPONENT}smod
	${CP} rm.${COMPONENT}mod ${INSTDIR}.${COMPONENT}mod ${CPFLAGS}
	${CP} rm.${COMPONENT}smod ${INSTDIR}.${COMPONENT}smod ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} os.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#

rom_strong: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}
        ${LD} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}

rom_secure: ${SECUREXOBJS} ${ROMCSTUBS} ${SECUREXLIBS}
        ${LD} -o ${TARGET} -aof ${SECUREXOBJS} ${ROMCSTUBS} ${SECUREXLIBS}

rom_ rom_insecure: ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}
        ${LD} -o ${TARGET} -aof ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}mod: ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}
        ${LD} -o $@ -module ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}smod: ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}
        ${LD} -o $@ -module ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}sxmod: ${SECUREXOBJS} ${REMOTEDB} ${CLIB} ${SECUREXLIBS}
        ${LD} -o $@ -module ${SECUREXOBJS} ${REMOTEDB} ${CLIB} ${SECUREXLIBS}
        Access $@ RW/R

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
