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
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -zps1 -ff -wp ${INCLUDES} ${DFLAGS} -DCOMPAT_INET4 -DUSING_INET5
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
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
SSLDIR    = <Lib$Dir>.SSL
REMOTEDB  = <Lib$Dir>.debug.o.remotezm

INSECURELIBS= ${INETLIB} ${SOCKLIB}
SECURELIBS= $(SSLDIR).o.ssllib $(SSLDIR).o.cryptolib ${UNIXLIB} ${INSECURELIBS}
#
# Include files
#
INCLUDES  = -IC:,TCPIPLibs:,$(SSLDIR),<Lib$Dir>.
DFLAGS = -DCSFS ${DB}
#
# Program specific options:
#
COMPONENT = http
TARGET    = aof.${COMPONENT}
OBJS      = mimemap.o files.o http.o modhead.o
SECUREOBJS= os.mimemap os.files os.http modheadS.o
EXPORTS   = h.httppub

.SUFFIXES:   .os

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $< -DVERIFY_MD
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}mod rm.${COMPONENT}smod
#all: rm.${COMPONENT}mod

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.httppub ${EXP_HDR}.h.http ${CPFLAGS}
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
${TARGET}: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS} ${REMOTEDB}
        ${LD} -o $@ -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS} ${REMOTEDB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}mod: ${OBJS} ${CLIB} ${INSECURELIBS} ${REMOTEDB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${INSECURELIBS} ${REMOTEDB}
        Access rm.${COMPONENT}mod RW/R

rm.${COMPONENT}smod: ${SECUREOBJS} ${CLIB} ${SECURELIBS} ${REMOTEDB}
        ${LD} -o $@ -module ${SECUREOBJS} ${CLIB} ${SECURELIBS} ${REMOTEDB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
