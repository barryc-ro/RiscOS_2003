# Makefile for CSFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-1997  RWB 	  New for CSFS
# 28 May 1997  RWB 	  Different Debug build
# 11 Jun 1997  SC	  Added option for ensure line
# 18 Jul 1997  RWB	  Added build for file debug

#
# Paths
#

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile

OPTIONS = -DENSURE_LINE

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS} ${OPTIONS}
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
DFLAGS  = ${MESSAGES}
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
INETLIB    = TCPIPLibs:o.inetlibzm
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
FILEDB     = <Lib$Dir>.debug.o.filezm
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = CSFS
TARGET    = aof.${COMPONENT}

BASEOBJS      =	o.RMInfo o.csfserr o.sfs o.sff o.sfi o.arcs o.arcf o.dir \
	        o.cat o.flex o.fx o.mem o.scrap o.mount o.service o.serx \
	        o.update o.fs o.interface o.link o.open	o.gbpb o.func	 \
		o.file o.close o.args o.getbytes o.putbytes o.xstr       \
		o.usermode o.modulewrap o.header

RDBASEOBJS    =	ord.RMInfo ord.csfserr ord.sfs ord.sff ord.sfi ord.arcs    \
		ord.arcf ord.dir ord.cat ord.flex ord.fx ord.mem ord.scrap \
		ord.mount ord.service ord.serx ord.update ord.fs 	   \
		ord.interface ord.link ord.open	ord.gbpb ord.func ord.file \
		ord.close ord.args ord.getbytes ord.putbytes ord.xstr      \
		ord.usermode ord.modulewrap ord.header

FDBASEOBJS    =	ofd.RMInfo ofd.csfserr ofd.sfs ofd.sff ofd.sfi ofd.arcs    \
		ofd.arcf ofd.dir ofd.cat ofd.flex ofd.fx ofd.mem ofd.scrap \
		ofd.mount ofd.service ofd.serx ofd.update ofd.fs 	   \
		ofd.interface ofd.link ofd.open	ofd.gbpb ofd.func ofd.file \
		ofd.close ofd.args ofd.getbytes ofd.putbytes ofd.xstr      \
		ofd.usermode ofd.modulewrap ofd.header 

OBJS =    o.resources  ${BASEOBJS}
FDOBJS =  ofd.resources ${FDBASEOBJS}
RDOBJS =  ord.resources ${RDBASEOBJS}
ROMOBJS = ${BASEOBJS}


.SUFFIXES:   .ofd 
.SUFFIXES:   .ord

#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.modulewrap
.s.o:;      ${AS} ${AFLAGS}  $< $@
.c.ofd:;    ${CC} ${CFLAGS} -o $@ $< -DFILE_DEBUG
.cmhg.ofd:; ${CMHG} ${CMHGFLAGS} -o $@ $< h.modulewrap -DFILE_DEBUG
.s.ofd:;    ${AS} ${AFLAGS} $< $@
.c.ord:;    ${CC} ${CFLAGS} -o $@ $< -DREMOTE_DEBUG
.cmhg.ord:; ${CMHG} ${CMHGFLAGS} -o $@ $< h.modulewrap -DREMOTE_DEBUG
.s.ord:;    ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} dirs
        @echo ${COMPONENT}: complete

rdebug: rm.${COMPONENT}RD dirs
        @echo ${COMPONENT}RD: complete

fdebug: rm.${COMPONENT}FD dirs
        @echo ${COMPONENT}FD: complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS} dirs
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ord.* ${WFLAGS}
        ${WIPE} ofd.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

dirs:
	${MKDIR} o
	${MKDIR} ofd
	${MKDIR} ord
	
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${INETLIB}
	${MKDIR} o
	${MKDIR} aof
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${INETLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB} ${INETLIB}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} ${INETLIB}
        Access rm.${COMPONENT} RW/R

rm.${COMPONENT}RD: ${RDOBJS} ${CLIB} ${REMOTEDB} ${INETLIB}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${RDOBJS} ${CLIB} ${INETLIB} ${REMOTEDB}
        Access rm.${COMPONENT}RD RW/R
        
rm.${COMPONENT}FD: ${FDOBJS} ${CLIB} ${FILEDB} ${INETLIB}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${FDOBJS} ${CLIB} ${FILEDB} ${INETLIB}
        Access rm.${COMPONENT}FD RW/R
        
#
# Dynamic dependencies:
