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
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ord.* ${WFLAGS}
        ${WIPE} ofd.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
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
	${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
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
ofd.resources: s.resources
ofd.resources: hdr:ListOpts
ofd.resources: hdr:Macros
ofd.resources: hdr:System
ofd.resources: Hdr:SWIs
ofd.resources: Hdr:CPU.Generic26
ofd.resources: Hdr:IO.GenericIO
ofd.resources: Hdr:RISCOS
ofd.resources: hdr:ModHand
ofd.resources: hdr:ResourceFS
ofd.resources: hdr:Proc
ofd.RMInfo: s.RMInfo
ofd.csfserr:	c.csfserr
ofd.csfserr:	C:h.kernel
ofd.csfserr:	C:h.swis
ofd.csfserr:	h.etc
ofd.csfserr:	h.csfserr
ofd.sfs:	c.sfs
ofd.sfs:	C:h.kernel
ofd.sfs:	C:h.swis
ofd.sfs:	h.Consts
ofd.sfs:	C:debug.h.file
ofd.sfs:	h.RMInfo
ofd.sfs:	h.veneers
ofd.sfs:	h.Interface
ofd.sfs:	h.fsentries
ofd.sfs:	h.ModuleWrap
ofd.sfs:	h.flex
ofd.sfs:	h.csfserr
ofd.sfs:	h.str
ofd.sfs:	h.etc
ofd.sfs:	h.arcs
ofd.sfs:	h.dir
ofd.sfs:	h.cat
ofd.sfs:	h.fs
ofd.sfs:	h.sff
ofd.sfs:	h.link
ofd.sfs:	h.mem
ofd.sfs:	h.fx
ofd.sfs:	h.scrap
ofd.sfs:	h.service
ofd.sfs:	h.xstr
ofd.sfs:	h.csfs
ofd.sfs:	h.sfs
ofd.sff:	c.sff
ofd.sff:	C:h.kernel
ofd.sff:	C:h.swis
ofd.sff:	h.Consts
ofd.sff:	C:debug.h.file
ofd.sff:	h.RMInfo
ofd.sff:	h.veneers
ofd.sff:	h.Interface
ofd.sff:	h.fsentries
ofd.sff:	h.ModuleWrap
ofd.sff:	h.flex
ofd.sff:	h.xstr
ofd.sff:	h.str
ofd.sff:	h.etc
ofd.sff:	h.csfserr
ofd.sff:	h.fs
ofd.sff:	h.arcs
ofd.sff:	h.arcf
ofd.sff:	h.dir
ofd.sff:	h.cat
ofd.sff:	h.fx
ofd.sff:	h.link
ofd.sff:	h.csfs
ofd.sff:	h.service
ofd.sff:	h.sfs
ofd.sff:	h.sff
ofd.sfi:	c.sfi
ofd.sfi:	C:h.kernel
ofd.sfi:	C:h.swis
ofd.sfi:	h.Consts
ofd.sfi:	C:debug.h.file
ofd.sfi:	h.RMInfo
ofd.sfi:	h.veneers
ofd.sfi:	h.Interface
ofd.sfi:	h.fsentries
ofd.sfi:	h.ModuleWrap
ofd.sfi:	h.flex
ofd.sfi:	h.str
ofd.sfi:	h.csfserr
ofd.sfi:	h.fs
ofd.sfi:	h.arcs
ofd.sfi:	h.arcf
ofd.sfi:	h.dir
ofd.sfi:	h.sfs
ofd.sfi:	h.sff
ofd.sfi:	h.fx
ofd.sfi:	h.sfi
ofd.arcs:	c.arcs
ofd.arcs:	C:h.kernel
ofd.arcs:	C:h.swis
ofd.arcs:	h.Consts
ofd.arcs:	C:debug.h.file
ofd.arcs:	h.RMInfo
ofd.arcs:	h.veneers
ofd.arcs:	h.Interface
ofd.arcs:	h.fsentries
ofd.arcs:	h.ModuleWrap
ofd.arcs:	h.flex
ofd.arcs:	h.csfserr
ofd.arcs:	h.str
ofd.arcs:	h.etc
ofd.arcs:	h.scrap
ofd.arcs:	h.xstr
ofd.arcs:	h.fs
ofd.arcs:	h.sfs
ofd.arcs:	h.fx
ofd.arcs:	h.cat
ofd.arcs:	h.link
ofd.arcs:	h.arcs
ofd.arcf:	c.arcf
ofd.arcf:	C:h.kernel
ofd.arcf:	C:h.swis
ofd.arcf:	h.Consts
ofd.arcf:	C:debug.h.file
ofd.arcf:	h.RMInfo
ofd.arcf:	h.veneers
ofd.arcf:	h.Interface
ofd.arcf:	h.fsentries
ofd.arcf:	h.ModuleWrap
ofd.arcf:	h.flex
ofd.arcf:	h.csfserr
ofd.arcf:	h.str
ofd.arcf:	h.fs
ofd.arcf:	h.sfs
ofd.arcf:	h.arcs
ofd.arcf:	h.cat
ofd.arcf:	h.link
ofd.arcf:	h.xstr
ofd.arcf:	h.csfs
ofd.arcf:	h.service
ofd.arcf:	h.arcf
ofd.dir:	c.dir
ofd.dir:	C:h.kernel
ofd.dir:	C:h.swis
ofd.dir:	h.Consts
ofd.dir:	C:debug.h.file
ofd.dir:	h.RMInfo
ofd.dir:	h.veneers
ofd.dir:	h.Interface
ofd.dir:	h.fsentries
ofd.dir:	h.ModuleWrap
ofd.dir:	h.flex
ofd.dir:	h.csfserr
ofd.dir:	h.str
ofd.dir:	h.fs
ofd.dir:	h.sfs
ofd.dir:	h.arcs
ofd.dir:	h.cat
ofd.dir:	h.link
ofd.dir:	h.xstr
ofd.dir:	h.dir
ofd.cat:	c.cat
ofd.cat:	C:h.kernel
ofd.cat:	C:h.swis
ofd.cat:	h.Consts
ofd.cat:	C:debug.h.file
ofd.cat:	h.RMInfo
ofd.cat:	h.veneers
ofd.cat:	h.Interface
ofd.cat:	h.fsentries
ofd.cat:	h.ModuleWrap
ofd.cat:	h.flex
ofd.cat:	h.str
ofd.cat:	h.fs
ofd.cat:	h.sfs
ofd.cat:	h.arcs
ofd.cat:	h.fx
ofd.cat:	h.cat
ofd.flex:	c.flex
ofd.flex:	C:h.kernel
ofd.flex:	C:h.swis
ofd.flex:	h.mem
ofd.flex:	h.csfserr
ofd.flex:	h.flex
ofd.fx:	c.fx
ofd.fx:	C:h.kernel
ofd.fx:	C:h.swis
ofd.fx:	h.Consts
ofd.fx:	C:debug.h.file
ofd.fx:	h.RMInfo
ofd.fx:	h.veneers
ofd.fx:	h.Interface
ofd.fx:	h.fsentries
ofd.fx:	h.ModuleWrap
ofd.fx:	h.flex
ofd.fx:	h.str
ofd.fx:	h.etc
ofd.fx:	h.arcs
ofd.fx:	h.sfs
ofd.fx:	h.cat
ofd.fx:	h.fx
ofd.fx:	h.fs
ofd.fx:	h.link
ofd.fx:	h.scrap
ofd.mem:	c.mem
ofd.mem:	C:h.kernel
ofd.mem:	C:h.swis
ofd.mem:	h.consts
ofd.mem:	C:debug.h.file
ofd.mem:	h.etc
ofd.mem:	h.csfserr
ofd.mem:	h.flex
ofd.mem:	h.mem
ofd.scrap:	c.scrap
ofd.scrap:	C:h.kernel
ofd.scrap:	C:h.swis
ofd.scrap:	h.consts
ofd.scrap:	C:debug.h.file
ofd.scrap:	h.etc
ofd.scrap:	h.str
ofd.scrap:	h.fs
ofd.scrap:	h.flex
ofd.scrap:	h.scrap
ofd.mount:	c.mount
ofd.mount:	C:h.kernel
ofd.mount:	C:h.swis
ofd.mount:	TCPIPLibs:h.inetlib
ofd.mount:	TCPIPLibs:sys.h.types
ofd.mount:	TCPIPLibs:sys.h.cdefs
ofd.mount:	TCPIPLibs:machine.h.endian
ofd.mount:	TCPIPLibs:machine.h.ansi
ofd.mount:	TCPIPLibs:machine.h.types
ofd.mount:	TCPIPLibs:netinet.h.in
ofd.mount:	TCPIPLibs:h.netdb
ofd.mount:	TCPIPLibs:sys.h.cdefs
ofd.mount:	h.etc
ofd.mount:	h.csfserr
ofd.mount:	h.consts
ofd.mount:	C:debug.h.file
ofd.mount:	h.flex
ofd.mount:	h.xstr
ofd.mount:	h.fs
ofd.mount:	h.str
ofd.mount:	h.csfs
ofd.mount:	h.link
ofd.mount:	h.arcs
ofd.mount:	h.fx
ofd.mount:	h.cat
ofd.mount:	h.dir
ofd.mount:	h.service
ofd.mount:	h.mount
ofd.service:	c.service
ofd.service:	C:h.kernel
ofd.service:	C:h.swis
ofd.service:	h.Consts
ofd.service:	C:debug.h.file
ofd.service:	h.RMInfo
ofd.service:	h.veneers
ofd.service:	h.Interface
ofd.service:	h.fsentries
ofd.service:	h.ModuleWrap
ofd.service:	h.flex
ofd.service:	h.csfserr
ofd.service:	h.xstr
ofd.service:	h.str
ofd.service:	h.etc
ofd.service:	h.csfs
ofd.service:	h.scrap
ofd.service:	h.fs
ofd.service:	h.sfs
ofd.service:	h.arcs
ofd.service:	h.cat
ofd.service:	h.fx
ofd.service:	h.mount
ofd.service:	h.service
ofd.service:	h.usermode
ofd.service:	C:h.kernel
ofd.serx:	c.serx
ofd.serx:	C:h.kernel
ofd.serx:	C:h.swis
ofd.serx:	h.Consts
ofd.serx:	C:debug.h.file
ofd.serx:	h.RMInfo
ofd.serx:	h.veneers
ofd.serx:	h.Interface
ofd.serx:	h.fsentries
ofd.serx:	h.ModuleWrap
ofd.serx:	h.flex
ofd.serx:	h.xstr
ofd.serx:	h.str
ofd.serx:	h.etc
ofd.serx:	h.scrap
ofd.serx:	h.fs
ofd.serx:	h.sfs
ofd.serx:	h.arcs
ofd.serx:	h.cat
ofd.serx:	h.fx
ofd.serx:	h.dir
ofd.serx:	h.csfs
ofd.serx:	h.link
ofd.serx:	h.serx
ofd.serx:	h.service
ofd.update:	c.update
ofd.update:	C:h.kernel
ofd.update:	C:h.swis
ofd.update:	h.Consts
ofd.update:	C:debug.h.file
ofd.update:	h.RMInfo
ofd.update:	h.veneers
ofd.update:	h.Interface
ofd.update:	h.fsentries
ofd.update:	h.ModuleWrap
ofd.update:	h.flex
ofd.update:	h.xstr
ofd.update:	h.csfserr
ofd.update:	h.str
ofd.update:	h.csfs
ofd.update:	h.etc
ofd.update:	h.arcs
ofd.update:	h.dir
ofd.update:	h.cat
ofd.update:	h.fs
ofd.update:	h.sff
ofd.update:	h.sfs
ofd.update:	h.link
ofd.update:	h.mem
ofd.update:	h.fx
ofd.update:	h.scrap
ofd.update:	h.service
ofd.update:	h.serx
ofd.update:	h.mount
ofd.update:	h.update
ofd.fs:	c.fs
ofd.fs:	C:h.kernel
ofd.fs:	C:h.swis
ofd.fs:	h.Consts
ofd.fs:	C:debug.h.file
ofd.fs:	h.RMInfo
ofd.fs:	h.veneers
ofd.fs:	h.Interface
ofd.fs:	h.fsentries
ofd.fs:	h.ModuleWrap
ofd.fs:	h.str
ofd.fs:	h.fs
ofd.interface: s.interface
ofd.link:	c.link
ofd.link:	C:h.kernel
ofd.link:	C:h.swis
ofd.link:	h.Consts
ofd.link:	C:debug.h.file
ofd.link:	h.RMInfo
ofd.link:	h.veneers
ofd.link:	h.Interface
ofd.link:	h.fsentries
ofd.link:	h.ModuleWrap
ofd.link:	h.flex
ofd.link:	h.xstr
ofd.link:	h.str
ofd.link:	h.etc
ofd.link:	h.scrap
ofd.link:	h.fs
ofd.link:	h.sfs
ofd.link:	h.arcs
ofd.link:	h.cat
ofd.link:	h.fx
ofd.link:	h.mount
ofd.link:	h.serx
ofd.link:	h.link
ofd.open:	c.open
ofd.open:	C:h.kernel
ofd.open:	C:h.swis
ofd.open:	h.Consts
ofd.open:	C:debug.h.file
ofd.open:	h.RMInfo
ofd.open:	h.veneers
ofd.open:	h.Interface
ofd.open:	h.fsentries
ofd.open:	h.ModuleWrap
ofd.open:	h.str
ofd.open:	h.csfserr
ofd.open:	h.fs
ofd.open:	h.mem
ofd.open:	h.sfs
ofd.open:	h.arcs
ofd.open:	h.sff
ofd.open:	h.sfi
ofd.gbpb:	c.gbpb
ofd.gbpb:	C:h.kernel
ofd.gbpb:	C:h.swis
ofd.gbpb:	h.Consts
ofd.gbpb:	C:debug.h.file
ofd.gbpb:	h.RMInfo
ofd.gbpb:	h.veneers
ofd.gbpb:	h.Interface
ofd.gbpb:	h.fsentries
ofd.gbpb:	h.ModuleWrap
ofd.gbpb:	h.etc
ofd.gbpb:	h.mem
ofd.gbpb:	h.sfi
ofd.func:	c.func
ofd.func:	C:h.kernel
ofd.func:	C:h.swis
ofd.func:	h.Consts
ofd.func:	C:debug.h.file
ofd.func:	h.RMInfo
ofd.func:	h.veneers
ofd.func:	h.Interface
ofd.func:	h.fsentries
ofd.func:	h.ModuleWrap
ofd.func:	h.str
ofd.func:	h.csfserr
ofd.func:	h.mem
ofd.func:	h.sfs
ofd.file:	c.file
ofd.file:	C:h.kernel
ofd.file:	C:h.swis
ofd.file:	h.Consts
ofd.file:	C:debug.h.file
ofd.file:	h.RMInfo
ofd.file:	h.veneers
ofd.file:	h.Interface
ofd.file:	h.fsentries
ofd.file:	h.ModuleWrap
ofd.file:	h.str
ofd.file:	h.csfserr
ofd.file:	h.fs
ofd.file:	h.mem
ofd.file:	h.sfs
ofd.file:	h.arcs
ofd.file:	h.sff
ofd.close:	c.close
ofd.close:	C:h.kernel
ofd.close:	C:h.swis
ofd.close:	h.Consts
ofd.close:	C:debug.h.file
ofd.close:	h.RMInfo
ofd.close:	h.veneers
ofd.close:	h.Interface
ofd.close:	h.fsentries
ofd.close:	h.ModuleWrap
ofd.close:	h.mem
ofd.close:	h.sfi
ofd.args:	c.args
ofd.args:	C:h.kernel
ofd.args:	C:h.swis
ofd.args:	h.Consts
ofd.args:	C:debug.h.file
ofd.args:	h.RMInfo
ofd.args:	h.veneers
ofd.args:	h.Interface
ofd.args:	h.fsentries
ofd.args:	h.ModuleWrap
ofd.args:	h.mem
ofd.args:	h.sfi
ofd.args:	h.csfserr
ofd.getbytes:	c.getbytes
ofd.getbytes:	C:h.kernel
ofd.getbytes:	C:h.swis
ofd.getbytes:	h.Consts
ofd.getbytes:	C:debug.h.file
ofd.getbytes:	h.RMInfo
ofd.getbytes:	h.veneers
ofd.getbytes:	h.Interface
ofd.getbytes:	h.fsentries
ofd.getbytes:	h.ModuleWrap
ofd.getbytes:	h.mem
ofd.getbytes:	h.sfi
ofd.putbytes:	c.putbytes
ofd.putbytes:	C:h.kernel
ofd.putbytes:	C:h.swis
ofd.putbytes:	h.Consts
ofd.putbytes:	C:debug.h.file
ofd.putbytes:	h.RMInfo
ofd.putbytes:	h.veneers
ofd.putbytes:	h.Interface
ofd.putbytes:	h.fsentries
ofd.putbytes:	h.ModuleWrap
ofd.putbytes:	h.mem
ofd.putbytes:	h.sfi
ofd.xstr:	c.xstr
ofd.xstr:	C:h.kernel
ofd.xstr:	C:h.swis
ofd.xstr:	h.consts
ofd.xstr:	C:debug.h.file
ofd.xstr:	h.flex
ofd.xstr:	h.xstr
ofd.usermode: s.usermode
ofd.modulewrap:	c.modulewrap
ofd.modulewrap:	C:h.kernel
ofd.modulewrap:	C:h.swis
ofd.modulewrap:	h.Consts
ofd.modulewrap:	C:debug.h.file
ofd.modulewrap:	h.RMInfo
ofd.modulewrap:	h.veneers
ofd.modulewrap:	h.Interface
ofd.modulewrap:	h.fsentries
ofd.modulewrap:	h.modulewrap
ofd.modulewrap:	h.resources
ofd.modulewrap:	h.str
ofd.modulewrap:	h.etc
ofd.modulewrap:	h.csfserr
ofd.modulewrap:	h.fs
ofd.modulewrap:	h.sfs
ofd.modulewrap:	h.arcs
ofd.modulewrap:	h.cat
ofd.modulewrap:	h.link
ofd.modulewrap:	h.mem
ofd.modulewrap:	h.mount
ofd.modulewrap:	h.update
ofd.modulewrap:	h.service
o.RMInfo: s.RMInfo
o.csfserr:	c.csfserr
o.csfserr:	C:h.kernel
o.csfserr:	C:h.swis
o.csfserr:	h.etc
o.csfserr:	h.csfserr
o.flex:	c.flex
o.flex:	C:h.kernel
o.flex:	C:h.swis
o.flex:	h.mem
o.flex:	h.csfserr
o.flex:	h.flex
o.mem:	c.mem
o.mem:	C:h.kernel
o.mem:	C:h.swis
o.mem:	h.consts
o.mem:	h.etc
o.mem:	h.csfserr
o.mem:	h.flex
o.mem:	h.mem
o.scrap:	c.scrap
o.scrap:	C:h.kernel
o.scrap:	C:h.swis
o.scrap:	h.consts
o.scrap:	h.etc
o.scrap:	h.str
o.scrap:	h.fs
o.scrap:	h.flex
o.scrap:	h.scrap
o.mount:	c.mount
o.mount:	C:h.kernel
o.mount:	C:h.swis
o.mount:	TCPIPLibs:h.inetlib
o.mount:	TCPIPLibs:sys.h.types
o.mount:	TCPIPLibs:sys.h.cdefs
o.mount:	TCPIPLibs:machine.h.endian
o.mount:	TCPIPLibs:machine.h.ansi
o.mount:	TCPIPLibs:machine.h.types
o.mount:	TCPIPLibs:netinet.h.in
o.mount:	TCPIPLibs:h.netdb
o.mount:	TCPIPLibs:sys.h.cdefs
o.mount:	h.etc
o.mount:	h.csfserr
o.mount:	h.consts
o.mount:	h.flex
o.mount:	h.xstr
o.mount:	h.fs
o.mount:	h.str
o.mount:	h.csfs
o.mount:	h.link
o.mount:	h.arcs
o.mount:	h.fx
o.mount:	h.cat
o.mount:	h.dir
o.mount:	h.service
o.mount:	h.mount
o.interface: s.interface
o.xstr:	c.xstr
o.xstr:	C:h.kernel
o.xstr:	C:h.swis
o.xstr:	h.consts
o.xstr:	h.flex
o.xstr:	h.xstr
ord.resources: s.resources
ord.resources: hdr:ListOpts
ord.resources: hdr:Macros
ord.resources: hdr:System
ord.resources: Hdr:SWIs
ord.resources: Hdr:CPU.Generic26
ord.resources: Hdr:IO.GenericIO
ord.resources: Hdr:RISCOS
ord.resources: hdr:ModHand
ord.resources: hdr:ResourceFS
ord.resources: hdr:Proc
ord.RMInfo: s.RMInfo
ord.csfserr:	c.csfserr
ord.csfserr:	C:h.kernel
ord.csfserr:	C:h.swis
ord.csfserr:	h.etc
ord.csfserr:	h.csfserr
ord.sfs:	c.sfs
ord.sfs:	C:h.kernel
ord.sfs:	C:h.swis
ord.sfs:	h.Consts
ord.sfs:	C:debug.h.remote
ord.sfs:	h.RMInfo
ord.sfs:	h.veneers
ord.sfs:	h.Interface
ord.sfs:	h.fsentries
ord.sfs:	h.ModuleWrap
ord.sfs:	h.flex
ord.sfs:	h.csfserr
ord.sfs:	h.str
ord.sfs:	h.etc
ord.sfs:	h.arcs
ord.sfs:	h.dir
ord.sfs:	h.cat
ord.sfs:	h.fs
ord.sfs:	h.sff
ord.sfs:	h.link
ord.sfs:	h.mem
ord.sfs:	h.fx
ord.sfs:	h.scrap
ord.sfs:	h.service
ord.sfs:	h.xstr
ord.sfs:	h.csfs
ord.sfs:	h.sfs
ord.sff:	c.sff
ord.sff:	C:h.kernel
ord.sff:	C:h.swis
ord.sff:	h.Consts
ord.sff:	C:debug.h.remote
ord.sff:	h.RMInfo
ord.sff:	h.veneers
ord.sff:	h.Interface
ord.sff:	h.fsentries
ord.sff:	h.ModuleWrap
ord.sff:	h.flex
ord.sff:	h.xstr
ord.sff:	h.str
ord.sff:	h.etc
ord.sff:	h.csfserr
ord.sff:	h.fs
ord.sff:	h.arcs
ord.sff:	h.arcf
ord.sff:	h.dir
ord.sff:	h.cat
ord.sff:	h.fx
ord.sff:	h.link
ord.sff:	h.csfs
ord.sff:	h.service
ord.sff:	h.sfs
ord.sff:	h.sff
ord.sfi:	c.sfi
ord.sfi:	C:h.kernel
ord.sfi:	C:h.swis
ord.sfi:	h.Consts
ord.sfi:	C:debug.h.remote
ord.sfi:	h.RMInfo
ord.sfi:	h.veneers
ord.sfi:	h.Interface
ord.sfi:	h.fsentries
ord.sfi:	h.ModuleWrap
ord.sfi:	h.flex
ord.sfi:	h.str
ord.sfi:	h.csfserr
ord.sfi:	h.fs
ord.sfi:	h.arcs
ord.sfi:	h.arcf
ord.sfi:	h.dir
ord.sfi:	h.sfs
ord.sfi:	h.sff
ord.sfi:	h.fx
ord.sfi:	h.sfi
ord.arcs:	c.arcs
ord.arcs:	C:h.kernel
ord.arcs:	C:h.swis
ord.arcs:	h.Consts
ord.arcs:	C:debug.h.remote
ord.arcs:	h.RMInfo
ord.arcs:	h.veneers
ord.arcs:	h.Interface
ord.arcs:	h.fsentries
ord.arcs:	h.ModuleWrap
ord.arcs:	h.flex
ord.arcs:	h.csfserr
ord.arcs:	h.str
ord.arcs:	h.etc
ord.arcs:	h.scrap
ord.arcs:	h.xstr
ord.arcs:	h.fs
ord.arcs:	h.sfs
ord.arcs:	h.fx
ord.arcs:	h.cat
ord.arcs:	h.link
ord.arcs:	h.arcs
ord.arcf:	c.arcf
ord.arcf:	C:h.kernel
ord.arcf:	C:h.swis
ord.arcf:	h.Consts
ord.arcf:	C:debug.h.remote
ord.arcf:	h.RMInfo
ord.arcf:	h.veneers
ord.arcf:	h.Interface
ord.arcf:	h.fsentries
ord.arcf:	h.ModuleWrap
ord.arcf:	h.flex
ord.arcf:	h.csfserr
ord.arcf:	h.str
ord.arcf:	h.fs
ord.arcf:	h.sfs
ord.arcf:	h.arcs
ord.arcf:	h.cat
ord.arcf:	h.link
ord.arcf:	h.xstr
ord.arcf:	h.csfs
ord.arcf:	h.service
ord.arcf:	h.arcf
ord.dir:	c.dir
ord.dir:	C:h.kernel
ord.dir:	C:h.swis
ord.dir:	h.Consts
ord.dir:	C:debug.h.remote
ord.dir:	h.RMInfo
ord.dir:	h.veneers
ord.dir:	h.Interface
ord.dir:	h.fsentries
ord.dir:	h.ModuleWrap
ord.dir:	h.flex
ord.dir:	h.csfserr
ord.dir:	h.str
ord.dir:	h.fs
ord.dir:	h.sfs
ord.dir:	h.arcs
ord.dir:	h.cat
ord.dir:	h.link
ord.dir:	h.xstr
ord.dir:	h.dir
ord.cat:	c.cat
ord.cat:	C:h.kernel
ord.cat:	C:h.swis
ord.cat:	h.Consts
ord.cat:	C:debug.h.remote
ord.cat:	h.RMInfo
ord.cat:	h.veneers
ord.cat:	h.Interface
ord.cat:	h.fsentries
ord.cat:	h.ModuleWrap
ord.cat:	h.flex
ord.cat:	h.str
ord.cat:	h.fs
ord.cat:	h.sfs
ord.cat:	h.arcs
ord.cat:	h.fx
ord.cat:	h.cat
ord.flex:	c.flex
ord.flex:	C:h.kernel
ord.flex:	C:h.swis
ord.flex:	h.mem
ord.flex:	h.csfserr
ord.flex:	h.flex
ord.fx:	c.fx
ord.fx:	C:h.kernel
ord.fx:	C:h.swis
ord.fx:	h.Consts
ord.fx:	C:debug.h.remote
ord.fx:	h.RMInfo
ord.fx:	h.veneers
ord.fx:	h.Interface
ord.fx:	h.fsentries
ord.fx:	h.ModuleWrap
ord.fx:	h.flex
ord.fx:	h.str
ord.fx:	h.etc
ord.fx:	h.arcs
ord.fx:	h.sfs
ord.fx:	h.cat
ord.fx:	h.fx
ord.fx:	h.fs
ord.fx:	h.link
ord.fx:	h.scrap
ord.mem:	c.mem
ord.mem:	C:h.kernel
ord.mem:	C:h.swis
ord.mem:	h.consts
ord.mem:	C:debug.h.remote
ord.mem:	h.etc
ord.mem:	h.csfserr
ord.mem:	h.flex
ord.mem:	h.mem
ord.scrap:	c.scrap
ord.scrap:	C:h.kernel
ord.scrap:	C:h.swis
ord.scrap:	h.consts
ord.scrap:	C:debug.h.remote
ord.scrap:	h.etc
ord.scrap:	h.str
ord.scrap:	h.fs
ord.scrap:	h.flex
ord.scrap:	h.scrap
ord.mount:	c.mount
ord.mount:	C:h.kernel
ord.mount:	C:h.swis
ord.mount:	TCPIPLibs:h.inetlib
ord.mount:	TCPIPLibs:sys.h.types
ord.mount:	TCPIPLibs:sys.h.cdefs
ord.mount:	TCPIPLibs:machine.h.endian
ord.mount:	TCPIPLibs:machine.h.ansi
ord.mount:	TCPIPLibs:machine.h.types
ord.mount:	TCPIPLibs:netinet.h.in
ord.mount:	TCPIPLibs:h.netdb
ord.mount:	TCPIPLibs:sys.h.cdefs
ord.mount:	h.etc
ord.mount:	h.csfserr
ord.mount:	h.consts
ord.mount:	C:debug.h.remote
ord.mount:	h.flex
ord.mount:	h.xstr
ord.mount:	h.fs
ord.mount:	h.str
ord.mount:	h.csfs
ord.mount:	h.link
ord.mount:	h.arcs
ord.mount:	h.fx
ord.mount:	h.cat
ord.mount:	h.dir
ord.mount:	h.service
ord.mount:	h.mount
ord.service:	c.service
ord.service:	C:h.kernel
ord.service:	C:h.swis
ord.service:	h.Consts
ord.service:	C:debug.h.remote
ord.service:	h.RMInfo
ord.service:	h.veneers
ord.service:	h.Interface
ord.service:	h.fsentries
ord.service:	h.ModuleWrap
ord.service:	h.flex
ord.service:	h.csfserr
ord.service:	h.xstr
ord.service:	h.str
ord.service:	h.etc
ord.service:	h.csfs
ord.service:	h.scrap
ord.service:	h.fs
ord.service:	h.sfs
ord.service:	h.arcs
ord.service:	h.cat
ord.service:	h.fx
ord.service:	h.mount
ord.service:	h.service
ord.service:	h.usermode
ord.service:	C:h.kernel
ord.serx:	c.serx
ord.serx:	C:h.kernel
ord.serx:	C:h.swis
ord.serx:	h.Consts
ord.serx:	C:debug.h.remote
ord.serx:	h.RMInfo
ord.serx:	h.veneers
ord.serx:	h.Interface
ord.serx:	h.fsentries
ord.serx:	h.ModuleWrap
ord.serx:	h.flex
ord.serx:	h.xstr
ord.serx:	h.str
ord.serx:	h.etc
ord.serx:	h.scrap
ord.serx:	h.fs
ord.serx:	h.sfs
ord.serx:	h.arcs
ord.serx:	h.cat
ord.serx:	h.fx
ord.serx:	h.dir
ord.serx:	h.csfs
ord.serx:	h.link
ord.serx:	h.serx
ord.serx:	h.service
ord.update:	c.update
ord.update:	C:h.kernel
ord.update:	C:h.swis
ord.update:	h.Consts
ord.update:	C:debug.h.remote
ord.update:	h.RMInfo
ord.update:	h.veneers
ord.update:	h.Interface
ord.update:	h.fsentries
ord.update:	h.ModuleWrap
ord.update:	h.flex
ord.update:	h.xstr
ord.update:	h.csfserr
ord.update:	h.str
ord.update:	h.csfs
ord.update:	h.etc
ord.update:	h.arcs
ord.update:	h.dir
ord.update:	h.cat
ord.update:	h.fs
ord.update:	h.sff
ord.update:	h.sfs
ord.update:	h.link
ord.update:	h.mem
ord.update:	h.fx
ord.update:	h.scrap
ord.update:	h.service
ord.update:	h.serx
ord.update:	h.mount
ord.update:	h.update
ord.fs:	c.fs
ord.fs:	C:h.kernel
ord.fs:	C:h.swis
ord.fs:	h.Consts
ord.fs:	C:debug.h.remote
ord.fs:	h.RMInfo
ord.fs:	h.veneers
ord.fs:	h.Interface
ord.fs:	h.fsentries
ord.fs:	h.ModuleWrap
ord.fs:	h.str
ord.fs:	h.fs
ord.interface: s.interface
ord.link:	c.link
ord.link:	C:h.kernel
ord.link:	C:h.swis
ord.link:	h.Consts
ord.link:	C:debug.h.remote
ord.link:	h.RMInfo
ord.link:	h.veneers
ord.link:	h.Interface
ord.link:	h.fsentries
ord.link:	h.ModuleWrap
ord.link:	h.flex
ord.link:	h.xstr
ord.link:	h.str
ord.link:	h.etc
ord.link:	h.scrap
ord.link:	h.fs
ord.link:	h.sfs
ord.link:	h.arcs
ord.link:	h.cat
ord.link:	h.fx
ord.link:	h.mount
ord.link:	h.serx
ord.link:	h.link
ord.open:	c.open
ord.open:	C:h.kernel
ord.open:	C:h.swis
ord.open:	h.Consts
ord.open:	C:debug.h.remote
ord.open:	h.RMInfo
ord.open:	h.veneers
ord.open:	h.Interface
ord.open:	h.fsentries
ord.open:	h.ModuleWrap
ord.open:	h.str
ord.open:	h.csfserr
ord.open:	h.fs
ord.open:	h.mem
ord.open:	h.sfs
ord.open:	h.arcs
ord.open:	h.sff
ord.open:	h.sfi
ord.gbpb:	c.gbpb
ord.gbpb:	C:h.kernel
ord.gbpb:	C:h.swis
ord.gbpb:	h.Consts
ord.gbpb:	C:debug.h.remote
ord.gbpb:	h.RMInfo
ord.gbpb:	h.veneers
ord.gbpb:	h.Interface
ord.gbpb:	h.fsentries
ord.gbpb:	h.ModuleWrap
ord.gbpb:	h.etc
ord.gbpb:	h.mem
ord.gbpb:	h.sfi
ord.func:	c.func
ord.func:	C:h.kernel
ord.func:	C:h.swis
ord.func:	h.Consts
ord.func:	C:debug.h.remote
ord.func:	h.RMInfo
ord.func:	h.veneers
ord.func:	h.Interface
ord.func:	h.fsentries
ord.func:	h.ModuleWrap
ord.func:	h.str
ord.func:	h.csfserr
ord.func:	h.mem
ord.func:	h.sfs
ord.file:	c.file
ord.file:	C:h.kernel
ord.file:	C:h.swis
ord.file:	h.Consts
ord.file:	C:debug.h.remote
ord.file:	h.RMInfo
ord.file:	h.veneers
ord.file:	h.Interface
ord.file:	h.fsentries
ord.file:	h.ModuleWrap
ord.file:	h.str
ord.file:	h.csfserr
ord.file:	h.fs
ord.file:	h.mem
ord.file:	h.sfs
ord.file:	h.arcs
ord.file:	h.sff
ord.close:	c.close
ord.close:	C:h.kernel
ord.close:	C:h.swis
ord.close:	h.Consts
ord.close:	C:debug.h.remote
ord.close:	h.RMInfo
ord.close:	h.veneers
ord.close:	h.Interface
ord.close:	h.fsentries
ord.close:	h.ModuleWrap
ord.close:	h.mem
ord.close:	h.sfi
ord.args:	c.args
ord.args:	C:h.kernel
ord.args:	C:h.swis
ord.args:	h.Consts
ord.args:	C:debug.h.remote
ord.args:	h.RMInfo
ord.args:	h.veneers
ord.args:	h.Interface
ord.args:	h.fsentries
ord.args:	h.ModuleWrap
ord.args:	h.mem
ord.args:	h.sfi
ord.args:	h.csfserr
ord.getbytes:	c.getbytes
ord.getbytes:	C:h.kernel
ord.getbytes:	C:h.swis
ord.getbytes:	h.Consts
ord.getbytes:	C:debug.h.remote
ord.getbytes:	h.RMInfo
ord.getbytes:	h.veneers
ord.getbytes:	h.Interface
ord.getbytes:	h.fsentries
ord.getbytes:	h.ModuleWrap
ord.getbytes:	h.mem
ord.getbytes:	h.sfi
ord.putbytes:	c.putbytes
ord.putbytes:	C:h.kernel
ord.putbytes:	C:h.swis
ord.putbytes:	h.Consts
ord.putbytes:	C:debug.h.remote
ord.putbytes:	h.RMInfo
ord.putbytes:	h.veneers
ord.putbytes:	h.Interface
ord.putbytes:	h.fsentries
ord.putbytes:	h.ModuleWrap
ord.putbytes:	h.mem
ord.putbytes:	h.sfi
ord.xstr:	c.xstr
ord.xstr:	C:h.kernel
ord.xstr:	C:h.swis
ord.xstr:	h.consts
ord.xstr:	C:debug.h.remote
ord.xstr:	h.flex
ord.xstr:	h.xstr
ord.usermode: s.usermode
ord.modulewrap:	c.modulewrap
ord.modulewrap:	C:h.kernel
ord.modulewrap:	C:h.swis
ord.modulewrap:	h.Consts
ord.modulewrap:	C:debug.h.remote
ord.modulewrap:	h.RMInfo
ord.modulewrap:	h.veneers
ord.modulewrap:	h.Interface
ord.modulewrap:	h.fsentries
ord.modulewrap:	h.modulewrap
ord.modulewrap:	h.resources
ord.modulewrap:	h.str
ord.modulewrap:	h.etc
ord.modulewrap:	h.csfserr
ord.modulewrap:	h.fs
ord.modulewrap:	h.sfs
ord.modulewrap:	h.arcs
ord.modulewrap:	h.cat
ord.modulewrap:	h.link
ord.modulewrap:	h.mem
ord.modulewrap:	h.mount
ord.modulewrap:	h.update
ord.modulewrap:	h.service
o.usermode: s.usermode
o.resources: s.resources
o.resources: hdr:ListOpts
o.resources: hdr:Macros
o.resources: hdr:System
o.resources: Hdr:SWIs
o.resources: Hdr:CPU.Generic26
o.resources: Hdr:IO.GenericIO
o.resources: Hdr:RISCOS
o.resources: hdr:ModHand
o.resources: hdr:ResourceFS
o.resources: hdr:Proc
o.sfs:	c.sfs
o.sfs:	C:h.kernel
o.sfs:	C:h.swis
o.sfs:	h.Consts
o.sfs:	h.RMInfo
o.sfs:	h.veneers
o.sfs:	h.Interface
o.sfs:	h.fsentries
o.sfs:	h.ModuleWrap
o.sfs:	h.flex
o.sfs:	h.csfserr
o.sfs:	h.str
o.sfs:	h.etc
o.sfs:	h.arcs
o.sfs:	h.dir
o.sfs:	h.cat
o.sfs:	h.fs
o.sfs:	h.sff
o.sfs:	h.link
o.sfs:	h.mem
o.sfs:	h.fx
o.sfs:	h.scrap
o.sfs:	h.service
o.sfs:	h.xstr
o.sfs:	h.csfs
o.sfs:	h.sfs
o.sff:	c.sff
o.sff:	C:h.kernel
o.sff:	C:h.swis
o.sff:	h.Consts
o.sff:	h.RMInfo
o.sff:	h.veneers
o.sff:	h.Interface
o.sff:	h.fsentries
o.sff:	h.ModuleWrap
o.sff:	h.flex
o.sff:	h.xstr
o.sff:	h.str
o.sff:	h.etc
o.sff:	h.csfserr
o.sff:	h.fs
o.sff:	h.arcs
o.sff:	h.arcf
o.sff:	h.dir
o.sff:	h.cat
o.sff:	h.fx
o.sff:	h.link
o.sff:	h.csfs
o.sff:	h.service
o.sff:	h.sfs
o.sff:	h.sff
o.sfi:	c.sfi
o.sfi:	C:h.kernel
o.sfi:	C:h.swis
o.sfi:	h.Consts
o.sfi:	h.RMInfo
o.sfi:	h.veneers
o.sfi:	h.Interface
o.sfi:	h.fsentries
o.sfi:	h.ModuleWrap
o.sfi:	h.flex
o.sfi:	h.str
o.sfi:	h.csfserr
o.sfi:	h.fs
o.sfi:	h.arcs
o.sfi:	h.arcf
o.sfi:	h.dir
o.sfi:	h.sfs
o.sfi:	h.sff
o.sfi:	h.fx
o.sfi:	h.sfi
o.arcs:	c.arcs
o.arcs:	C:h.kernel
o.arcs:	C:h.swis
o.arcs:	h.Consts
o.arcs:	h.RMInfo
o.arcs:	h.veneers
o.arcs:	h.Interface
o.arcs:	h.fsentries
o.arcs:	h.ModuleWrap
o.arcs:	h.flex
o.arcs:	h.csfserr
o.arcs:	h.str
o.arcs:	h.etc
o.arcs:	h.scrap
o.arcs:	h.xstr
o.arcs:	h.fs
o.arcs:	h.sfs
o.arcs:	h.fx
o.arcs:	h.cat
o.arcs:	h.link
o.arcs:	h.arcs
o.arcf:	c.arcf
o.arcf:	C:h.kernel
o.arcf:	C:h.swis
o.arcf:	h.Consts
o.arcf:	h.RMInfo
o.arcf:	h.veneers
o.arcf:	h.Interface
o.arcf:	h.fsentries
o.arcf:	h.ModuleWrap
o.arcf:	h.flex
o.arcf:	h.csfserr
o.arcf:	h.str
o.arcf:	h.fs
o.arcf:	h.sfs
o.arcf:	h.arcs
o.arcf:	h.cat
o.arcf:	h.link
o.arcf:	h.xstr
o.arcf:	h.csfs
o.arcf:	h.service
o.arcf:	h.arcf
o.dir:	c.dir
o.dir:	C:h.kernel
o.dir:	C:h.swis
o.dir:	h.Consts
o.dir:	h.RMInfo
o.dir:	h.veneers
o.dir:	h.Interface
o.dir:	h.fsentries
o.dir:	h.ModuleWrap
o.dir:	h.flex
o.dir:	h.csfserr
o.dir:	h.str
o.dir:	h.fs
o.dir:	h.sfs
o.dir:	h.arcs
o.dir:	h.cat
o.dir:	h.link
o.dir:	h.xstr
o.dir:	h.dir
o.cat:	c.cat
o.cat:	C:h.kernel
o.cat:	C:h.swis
o.cat:	h.Consts
o.cat:	h.RMInfo
o.cat:	h.veneers
o.cat:	h.Interface
o.cat:	h.fsentries
o.cat:	h.ModuleWrap
o.cat:	h.flex
o.cat:	h.str
o.cat:	h.fs
o.cat:	h.sfs
o.cat:	h.arcs
o.cat:	h.fx
o.cat:	h.cat
o.fx:	c.fx
o.fx:	C:h.kernel
o.fx:	C:h.swis
o.fx:	h.Consts
o.fx:	h.RMInfo
o.fx:	h.veneers
o.fx:	h.Interface
o.fx:	h.fsentries
o.fx:	h.ModuleWrap
o.fx:	h.flex
o.fx:	h.str
o.fx:	h.etc
o.fx:	h.arcs
o.fx:	h.sfs
o.fx:	h.cat
o.fx:	h.fx
o.fx:	h.fs
o.fx:	h.link
o.fx:	h.scrap
o.service:	c.service
o.service:	C:h.kernel
o.service:	C:h.swis
o.service:	h.Consts
o.service:	h.RMInfo
o.service:	h.veneers
o.service:	h.Interface
o.service:	h.fsentries
o.service:	h.ModuleWrap
o.service:	h.flex
o.service:	h.csfserr
o.service:	h.xstr
o.service:	h.str
o.service:	h.etc
o.service:	h.csfs
o.service:	h.scrap
o.service:	h.fs
o.service:	h.sfs
o.service:	h.arcs
o.service:	h.cat
o.service:	h.fx
o.service:	h.mount
o.service:	h.service
o.service:	h.usermode
o.service:	C:h.kernel
o.serx:	c.serx
o.serx:	C:h.kernel
o.serx:	C:h.swis
o.serx:	h.Consts
o.serx:	h.RMInfo
o.serx:	h.veneers
o.serx:	h.Interface
o.serx:	h.fsentries
o.serx:	h.ModuleWrap
o.serx:	h.flex
o.serx:	h.xstr
o.serx:	h.str
o.serx:	h.etc
o.serx:	h.scrap
o.serx:	h.fs
o.serx:	h.sfs
o.serx:	h.arcs
o.serx:	h.cat
o.serx:	h.fx
o.serx:	h.dir
o.serx:	h.csfs
o.serx:	h.link
o.serx:	h.serx
o.serx:	h.service
o.update:	c.update
o.update:	C:h.kernel
o.update:	C:h.swis
o.update:	h.Consts
o.update:	h.RMInfo
o.update:	h.veneers
o.update:	h.Interface
o.update:	h.fsentries
o.update:	h.ModuleWrap
o.update:	h.flex
o.update:	h.xstr
o.update:	h.csfserr
o.update:	h.str
o.update:	h.csfs
o.update:	h.etc
o.update:	h.arcs
o.update:	h.dir
o.update:	h.cat
o.update:	h.fs
o.update:	h.sff
o.update:	h.sfs
o.update:	h.link
o.update:	h.mem
o.update:	h.fx
o.update:	h.scrap
o.update:	h.service
o.update:	h.serx
o.update:	h.mount
o.update:	h.update
o.fs:	c.fs
o.fs:	C:h.kernel
o.fs:	C:h.swis
o.fs:	h.Consts
o.fs:	h.RMInfo
o.fs:	h.veneers
o.fs:	h.Interface
o.fs:	h.fsentries
o.fs:	h.ModuleWrap
o.fs:	h.str
o.fs:	h.fs
o.link:	c.link
o.link:	C:h.kernel
o.link:	C:h.swis
o.link:	h.Consts
o.link:	h.RMInfo
o.link:	h.veneers
o.link:	h.Interface
o.link:	h.fsentries
o.link:	h.ModuleWrap
o.link:	h.flex
o.link:	h.xstr
o.link:	h.str
o.link:	h.etc
o.link:	h.scrap
o.link:	h.fs
o.link:	h.sfs
o.link:	h.arcs
o.link:	h.cat
o.link:	h.fx
o.link:	h.mount
o.link:	h.serx
o.link:	h.link
o.open:	c.open
o.open:	C:h.kernel
o.open:	C:h.swis
o.open:	h.Consts
o.open:	h.RMInfo
o.open:	h.veneers
o.open:	h.Interface
o.open:	h.fsentries
o.open:	h.ModuleWrap
o.open:	h.str
o.open:	h.csfserr
o.open:	h.fs
o.open:	h.mem
o.open:	h.sfs
o.open:	h.arcs
o.open:	h.sff
o.open:	h.sfi
o.gbpb:	c.gbpb
o.gbpb:	C:h.kernel
o.gbpb:	C:h.swis
o.gbpb:	h.Consts
o.gbpb:	h.RMInfo
o.gbpb:	h.veneers
o.gbpb:	h.Interface
o.gbpb:	h.fsentries
o.gbpb:	h.ModuleWrap
o.gbpb:	h.etc
o.gbpb:	h.mem
o.gbpb:	h.sfi
o.func:	c.func
o.func:	C:h.kernel
o.func:	C:h.swis
o.func:	h.Consts
o.func:	h.RMInfo
o.func:	h.veneers
o.func:	h.Interface
o.func:	h.fsentries
o.func:	h.ModuleWrap
o.func:	h.str
o.func:	h.csfserr
o.func:	h.mem
o.func:	h.sfs
o.file:	c.file
o.file:	C:h.kernel
o.file:	C:h.swis
o.file:	h.Consts
o.file:	h.RMInfo
o.file:	h.veneers
o.file:	h.Interface
o.file:	h.fsentries
o.file:	h.ModuleWrap
o.file:	h.str
o.file:	h.csfserr
o.file:	h.fs
o.file:	h.mem
o.file:	h.sfs
o.file:	h.arcs
o.file:	h.sff
o.close:	c.close
o.close:	C:h.kernel
o.close:	C:h.swis
o.close:	h.Consts
o.close:	h.RMInfo
o.close:	h.veneers
o.close:	h.Interface
o.close:	h.fsentries
o.close:	h.ModuleWrap
o.close:	h.mem
o.close:	h.sfi
o.args:	c.args
o.args:	C:h.kernel
o.args:	C:h.swis
o.args:	h.Consts
o.args:	h.RMInfo
o.args:	h.veneers
o.args:	h.Interface
o.args:	h.fsentries
o.args:	h.ModuleWrap
o.args:	h.mem
o.args:	h.sfi
o.args:	h.csfserr
o.getbytes:	c.getbytes
o.getbytes:	C:h.kernel
o.getbytes:	C:h.swis
o.getbytes:	h.Consts
o.getbytes:	h.RMInfo
o.getbytes:	h.veneers
o.getbytes:	h.Interface
o.getbytes:	h.fsentries
o.getbytes:	h.ModuleWrap
o.getbytes:	h.mem
o.getbytes:	h.sfi
o.putbytes:	c.putbytes
o.putbytes:	C:h.kernel
o.putbytes:	C:h.swis
o.putbytes:	h.Consts
o.putbytes:	h.RMInfo
o.putbytes:	h.veneers
o.putbytes:	h.Interface
o.putbytes:	h.fsentries
o.putbytes:	h.ModuleWrap
o.putbytes:	h.mem
o.putbytes:	h.sfi
o.modulewrap:	c.modulewrap
o.modulewrap:	C:h.kernel
o.modulewrap:	C:h.swis
o.modulewrap:	h.Consts
o.modulewrap:	h.RMInfo
o.modulewrap:	h.veneers
o.modulewrap:	h.Interface
o.modulewrap:	h.fsentries
o.modulewrap:	h.modulewrap
o.modulewrap:	h.resources
o.modulewrap:	h.str
o.modulewrap:	h.etc
o.modulewrap:	h.csfserr
o.modulewrap:	h.fs
o.modulewrap:	h.sfs
o.modulewrap:	h.arcs
o.modulewrap:	h.cat
o.modulewrap:	h.link
o.modulewrap:	h.mem
o.modulewrap:	h.mount
o.modulewrap:	h.update
o.modulewrap:	h.service
