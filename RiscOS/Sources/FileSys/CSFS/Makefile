# Makefile for CSFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-1997  RWB 	  New for CSFS
# 28 May 1997  RWB 	  Different Debug build


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
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = CSFS
TARGET    = aof.${COMPONENT}

.SUFFIXES:   .od

BASEOBJS      =	o.RMInfo o.csfserr o.sfs o.sff o.sfi o.arcs o.arcf o.dir \
	        o.cat o.flex o.fx o.mem o.scrap o.mount o.service o.serx \
	        o.update o.fs o.interface o.link o.open	o.gbpb o.func	 \
		o.file o.close o.args o.getbytes o.putbytes o.xstr       \
		o.usermode o.header o.modulewrap

DBASEOBJS     =	od.RMInfo od.csfserr od.sfs od.sff od.sfi od.arcs od.arcf \
	      	od.dir od.cat od.flex od.fx od.mem od.scrap od.mount 	  \
	      	od.service od.serx od.update od.fs od.interface od.link   \
	      	od.open	od.gbpb od.func od.file od.close od.args 	  \
	      	od.getbytes od.putbytes od.xstr od.usermode od.header 	  \
	      	od.modulewrap

OBJS =    o.resources  ${BASEOBJS}
DOBJS =   od.resources ${DBASEOBJS}
ROMOBJS = ${BASEOBJS}


#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} $< $@
.s.o:;      ${AS} ${AFLAGS}  $< $@
.c.od:;     ${CC} ${CFLAGS} -o $@ $< -DREMOTE_DEBUG
.cmhg.od:;  ${CMHG} ${CMHGFLAGS} $< $@ -DREMOTE_DEBUG
.s.od:;     ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} dirs
        @echo ${COMPONENT}: complete

debug: rm.${COMPONENT}D
        @echo ${COMPONENT}D: complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
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
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
dirs:
	${MKDIR} rm
	${MKDIR} o
	${MKDIR} od
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} aof

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${INETLIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${INETLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB} ${INETLIB} dirs
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} ${INETLIB}
        Access rm.${COMPONENT} RW/R

rm.${COMPONENT}D: ${DOBJS} ${CLIB} ${REMOTEDB} ${INETLIB} dirs
        ${LD} -rmf -o $@ ${DOBJS} ${CLIB} ${INETLIB} ${REMOTEDB}
        Access rm.${COMPONENT}D RW/R

#
# Dynamic dependencies:
o.modulewrap:	c.modulewrap
o.modulewrap:	C:h.kernel
o.modulewrap:	C:h.swis
o.modulewrap:	h.Consts
o.modulewrap:	C:debug.h.remote
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
o.serx:	c.serx
o.serx:	C:h.kernel
o.serx:	C:h.swis
o.serx:	h.Consts
o.serx:	C:debug.h.remote
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
o.update:	C:debug.h.remote
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
o.fs:	C:debug.h.remote
o.fs:	h.RMInfo
o.fs:	h.veneers
o.fs:	h.Interface
o.fs:	h.fsentries
o.fs:	h.ModuleWrap
o.fs:	h.str
o.fs:	h.fs
o.interface: s.interface
o.link:	c.link
o.link:	C:h.kernel
o.link:	C:h.swis
o.link:	h.Consts
o.link:	C:debug.h.remote
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
o.open:	C:debug.h.remote
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
o.gbpb:	C:debug.h.remote
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
o.func:	C:debug.h.remote
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
o.file:	C:debug.h.remote
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
o.close:	C:debug.h.remote
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
o.args:	C:debug.h.remote
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
o.getbytes:	C:debug.h.remote
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
o.putbytes:	C:debug.h.remote
o.putbytes:	h.RMInfo
o.putbytes:	h.veneers
o.putbytes:	h.Interface
o.putbytes:	h.fsentries
o.putbytes:	h.ModuleWrap
o.putbytes:	h.mem
o.putbytes:	h.sfi
o.xstr:	c.xstr
o.xstr:	C:h.kernel
o.xstr:	C:h.swis
o.xstr:	h.consts
o.xstr:	C:debug.h.remote
o.xstr:	h.flex
o.xstr:	h.xstr
o.usermode: s.usermode
o.RMInfo: s.RMInfo
o.csfserr:	c.csfserr
o.csfserr:	C:h.kernel
o.csfserr:	C:h.swis
o.csfserr:	h.etc
o.csfserr:	h.csfserr
o.sfs:	c.sfs
o.sfs:	C:h.kernel
o.sfs:	C:h.swis
o.sfs:	h.Consts
o.sfs:	C:debug.h.remote
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
o.sff:	C:debug.h.remote
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
o.sfi:	C:debug.h.remote
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
o.arcs:	C:debug.h.remote
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
o.arcf:	C:debug.h.remote
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
o.dir:	C:debug.h.remote
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
o.cat:	C:debug.h.remote
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
o.flex:	c.flex
o.flex:	C:h.kernel
o.flex:	C:h.swis
o.flex:	h.mem
o.flex:	h.csfserr
o.flex:	h.flex
o.fx:	c.fx
o.fx:	C:h.kernel
o.fx:	C:h.swis
o.fx:	h.Consts
o.fx:	C:debug.h.remote
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
o.mem:	c.mem
o.mem:	C:h.kernel
o.mem:	C:h.swis
o.mem:	h.etc
o.mem:	h.csfserr
o.mem:	h.flex
o.mem:	h.mem
o.scrap:	c.scrap
o.scrap:	C:h.kernel
o.scrap:	C:h.swis
o.scrap:	h.consts
o.scrap:	C:debug.h.remote
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
o.mount:	C:debug.h.remote
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
o.service:	c.service
o.service:	C:h.kernel
o.service:	C:h.swis
o.service:	h.Consts
o.service:	C:debug.h.remote
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
