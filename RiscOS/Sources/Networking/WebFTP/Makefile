# Makefile for webftp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.


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
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -Wc -depend !Depend ${THROWBACK} -zM -zps1 -ffah ${INCLUDES} ${DFLAGS} -wp
CMHGFLAGS = -depend !Depend -p ${THROWBACK} ${DFLAGS} ${INCLUDES}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
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
INETLIB_NORM=TCPIPLibs:o.inetlib
SOCKLIB_NORM=TCPIPLibs:o.socklib
DEBUGLIB  = <Lib$Dir>.DebugLib.o.DebugLibzm
#
# Include files
#

INCLUDES  = -IC:,TCPIPLibs:,<Lib$Dir>.

DIRS      = o._dirs

# enable malloc debugging using the following define.
#-DDEBUG_MALLOC
DFLAGS = -DVERBOSEDEBUG=0 -DRISCOS -DPROG_IDENT="\"NCBrowser\"" -DCOMPAT_INET4 -DFTP_PUT -DCSFS ${FTP_DB} ${FTP_REMOTE_DB} -DCMHG_FINALISE=1

#
# Program specific options:
#
COMPONENT = webftp
COMPONENTD = webftpD
TARGET    = aof.${COMPONENT}
OBJS      = webftp.o q.o files.o header.o support.o misc.o db.o dirparse.o 
OBJSD     = od.webftp od.q od.files header.o support.o od.misc od.db od.dirparse
LIBS      = ${CLIB} ${INETLIB} ${SOCKLIB}
LIBSD     = ${CLIB} ${DEBUGLIB} ${INETLIB} ${SOCKLIB}
EXPORTS   = h.webftp

#
# Rule patterns
#
.SUFFIXES:	.o .od .s .h .cmhg .c .z
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;     ${CC} ${CFLAGS} -DDEBUGLIB -Fn -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}
debug: rm.${COMPONENTD}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
	${CP} h.webftp ${EXP_HDR}.h.webftp ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT}
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${RM} h.header
        ${STRIP}
        @echo ${COMPONENT}: cleaned

${DIRS}:
        @${MKDIR} aof
        @${MKDIR} linked
        @${MKDIR} map
        @${MKDIR} o
        @${MKDIR} od
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
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB} ${REMOTEDB} ${DIRS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB} ${EXTRALIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Non-debug RAM target
rm.${COMPONENT}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} -o $@ -module ${OBJS} ${LIBS}
        Access rm.${COMPONENT} RW/R

# Debug RAM target
rm.${COMPONENTD}: ${OBJSD} ${LIBSD} ${DIRS}
        ${LD} -o $@ -module ${OBJSD} ${LIBSD}
        Access rm.${COMPONENTD} RW/R

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

test: o.test1
        ${LD} -o $@ o.test1 ${CLIB} ${INETLIB_NORM}
        @echo Test built

o.test1: c.test1
        ${CC} -c ${INCLUDES} ${DFLAGS} -o $@ c.test1

# Static dependencies

o.webftp:  h.header
od.webftp: h.header
o.misc:    h.header
od.misc:   h.header

# Dynamic dependencies:
