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
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
NOP     = |


AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -zps1 -ff -wpc ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES} ${THROWBACK}
LDFLAGS =

#
# Libraries
#
LIBDIR		= <Lib$Dir>

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
DEBUGLIB  = ${LIBDIR}.DebugLib.o.DebugLibzm

INSECURELIBS= ${INETLIB} ${SOCKLIB}
BASESECURELIBS=$(SSLDIR).o.libcrypto ${UNIXLIB} ${INSECURELIBS}
SECURELIBS= $(SSLDIR).o.ssl ${BASESECURELIBS}
SECUREXLIBS= $(SSLDIR).o.sslx ${BASESECURELIBS}
PKCS11LIB=C:CryptoBox.o.cryptolib
#
# Include files
#
INCLUDES  = -IC:,C:TCPIPLibs.,${SSLDIR}.
DFLAGS =  ${DB} -DCOMPAT_INET4 -DFLAVOUR_TRANSPORT_MODULES=0 -DHTTPMOD -DRISCOS -DACORN_BUILD ${D}

DIRS = o._dirs
#
# Program specific options:
#
COMPONENT = HTTP
TARGET    = aof.${COMPONENT}
OBJS      = ModHead.o mimemap.o files.o file.o http.o module.o csfs.o certs.o layershttp.o
SECUREOBJS= os.ModHead os.mimemap os.files os.file os.http os.module os.csfs os.certs os.predirect os.layershttp os.client
PKCS11OBJS= op.ModHead op.mimemap op.files op.file op.http op.module op.csfs op.certs op.predirect op.layershttp op.client

PREPROOBJS= p.mimemap p.files p.file p.http p.module p.csfs p.certs p.predirect p.layershttp p.client

EXPORTS   = h.httppub h.pads

.SUFFIXES:   .os .op

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $< 
.c.op:;     ${CC} ${CFLAGS} -o $@ $< -DFLAVOUR_PKCS11=1 -IC:CryptoBox
.c.p:;      ${CC} ${CFLAGS} -E -C $< > p.$*
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -DNO_SECURE
.cmhg.os:;  ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.op:;  ${CMHG} ${CMHGFLAGS} -o $@ $< -DFLAVOUR_PKCS11=1 
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: all_${TYPE}
        @echo ${COMPONENT}${TYPE}: built (all)

all_: rm.${COMPONENT}Mod rm.${COMPONENT}sMod rm.${COMPONENT}sxMod rm.${COMPONENT}spMod rm.${COMPONENT}spxMod
        ${NOP}

all_i: rm.${COMPONENT}Mod
        ${NOP}

all_s: rm.${COMPONENT}sMod
        ${NOP}

all_sx: rm.${COMPONENT}sxMod
        ${NOP}

all_sp: rm.${COMPONENT}spMod
        ${NOP}

all_spx: rm.${COMPONENT}spxMod
        ${NOP}

prepro: ${PREPROOBJS}
        @echo preprocessed module made

#
# RISC OS ROM build rules:
#
rom:    rom_${TYPE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.httppub ${EXP_HDR}.h.http ${CPFLAGS}
        ${CP} h.pads ${EXP_HDR}.h.pads ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: install_${TYPE} instdir
        @echo ${COMPONENT}${TYPE}: installed (ram)

instdir:
        ${MKDIR} ${INSTDIR}

install_: rm.${COMPONENT}Mod rm.${COMPONENT}sMod rm.${COMPONENT}sxMod rm.${COMPONENT}spMod rm.${COMPONENT}spxMod
        ${NOP}

install_i: rm.${COMPONENT}Mod
        ${CP} rm.${COMPONENT}Mod ${INSTDIR}.${COMPONENT}Mod ${CPFLAGS}

install_s: rm.${COMPONENT}sMod
        ${CP} rm.${COMPONENT}sMod ${INSTDIR}.${COMPONENT}sMod ${CPFLAGS}

install_sx: rm.${COMPONENT}sxMod
        ${CP} rm.${COMPONENT}sxMod ${INSTDIR}.${COMPONENT}sxMod ${CPFLAGS}

install_sp: rm.${COMPONENT}spMod
        ${CP} rm.${COMPONENT}spMod ${INSTDIR}.${COMPONENT}spMod ${CPFLAGS}

install_spx: rm.${COMPONENT}spxMod
        ${CP} rm.${COMPONENT}spxMod ${INSTDIR}.${COMPONENT}spxMod ${CPFLAGS}

clean:
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} os ${WFLAGS}
        ${WIPE} op ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${RM} h.ModHead
        @echo ${COMPONENT}: cleaned

${DIRS}:
        @${MKDIR} aof
        @${MKDIR} linked
        @${MKDIR} map
        @${MKDIR} o
        @${MKDIR} os
        @${MKDIR} op
        @${MKDIR} rm
        create ${DIRS}
        stamp  ${DIRS}

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#

rom_pkcs11_strong: ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${TARGET} -aof ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECURELIBS}

rom_pkcs11_secure: ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECUREXLIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${TARGET} -aof ${PKCS11OBJS} ${ROMCSTUBS} ${PKCS11LIB} ${SECUREXLIBS}

rom_strong: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}

rom_secure: ${SECUREOBJS} ${ROMCSTUBS} ${SECUREXLIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECUREXLIBS}

rom_ rom_insecure: ${OBJS} ${ROMCSTUBS} ${INSECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${TARGET} -aof ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link: ${DIRS}
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}Mod: ${OBJS} ${DEBUGLIB} ${CLIB} ${INSECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${OBJS} ${DEBUGLIB} ${CLIB} ${INSECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}sMod: ${SECUREOBJS} ${DEBUGLIB} ${CLIB} ${SECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${SECUREOBJS} ${DEBUGLIB} ${CLIB} ${SECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}sxMod: ${SECUREOBJS} ${DEBUGLIB} ${CLIB} ${SECUREXLIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${SECUREOBJS} ${DEBUGLIB} ${CLIB} ${SECUREXLIBS}
        Access $@ RW/R

rm.${COMPONENT}spMod: ${PKCS11OBJS} ${DEBUGLIB} ${CLIB} ${PKCS11LIB} ${SECURELIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${PKCS11OBJS} ${DEBUGLIB} ${CLIB} ${PKCS11LIB} ${SECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}spxMod: ${PKCS11OBJS} ${DEBUGLIB} ${CLIB} ${PKCS11LIB} ${SECUREXLIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ -module ${PKCS11OBJS} ${DEBUGLIB} ${CLIB} ${PKCS11LIB} ${SECUREXLIBS}
        Access $@ RW/R

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Static dependencies

o.module: h.ModHead
os.module: h.ModHead
op.module: h.ModHead

# Dynamic dependencies:
