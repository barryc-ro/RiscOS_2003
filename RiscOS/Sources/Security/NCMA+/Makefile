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
THROWBACK = -Throwback


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p -depend !Depend -throwback 
INCLUDES= -IC: 

#
# Different compile options - choose one
#
# use REMOTE_DEBUG to enable remote debugging
# use ENABLE_DEBUGGING if you want to enable debugging!
# use POINTLESS_BUTTONS if you want managed access to ignore the card going in and out and wait for the user
#                       to click on various 'continue' buttons. Undefine this for any sane implementation
# use AUTHENTICATE_SIGNATURES if you want managed access to be strict about signed contents
# use SHOW_BOOTBLOCK_TRANSITIONS if you want really verbose debugging of what NCBootblock's doing with your data
# use CHIMPANZEE_INTERFACE if you're a big American corporation and you need special hacks in Managed Access to accomodate
# the fact that you bought 40,000 of the wrong type of smartcard
# use DIALING_FROM_UK to frig numbers locally and tell the server to frig them for UK international access
# use DEFEAT_BROWSER_LIMIT to prevent MA parsing NCIBrowserMode tags for restricting URLs
# use SECURE_RELATIONSHIP_SERVER to speak to the relationship server via SSL
#use MANUFACTURER_KEY_X509 if the root public key is in an x.509 certificate, otherwise raw DER RSA key is assumed

# no debugging
CCFLAGS  = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM -UREMOTE_DEBUG -UENABLE_DEBUGGING -DPOINTLESS_BUTTONS -DUSE_TINY \
		-DCHIMPANZEE_INTERFACE -DAUTHENTICATE_SIGNATURES -DDIALING_FROM_UK -UDEFEAT_BROWSER_LIMIT \
		-USECURE_RELATIONSHIP_SERVER -DMANUFACTURER_KEY_X509

# debugging
#CCFLAGS  = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM -DREMOTE_DEBUG -DENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -DAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-DSHOW_BOOTBLOCK_TRANSITIONS -DDIALING_FROM_UK -DSECURE_RELATIONSHIP_SERVER \
		-UMANUFACTURER_KEY_X509


# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
DBUGLIB    = C:o.DBUGzm

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
BASEOBJS  = module.o header.o vectors.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o kprintf.o ncma_smurf.o \
	  ncma_cron.o ncma_exec.o ncma_atr.o ncma_bootb.o ncma_mime.o \
	  ncma_tags.o ncma_nvram.o ncma_cache.o ncma_uid.o ncma_net.o \
	  usermode.o ncma_time.o areacode.o ncma_auth.o ncma_fx.o

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = o.resources ${DBUGLIB} ${BASEOBJS}

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
        ${MKDIR} <resource$dir>.Resources
        ${MKDIR} <resource$dir>.Resources.NCMA
        ${CP} Resources.MFRCERT  <resource$dir>.Resources.NCMA.MFRCERT ${CPFLAGS}
        ${CP} Resources.MFRKEY <resource$dir>.Resources.NCMA.MFRKEY ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${DBUGLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB}

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
