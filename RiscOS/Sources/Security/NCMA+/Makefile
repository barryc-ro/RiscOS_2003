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
# use ENABLE_DEBUGGING if you want to enable debugging!
# use POINTLESS_BUTTONS if you want managed access to ignore the card going in and out and wait for the user
#                       to click on various 'continue' buttons. Undefine this for any sane implementation
# use AUTHENTICATE_SIGNATURES if you want managed access to be strict about signed contents
# use SHOW_BOOTBLOCK_TRANSITIONS if you want really verbose debugging of what NCBootblock's doing with your data

# no debugging
CCFLAGS  = -c -Throwback -depend !Depend ${INCLUDES} -zM -DUSE_TINY -UENABLE_DEBUGGING -DPOINTLESS_BUTTONS -UAUTHENTICATE_SIGNATURES 

# debugging
#CCFLAGS  = -c -Throwback -depend !Depend ${INCLUDES} -zM -DENABLE_DEBUGGING -DPOINTLESS_BUTTONS -UAUTHENTICATE_SIGNATURES \
		-DSHOW_BOOTBLOCK_TRANSITIONS


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
COMPONENT = NCMA+
TARGET    = aof.NCMA+
TARGETSA  = rm.NCMA+
EXPORTS   =
BASEOBJS  = module.o header.o vectors.o DBUG.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o kprintf.o ncma_smurf.o \
	  ncma_cron.o ncma_exec.o ncma_atr.o ncma_bootb.o ncma_mime.o \
	  ncma_tags.o ncma_nvram.o ncma_cache.o ncma_uid.o ncma_net.o \
	  usermode.o ncma_time.o areacode.o ncma_auth.o

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
