# Makefile for CSFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-1997  RWB 	  New for CSFS


#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
DFLAGS  =
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
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

# Program specific options:
#
COMPONENT = CSFS
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.modulewrap \
o.header   \
o.RMInfo   \
o.Statics  \
o.allerrs  \
o.sfs      \
o.sff      \
o.sfi      \
o.arcs     \
o.arcf     \
o.dir      \
o.cat      \
o.virt     \
o.flex     \
o.fx       \
o.mem      \
o.scrap    \
o.mount    \
o.service  \
o.serx     \
o.update   \
o.fs	   \
o.interface \
o.link	    \
o.open	    \
o.gbpb	    \
o.func	    \
o.file	    \
o.close	    \
o.args	    \
o.getbytes  \
o.putbytes  \
o.usermode

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} $< $@
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} dirs
        @echo ${COMPONENT}: all complete
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

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
dirs:
	${MKDIR} rm
	${MKDIR} o
	${MKDIR} linked
	${MKDIR} map

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INETLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB}

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
rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} ${INETLIB}

#
# Dynamic dependencies:
o.modulewrap:	c.modulewrap
o.modulewrap:	C:h.kernel
o.modulewrap:	C:h.swis
o.modulewrap:	h.Consts
o.modulewrap:	h.AllErrs
o.modulewrap:	h.RMInfo
o.modulewrap:	h.Statics
o.modulewrap:	h.veneers
o.modulewrap:	h.Interface
o.modulewrap:	h.fsentries
o.modulewrap:	h.modulewrap
o.modulewrap:	h.str
o.modulewrap:	h.etc
o.modulewrap:	h.fs
o.modulewrap:	h.sfs
o.modulewrap:	h.arcs
o.modulewrap:	h.cat
o.modulewrap:	h.link
o.modulewrap:	h.mem
o.modulewrap:	h.mount
o.modulewrap:	h.update
o.modulewrap:	h.service
o.RMInfo: s.RMInfo
o.Statics:	c.Statics
o.Statics:	C:h.kernel
o.Statics:	C:h.swis
o.Statics:	h.Consts
o.Statics:	h.AllErrs
o.Statics:	h.RMInfo
o.Statics:	h.Statics
o.Statics:	h.veneers
o.Statics:	h.Interface
o.Statics:	h.fsentries
o.Statics:	h.ModuleWrap
o.allerrs: s.allerrs
o.sfs:	c.sfs
o.sfs:	C:h.kernel
o.sfs:	C:h.swis
o.sfs:	h.Consts
o.sfs:	h.AllErrs
o.sfs:	h.RMInfo
o.sfs:	h.Statics
o.sfs:	h.veneers
o.sfs:	h.Interface
o.sfs:	h.fsentries
o.sfs:	h.ModuleWrap
o.sfs:	h.flex
o.sfs:	h.str
o.sfs:	h.etc
o.sfs:	h.allerrs
o.sfs:	h.arcs
o.sfs:	h.dir
o.sfs:	h.virt
o.sfs:	h.cat
o.sfs:	h.fs
o.sfs:	h.sff
o.sfs:	h.link
o.sfs:	h.mem
o.sfs:	h.fx
o.sfs:	h.scrap
o.sfs:	h.service
o.sfs:	h.sfs
o.sff:	c.sff
o.sff:	C:h.kernel
o.sff:	C:h.swis
o.sff:	h.Consts
o.sff:	h.AllErrs
o.sff:	h.RMInfo
o.sff:	h.Statics
o.sff:	h.veneers
o.sff:	h.Interface
o.sff:	h.fsentries
o.sff:	h.ModuleWrap
o.sff:	h.flex
o.sff:	h.str
o.sff:	h.etc
o.sff:	h.fs
o.sff:	h.arcs
o.sff:	h.arcf
o.sff:	h.dir
o.sff:	h.cat
o.sff:	h.fx
o.sff:	h.link
o.sff:	h.sfs
o.sff:	h.sff
o.sfi:	c.sfi
o.sfi:	C:h.kernel
o.sfi:	C:h.swis
o.sfi:	h.Consts
o.sfi:	h.AllErrs
o.sfi:	h.RMInfo
o.sfi:	h.Statics
o.sfi:	h.veneers
o.sfi:	h.Interface
o.sfi:	h.fsentries
o.sfi:	h.ModuleWrap
o.sfi:	h.flex
o.sfi:	h.str
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
o.arcs:	h.AllErrs
o.arcs:	h.RMInfo
o.arcs:	h.Statics
o.arcs:	h.veneers
o.arcs:	h.Interface
o.arcs:	h.fsentries
o.arcs:	h.ModuleWrap
o.arcs:	h.flex
o.arcs:	h.str
o.arcs:	h.etc
o.arcs:	h.scrap
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
o.arcf:	h.AllErrs
o.arcf:	h.RMInfo
o.arcf:	h.Statics
o.arcf:	h.veneers
o.arcf:	h.Interface
o.arcf:	h.fsentries
o.arcf:	h.ModuleWrap
o.arcf:	h.flex
o.arcf:	h.str
o.arcf:	h.fs
o.arcf:	h.sfs
o.arcf:	h.arcs
o.arcf:	h.cat
o.arcf:	h.link
o.arcf:	h.arcf
o.dir:	c.dir
o.dir:	C:h.kernel
o.dir:	C:h.swis
o.dir:	h.Consts
o.dir:	h.AllErrs
o.dir:	h.RMInfo
o.dir:	h.Statics
o.dir:	h.veneers
o.dir:	h.Interface
o.dir:	h.fsentries
o.dir:	h.ModuleWrap
o.dir:	h.flex
o.dir:	h.str
o.dir:	h.fs
o.dir:	h.sfs
o.dir:	h.arcs
o.dir:	h.cat
o.dir:	h.link
o.dir:	h.dir
o.cat:	c.cat
o.cat:	C:h.kernel
o.cat:	C:h.swis
o.cat:	h.Consts
o.cat:	h.AllErrs
o.cat:	h.RMInfo
o.cat:	h.Statics
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
o.virt:	c.virt
o.virt:	C:h.kernel
o.virt:	C:h.swis
o.virt:	h.Consts
o.virt:	h.AllErrs
o.virt:	h.RMInfo
o.virt:	h.Statics
o.virt:	h.veneers
o.virt:	h.Interface
o.virt:	h.fsentries
o.virt:	h.ModuleWrap
o.virt:	h.flex
o.virt:	h.str
o.virt:	h.fs
o.virt:	h.sfs
o.virt:	h.arcs
o.virt:	h.virt
o.flex:	c.flex
o.flex:	C:h.kernel
o.flex:	C:h.swis
o.flex:	h.mem
o.flex:	h.allerrs
o.flex:	h.flex
o.fx:	c.fx
o.fx:	C:h.kernel
o.fx:	C:h.swis
o.fx:	h.Consts
o.fx:	h.AllErrs
o.fx:	h.RMInfo
o.fx:	h.Statics
o.fx:	h.veneers
o.fx:	h.Interface
o.fx:	h.fsentries
o.fx:	h.ModuleWrap
o.fx:	h.flex
o.fx:	h.str
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
o.mem:	h.allerrs
o.mem:	h.flex
o.mem:	h.mem
o.scrap:	c.scrap
o.scrap:	C:h.kernel
o.scrap:	C:h.swis
o.scrap:	h.etc
o.scrap:	h.str
o.scrap:	h.allerrs
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
o.mount:	h.allerrs
o.mount:	h.consts
o.mount:	h.flex
o.mount:	h.str
o.mount:	h.csfs
o.mount:	h.link
o.mount:	h.arcs
o.mount:	h.fs
o.mount:	h.cat
o.mount:	h.dir
o.mount:	h.service
o.mount:	h.mount
o.service:	c.service
o.service:	C:h.kernel
o.service:	C:h.swis
o.service:	h.Consts
o.service:	h.AllErrs
o.service:	h.RMInfo
o.service:	h.Statics
o.service:	h.veneers
o.service:	h.Interface
o.service:	h.fsentries
o.service:	h.ModuleWrap
o.service:	h.flex
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
o.serx:	h.AllErrs
o.serx:	h.RMInfo
o.serx:	h.Statics
o.serx:	h.veneers
o.serx:	h.Interface
o.serx:	h.fsentries
o.serx:	h.ModuleWrap
o.serx:	h.flex
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
o.update:	h.AllErrs
o.update:	h.RMInfo
o.update:	h.Statics
o.update:	h.veneers
o.update:	h.Interface
o.update:	h.fsentries
o.update:	h.ModuleWrap
o.update:	h.flex
o.update:	h.str
o.update:	h.csfs
o.update:	h.etc
o.update:	h.allerrs
o.update:	h.arcs
o.update:	h.dir
o.update:	h.virt
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
o.fs:	h.AllErrs
o.fs:	h.RMInfo
o.fs:	h.Statics
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
o.link:	h.AllErrs
o.link:	h.RMInfo
o.link:	h.Statics
o.link:	h.veneers
o.link:	h.Interface
o.link:	h.fsentries
o.link:	h.ModuleWrap
o.link:	h.flex
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
o.open:	h.AllErrs
o.open:	h.RMInfo
o.open:	h.Statics
o.open:	h.veneers
o.open:	h.Interface
o.open:	h.fsentries
o.open:	h.ModuleWrap
o.open:	h.str
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
o.gbpb:	h.AllErrs
o.gbpb:	h.RMInfo
o.gbpb:	h.Statics
o.gbpb:	h.veneers
o.gbpb:	h.Interface
o.gbpb:	h.fsentries
o.gbpb:	h.ModuleWrap
o.gbpb:	h.mem
o.gbpb:	h.sfi
o.func:	c.func
o.func:	C:h.kernel
o.func:	C:h.swis
o.func:	h.Consts
o.func:	h.AllErrs
o.func:	h.RMInfo
o.func:	h.Statics
o.func:	h.veneers
o.func:	h.Interface
o.func:	h.fsentries
o.func:	h.ModuleWrap
o.func:	h.str
o.func:	h.mem
o.func:	h.sfs
o.file:	c.file
o.file:	C:h.kernel
o.file:	C:h.swis
o.file:	h.Consts
o.file:	h.AllErrs
o.file:	h.RMInfo
o.file:	h.Statics
o.file:	h.veneers
o.file:	h.Interface
o.file:	h.fsentries
o.file:	h.ModuleWrap
o.file:	h.str
o.file:	h.fs
o.file:	h.mem
o.file:	h.sfs
o.file:	h.arcs
o.file:	h.sff
o.close:	c.close
o.close:	C:h.kernel
o.close:	C:h.swis
o.close:	h.Consts
o.close:	h.AllErrs
o.close:	h.RMInfo
o.close:	h.Statics
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
o.args:	h.AllErrs
o.args:	h.RMInfo
o.args:	h.Statics
o.args:	h.veneers
o.args:	h.Interface
o.args:	h.fsentries
o.args:	h.ModuleWrap
o.args:	h.mem
o.args:	h.sfi
o.getbytes:	c.getbytes
o.getbytes:	C:h.kernel
o.getbytes:	C:h.swis
o.getbytes:	h.Consts
o.getbytes:	h.AllErrs
o.getbytes:	h.RMInfo
o.getbytes:	h.Statics
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
o.putbytes:	h.AllErrs
o.putbytes:	h.RMInfo
o.putbytes:	h.Statics
o.putbytes:	h.veneers
o.putbytes:	h.Interface
o.putbytes:	h.fsentries
o.putbytes:	h.ModuleWrap
o.putbytes:	h.mem
o.putbytes:	h.sfi
o.usermode: s.usermode
o.usermode: Hdr:ListOpts
o.usermode: Hdr:Macros
o.usermode: Hdr:System
o.usermode: Hdr:SWIs
o.usermode: Hdr:CPU.Generic26
o.usermode: Hdr:IO.GenericIO
o.usermode: Hdr:RISCOS
o.usermode: Hdr:EnvNumbers
