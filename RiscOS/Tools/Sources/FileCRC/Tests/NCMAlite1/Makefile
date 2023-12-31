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
# use SHOW_BOOTBLOCK_TRANSITIONS if you want really verbose debugging of what NCBootblock's doing with your data
# use CHIMPANZEE_INTERFACE if you're a big American corporation and you need special hacks in Managed Access to accomodate
# the fact that you bought 40,000 of the wrong type of smartcard
# use DEFEAT_BROWSER_LIMIT to prevent MA parsing NCIBrowserMode tags for restricting URLs
# use LIMIT_RUNAWAY_UIDS if you've got development NVRAM/OTP, otherwise weird stuff may happen
# use HIDEOUS_PROXY_HACK if you want https proxy to always be set to whatever the http proxy is (undef to use real
#                        security proxy information, if available in your area
# use NVRAM_DEBUG if you know what it does, otherwise don't
# use ETHERNET to disable all the modem crap and use a proper network interface
# use SEND_SERIALID_LENGTH to include length data as first 2 bytes of the serial ID string
# use ENABLE_HTTP_PROXY / ENABLE_HTTPS_PROXY to enable ROM based proxies for http and/or SSL http
# use DEBUG_DISCONNECTS to set all PPP line drop timeouts to a minute
# use ISP_CAPTURE to run ISPcapture on invalid smartcards
# use TRAP_PPP_TIMEOUTS if you want a "disconnect?" OSD to sit on PPPTimeout events. Undef to just drop the line
# use USE_NETINIT to invoke the NetInit module instead of bringing the network stuff up ourself
# use SILENT_HUP to ignore the line dropping out from under us, otherwise return to the 'Connect' screen
# use VERIFY_URL to check the home URL before sending it to the browser and generating an error if it seems bogus

# Override any options by setting the OPTS in the Build options

# debugging


NODB  =         -UREMOTE_DEBUG -DCHIMPANZEE_INTERFACE \
		-UPOINTLESS_BUTTONS -UAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-USHOW_BOOTBLOCK_TRANSITIONS  \
		-DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-DSEND_SERIALID_LENGTH \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY \
		-DISP_CAPTURE -UTRAP_PPP_TIMEOUTS -UUSE_NETINIT \
		-DSILENT_HUP -UVERIFY_URL ${TYPE}

NCTARGETARCH = ${NODB}

CCFLAGS = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${NCTARGETARCH} ${OPTS}





# Libraries
#
LIBDIR	   = <Lib$Dir>
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.c_AbsSym
SOCKLIB	   = TCPIPLibs:o.socklib5 
INETLIB	   = TCPIPLibs:o.inetlib
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz

#
# Rule patterns
#
.SUFFIXES:  .oz
.c.o:;      ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Program specific options:
#
COMPONENT = NCMAlite
TARGET    = aof.NCMAlite
TARGETSA  = rm.NCMAlite
EXPORTS   =
BASEOBJS  = module.o header.o vectors.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o kprintf.o  \
	  ncma_bootb.o task.o \
	  ncma_tags.o  ncma_cache.o ncma_uid.o ncma_net.o \
	  usermode.o areacode.o dbugzm.o ncma_fx.o

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = oz.resources ${BASEOBJS}

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
        @echo ${COMPONENT}: resource files copied

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}

#
# Exported interface headers
#

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${C++LIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${C++LIB}


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
o.module:	h.machine
o.module:	h.vectors
o.module:	h.ncma_cmds
o.module:	h.ncma_swis
o.module:	h.ncma_ncma
o.module:	h.ncma_scard
o.module:	h.ncma_dial
o.module:	h.ncma_uid
o.module:	h.dynalloc
o.module:	h.ncma_url
o.module:	h.types
o.module:	h.ncma_fx
o.module:	h.task
o.module:	C:h.kernel
o.module:	h.cmhg_defs
o.module:	h.DBUG
o.header:	cmhg.header
o.header:	h.cmhg_defs
o.header:	VersionNum
o.vectors:	c.vectors
o.vectors:	C:h.kernel
o.vectors:	C:h.swis
o.vectors:	h.ncma_ncma
o.vectors:	h.machine
o.vectors:	h.vectors
o.vectors:	h.module
o.vectors:	h.DBUG
o.vectors:	C:h.scinter
o.ncma_cmds:	c.ncma_cmds
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	C:h.swis
o.ncma_cmds:	h.ncma_cmds
o.ncma_cmds:	h.ncma_ncma
o.ncma_cmds:	h.ncma_utils
o.ncma_cmds:	h.ncma_url
o.ncma_cmds:	h.types
o.ncma_cmds:	h.ncma_scard
o.ncma_cmds:	h.ncma_cache
o.ncma_cmds:	h.ncma_dial
o.ncma_cmds:	h.machine
o.ncma_cmds:	h.vectors
o.ncma_cmds:	h.kprintf
o.ncma_cmds:	h.dynalloc
o.ncma_cmds:	h.usermode
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	h.DBUG
o.ncma_cmds:	h.task
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	h.cmhg_defs
o.ncma_swis:	c.ncma_swis
o.ncma_swis:	C:h.kernel
o.ncma_swis:	C:h.swis
o.ncma_swis:	h.ncma_swis
o.ncma_swis:	h.ncma_ncma
o.ncma_swis:	h.ncma_scard
o.ncma_swis:	h.ncma_cache
o.ncma_swis:	h.dynalloc
o.ncma_swis:	h.module
o.ncma_swis:	h.machine
o.ncma_swis:	h.usermode
o.ncma_swis:	C:h.kernel
o.ncma_swis:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	h.DBUG
o.dynalloc:	h.kprintf
o.ncma_ncma:	c.ncma_ncma
o.ncma_ncma:	C:h.kernel
o.ncma_ncma:	h.ncma_ncma
o.ncma_ncma:	h.ncma_url
o.ncma_ncma:	h.types
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
o.machine:	C:h.os
o.machine:	h.machine
o.machine:	h.module
o.machine:	h.ncma_ncma
o.machine:	h.ncma_scard
o.machine:	h.ncma_url
o.machine:	h.types
o.machine:	h.ncma_utils
o.machine:	h.ncma_dial
o.machine:	h.ncma_cache
o.machine:	h.ncma_bootb
o.machine:	h.ncma_net
o.machine:	h.ncma_fx
o.machine:	h.vectors
o.machine:	h.DBUG
o.machine:	h.task
o.machine:	C:h.kernel
o.ncma_url:	c.ncma_url
o.ncma_url:	C:h.kernel
o.ncma_url:	C:h.swis
o.ncma_url:	h.ncma_ncma
o.ncma_url:	h.ncma_url
o.ncma_url:	h.types
o.ncma_url:	h.ncma_scard
o.ncma_url:	h.ncma_cache
o.ncma_url:	h.ncma_tags
o.ncma_url:	h.ncma_bootb
o.ncma_url:	h.ncma_uid
o.ncma_url:	h.ncma_utils
o.ncma_url:	h.ncma_net
o.ncma_url:	h.smcard
o.ncma_url:	h.dynalloc
o.ncma_url:	h.types
o.ncma_url:	h.task
o.ncma_url:	C:h.kernel
o.ncma_url:	h.DBUG
o.ncma_scard:	c.ncma_scard
o.ncma_scard:	C:h.kernel
o.ncma_scard:	C:h.swis
o.ncma_scard:	h.ncma_scard
o.ncma_scard:	h.ncma_url
o.ncma_scard:	h.types
o.ncma_scard:	h.ncma_utils
o.ncma_scard:	h.dynalloc
o.ncma_scard:	h.usermode
o.ncma_scard:	C:h.kernel
o.ncma_scard:	h.machine
o.ncma_scard:	h.vectors
o.ncma_scard:	h.DBUG
o.ncma_scard:	h.kprintf
o.ncma_utils:	c.ncma_utils
o.ncma_utils:	C:h.kernel
o.ncma_utils:	C:h.swis
o.ncma_utils:	h.ncma_utils
o.ncma_utils:	h.ncma_scard
o.ncma_utils:	h.ncma_ncma
o.ncma_utils:	h.ncma_cache
o.ncma_utils:	h.ncma_tags
o.ncma_utils:	h.ncma_bootb
o.ncma_utils:	h.ncma_url
o.ncma_utils:	h.types
o.ncma_utils:	h.smcard
o.ncma_utils:	h.DBUG
o.ncma_dial:	c.ncma_dial
o.ncma_dial:	C:h.kernel
o.ncma_dial:	C:h.swis
o.ncma_dial:	h.machine
o.ncma_dial:	h.vectors
o.ncma_dial:	h.ncma_net
o.ncma_dial:	h.ncma_dial
o.ncma_dial:	h.DBUG
o.kprintf:	c.kprintf
o.kprintf:	C:h.kernel
o.kprintf:	C:h.swis
o.kprintf:	h.kprintf
o.ncma_bootb:	c.ncma_bootb
o.ncma_bootb:	C:h.kernel
o.ncma_bootb:	C:h.swis
o.ncma_bootb:	h.ncma_bootb
o.ncma_bootb:	h.ncma_tags
o.ncma_bootb:	h.ncma_scard
o.ncma_bootb:	h.ncma_cache
o.ncma_bootb:	h.ncma_url
o.ncma_bootb:	h.types
o.ncma_bootb:	h.ncma_uid
o.ncma_bootb:	h.ncma_utils
o.ncma_bootb:	h.smcard
o.ncma_bootb:	h.machine
o.ncma_bootb:	h.DBUG
o.task:	c.task
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	C:tboxlibs.h.wimp
o.task:	C:tboxlibs.h.wimplib
o.task:	h.cmhg_defs
o.task:	h.module
o.task:	h.task
o.task:	C:h.kernel
o.task:	h.machine
o.task:	h.ncma_url
o.task:	h.types
o.task:	h.dbug
o.ncma_tags:	c.ncma_tags
o.ncma_tags:	h.ncma_tags
o.ncma_tags:	h.ncma_utils
o.ncma_tags:	h.DBUG
o.ncma_cache:	c.ncma_cache
o.ncma_cache:	C:h.kernel
o.ncma_cache:	C:h.swis
o.ncma_cache:	h.smcard
o.ncma_cache:	h.dynalloc
o.ncma_cache:	h.ncma_cache
o.ncma_cache:	h.ncma_utils
o.ncma_cache:	h.ncma_tags
o.ncma_cache:	h.ncma_bootb
o.ncma_cache:	h.kprintf
o.ncma_cache:	h.DBUG
o.ncma_uid:	c.ncma_uid
o.ncma_uid:	C:h.kernel
o.ncma_uid:	C:h.swis
o.ncma_uid:	h.smcard
o.ncma_uid:	h.ncma_uid
o.ncma_uid:	h.DBUG
o.ncma_net:	c.ncma_net
o.ncma_net:	C:h.kernel
o.ncma_net:	C:h.swis
o.ncma_net:	h.ncma_utils
o.ncma_net:	h.ncma_scard
o.ncma_net:	h.ncma_ncma
o.ncma_net:	h.ncma_net
o.ncma_net:	h.ncma_cache
o.ncma_net:	h.ncma_tags
o.ncma_net:	h.ncma_bootb
o.ncma_net:	h.ncma_url
o.ncma_net:	h.types
o.ncma_net:	h.smcard
o.ncma_net:	h.DBUG
o.dbugzm:	c.dbugzm
o.dbugzm:	H.DBUG
o.ncma_fx:	c.ncma_fx
o.ncma_fx:	C:h.kernel
o.ncma_fx:	C:h.swis
o.ncma_fx:	h.ncma_fx
