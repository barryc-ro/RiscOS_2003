# Makefile for ISPCapture
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 08-Aug-97	AJS	Created
#

#
# Paths
#
EXP_HDR = <export$dir>
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
# THROWBACK = -throwback


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p -depend !Depend ${THROWBACK}
INCLUDES= -IC: -ITCPIPLibs:

#
# Different compile options - choose one
#
# use REMOTE_DEBUG to enable remote debugging
# use ENABLE_DEBUGGING if you want to enable debugging!
# use LOCAL_URL_PATHS to get pages from ispc: instead of ispcScr:

# debugging

ETHERNETDB  =   -DREMOTE_DEBUG -DENABLE_DEBUGGING -DLOCAL_URL_PATHS

ZIPDB  =        -UREMOTE_DEBUG -DENABLE_DEBUGGING -ULOCAL_URL_PATHS

NODB  =         -UREMOTE_DEBUG -UENABLE_DEBUGGING -DLOCAL_URL_PATHS

#
# select one of these
#

#NCTARGETARCH = ${ETHERNETDB}
#NCTARGETARCH = ${ZIPDB}
NCTARGETARCH = ${NODB}

CCFLAGS = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM -DCOMPAT_INET4 ${NCTARGETARCH} 





# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
DBUGLIB    = C:o.DBUGzm

SOCKLIB	   = TCPIPLibs:o.socklibzm
INETLIB	   = TCPIPLibs:o.inetlibzm

#
# Rule patterns
#
.SUFFIXES:  .oz
.c.o:;      ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ -d h.cmhg $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Program specific options:
#
COMPONENT = ISPCapture
TARGET    = aof.ISPCapture
TARGETSA  = rm.ISPCapture
EXPORTS   =
BASEOBJS  = module.o header.o ispc_cmds.o dynalloc.o ispc_ispc.o \
	  ispc_url.o kprintf.o ispc_utils.o usermode.o ispc_file.o

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = o.resources ${DBUGLIB} ${BASEOBJS}

RESDIR = <resource$dir>.Resources.${COMPONENT}
HTMLDIR = <resource$dir>.!Boot.HTML
LDIR = Resources.${LOCALE}.${USERIF}

NOTROMRESDEP = html.ISPC1 html.ISPC2 html.ISPC3 html.ISPC4

ROMRESDEP = html.ISPC1 html.ISPC2 html.ISPC3 html.ISPC4

#
# Component build rules
#

o.resources: s.resources ${NOTROMRESDEP}

oz.resources: s.resources ${ROMRESDEP}

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: all complete

rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}        
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${HTMLDIR}.ISPCapture
        ${CP} ${LDIR}.HTML.connect ${HTMLDIR}.MA.connect ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC1 ${HTMLDIR}.ISPCapture.ISPC1 ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC2 ${HTMLDIR}.ISPCapture.ISPC2 ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied
#        ${MKDIR} <resource$dir>.Resources
#        ${MKDIR} <resource$dir>.Resources.ISPCapture

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${DBUGLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${SOCKLIB} ${INETLIB}

#
# Exported interface headers
#

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}


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



# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	h.DBUG
o.module:	h.dynalloc
o.module:	h.ispc_ispc
o.module:	h.ispc_cmds
o.header:	cmhg.header
o.header:	h.cmhg_defs
o.ispc_cmds:	c.ispc_cmds
o.ispc_cmds:	C:h.kernel
o.ispc_cmds:	C:h.swis
o.ispc_cmds:	h.ispc_cmds
o.ispc_cmds:	h.ispc_ispc
o.ispc_cmds:	h.ispc_url
o.ispc_cmds:	h.ispc_utils
o.ispc_cmds:	h.kprintf
o.ispc_cmds:	h.dynalloc
o.ispc_cmds:	h.ispc_file
o.ispc_cmds:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	h.DBUG
o.dynalloc:	h.kprintf
o.ispc_ispc:	c.ispc_ispc
o.ispc_ispc:	C:h.kernel
o.ispc_ispc:	C:h.swis
o.ispc_ispc:	TCPIPLibs:h.socklib
o.ispc_ispc:	TCPIPLibs:sys.h.types
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	TCPIPLibs:machine.h.endian
o.ispc_ispc:	TCPIPLibs:machine.h.ansi
o.ispc_ispc:	TCPIPLibs:machine.h.types
o.ispc_ispc:	TCPIPLibs:sys.h.socket
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	TCPIPLibs:sys.h.stat
o.ispc_ispc:	TCPIPLibs:sys.h.time
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	TCPIPLibs:sys.h.time
o.ispc_ispc:	TCPIPLibs:sys.h.uio
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	C:h.kernel
o.ispc_ispc:	TCPIPLibs:h.inetlib
o.ispc_ispc:	TCPIPLibs:sys.h.types
o.ispc_ispc:	TCPIPLibs:netinet.h.in
o.ispc_ispc:	TCPIPLibs:h.netdb
o.ispc_ispc:	TCPIPLibs:sys.h.cdefs
o.ispc_ispc:	h.smcard
o.ispc_ispc:	h.ispc_cache
o.ispc_ispc:	h.ispc_ispc
o.ispc_ispc:	h.ispc_url
o.ispc_ispc:	h.ispc_utils
o.ispc_ispc:	h.dynalloc
o.ispc_ispc:	h.DBUG
o.ispc_url:	c.ispc_url
o.ispc_url:	C:h.kernel
o.ispc_url:	C:h.swis
o.ispc_url:	h.ispc_ispc
o.ispc_url:	h.ispc_url
o.ispc_url:	h.ispc_utils
o.ispc_url:	h.DBUG
o.kprintf:	c.kprintf
o.kprintf:	C:h.kernel
o.kprintf:	C:h.swis
o.kprintf:	h.kprintf
o.ispc_utils:	c.ispc_utils
o.ispc_utils:	C:h.kernel
o.ispc_utils:	C:h.swis
o.ispc_utils:	h.ispc_utils
o.ispc_utils:	h.ispc_ispc
o.ispc_utils:	h.ispc_url
o.ispc_utils:	h.smcard
o.ispc_utils:	h.DBUG
o.ispc_file:	c.ispc_file
o.ispc_file:	C:h.kernel
o.ispc_file:	C:h.swis
o.ispc_file:	h.dynalloc
o.ispc_file:	h.ispc_file
o.ispc_file:	h.ispc_cache
o.ispc_file:	h.ispc_utils
o.ispc_file:	h.DBUG
