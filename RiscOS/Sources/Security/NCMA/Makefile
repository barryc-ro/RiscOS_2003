# Makefile for NCAccessManager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 21-Mar-97	AJS	Created
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


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p -depend !Depend -throwback 
INCLUDES= -IC: 

#
# Different compile options - choose one
#

# no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -UENABLE_DEBUGGING 

# debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DENABLE_DEBUGGING 


# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#SOCKLIB	   = InetInc:^.^.libs.socklib.socklibzm
#INETLIB	   = InetInc:^.^.libs.inetlib.inetlibzm

#
# Rule patterns
#
.SUFFIXES:  .oz
.c.o:;      ${CC} ${CCFLAGS} -o $@ $< -DROM_BUILD=0
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< -DROM_BUILD=1
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ -d h.cmhg $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Program specific options:
#
COMPONENT = NCMA
TARGET    = aof.NCMA
TARGETSA  = rm.NCMA
EXPORTS   = 
BASEOBJS  = module.o header.o vectors.o DBUG.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o 

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = o.resources ${BASEOBJS}

RESDIR = <resource$dir>.Resources.${COMPONENT}

NOTROMRESDEP = html.badpin html.change html.damaged html.disconnect html.download \
	html.mismatch html.moved html.newpin html.notMA html.notNC html.pin \
	html.poweroff html.SCprobs html.splash html.transfer html.unblock \
	html.toplevel html.welcome

ROMRESDEP = 

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
#        ${MKDIR} ${RESDIR}
#        ${CP} Resources.${LOCALE}.*  ${RESDIR}.* ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
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
o.header:	cmhg.header
o.header:	h.cmhg_defs
o.vectors:	c.vectors
o.vectors:	C:h.kernel
o.vectors:	C:h.swis
o.vectors:	h.ncma_ncma
o.vectors:	h.machine
o.vectors:	h.vectors
o.vectors:	h.module
o.vectors:	h.DBUG
o.vectors:	h.scinter
o.DBUG:	c.DBUG
o.DBUG:	H.DBUG
o.ncma_cmds:	c.ncma_cmds
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	C:h.swis
o.ncma_cmds:	h.ncma_cmds
o.ncma_cmds:	h.ncma_ncma
o.ncma_cmds:	h.ncma_utils
o.ncma_cmds:	h.ncma_url
o.ncma_cmds:	h.ncma_scard
o.ncma_cmds:	h.machine
o.ncma_cmds:	h.vectors
o.ncma_cmds:	h.DBUG
o.ncma_swis:	c.ncma_swis
o.ncma_swis:	C:h.kernel
o.ncma_swis:	C:h.swis
o.ncma_swis:	h.ncma_swis
o.ncma_swis:	h.ncma_ncma
o.ncma_swis:	h.ncma_scard
o.ncma_swis:	h.dynalloc
o.ncma_swis:	h.module
o.ncma_swis:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	h.DBUG
o.ncma_ncma:	c.ncma_ncma
o.ncma_ncma:	C:h.kernel
o.ncma_ncma:	h.ncma_ncma
o.ncma_ncma:	h.ncma_url
o.ncma_ncma:	h.dynalloc
o.ncma_ncma:	h.module
o.ncma_ncma:	h.DBUG
o.ncma_ncma:	C:h.kernel
o.ncma_ncma:	C:h.swis
o.ncma_ncma:	h.machine
o.ncma_ncma:	h.vectors
o.machine:	c.machine
o.machine:	C:h.kernel
o.machine:	C:h.swis
o.machine:	h.machine
o.machine:	h.module
o.machine:	h.ncma_ncma
o.machine:	h.ncma_scard
o.machine:	h.ncma_url
o.machine:	h.ncma_utils
o.machine:	h.ncma_dial
o.machine:	h.vectors
o.machine:	h.DBUG
o.ncma_url:	c.ncma_url
o.ncma_url:	C:h.kernel
o.ncma_url:	C:h.swis
o.ncma_url:	h.ncma_ncma
o.ncma_url:	h.ncma_url
o.ncma_url:	h.ncma_scard
o.ncma_url:	h.DBUG
o.ncma_scard:	c.ncma_scard
o.ncma_scard:	C:h.kernel
o.ncma_scard:	C:h.swis
o.ncma_scard:	h.ncma_scard
o.ncma_scard:	h.ncma_url
o.ncma_scard:	h.ncma_utils
o.ncma_scard:	h.dynalloc
o.ncma_scard:	h.usermode
o.ncma_scard:	C:h.kernel
o.ncma_scard:	h.machine
o.ncma_scard:	h.vectors
o.ncma_scard:	h.DBUG
o.ncma_utils:	c.ncma_utils
o.ncma_utils:	C:h.kernel
o.ncma_utils:	C:h.swis
o.ncma_utils:	h.ncma_utils
o.ncma_utils:	h.ncma_scard
o.ncma_utils:	h.ncma_ncma
o.ncma_utils:	h.DBUG
o.ncma_dial:	c.ncma_dial
o.ncma_dial:	C:h.kernel
o.ncma_dial:	C:h.swis
o.ncma_dial:	h.ncma_dial
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	h.DBUG
o.module:	h.machine
o.module:	h.vectors
o.module:	h.ncma_cmds
o.module:	h.ncma_swis
o.module:	h.ncma_ncma
o.module:	h.ncma_scard
o.module:	h.dynalloc
