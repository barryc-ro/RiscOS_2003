# Makefile for NCAccessManager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 29-Oct-98	AJS	Created
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
TOUCH   = create

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
# use LOCAL_URL_PATHS to get dialogues/screens off disk, otherwise they come from Resourcefs
# use FORCE_SINGLE_SLOT to debug single slot paths on a twin-slot NC (causes find_slots() to always return 1 regardless)
# use FORCE_DUAL_SLOT to prevent auto-detection of single slot box (causes find_slots() to always return 2 regardless)

# Override any options by setting the OPTS in the Build options

# debugging

ETHERNETDB  =   -DREMOTE_DEBUG -DENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -UDEFEAT_BROWSER_LIMIT \
		-DSHOW_BOOTBLOCK_TRANSITIONS \
		-DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-DETHERNET -DSEND_SERIALID_LENGTH \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY \
		-DISP_CAPTURE -UTRAP_PPP_TIMEOUTS -UUSE_NETINIT \
		-DSILENT_HUP -UVERIFY_URL -DLOCAL_URL_PATHS -DSHOW_EVENTS \
		-UFORCE_SINGLE_SLOT -UFORCE_DUAL_SLOT

ZIPDB  =        -UREMOTE_DEBUG -DENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -UDEFEAT_BROWSER_LIMIT \
		-DSHOW_BOOTBLOCK_TRANSITIONS  \
		-DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-UETHERNET -DSEND_SERIALID_LENGTH \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY \
		-DISP_CAPTURE -UTRAP_PPP_TIMEOUTS -UUSE_NETINIT \
		-DSILENT_HUP -UVERIFY_URL -DLOCAL_URL_PATHS -DSHOW_EVENTS \
		-UFORCE_SINGLE_SLOT -UFORCE_DUAL_SLOT

NODB  =         -UREMOTE_DEBUG -UENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-UPOINTLESS_BUTTONS -UAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-USHOW_BOOTBLOCK_TRANSITIONS  \
		-DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-UETHERNET -DSEND_SERIALID_LENGTH \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY \
		-DISP_CAPTURE -UTRAP_PPP_TIMEOUTS -UUSE_NETINIT \
		-DSILENT_HUP -UVERIFY_URL -ULOCAL_URL_PATHS -USHOW_EVENTS \
		-UFORCE_SINGLE_SLOT -UFORCE_DUAL_SLOT

#
# select one of these
#

#NCTARGETARCH = ${ETHERNETDB}
#NCTARGETARCH = ${ZIPDB}
NCTARGETARCH = ${NODB}

CCFLAGS = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${NCTARGETARCH} ${OPTS}





# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
DBUGLIB	   =

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
COMPONENT = NCMATwin
TARGET    = aof.NCMATwin
TARGETSA  = rm.NCMATwin
EXPORTS   =
BASEOBJS  = module.o header.o vectors.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o kprintf.o  \
	  ncma_bootb.o  ncma_funai.o \
	  ncma_tags.o  ncma_cache.o ncma_net.o \
	  usermode.o  ncma_fx.o

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = oz.resources ${DBUGLIB} ${BASEOBJS}

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
        ${RM} h.header
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

localdirs.o:
	$(MKDIR) o
	$(MKDIR) oz
	$(MKDIR) aof
	$(MKDIR) rm
	${TOUCH} $@

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${DBUGLIB} localdirs.o header.h
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${DBUGLIB}

#
# Exported interface headers
#

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} localdirs.o header.h
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
o.DBUG:	c.DBUG
o.DBUG:	C:H.DBUG
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	C:h.DBUG
o.module:	h.machine
o.module:	h.vectors
o.module:	h.ncma_cmds
o.module:	h.ncma_swis
o.module:	h.ncma_ncma
o.module:	h.ncma_scard
o.module:	h.ncma_dial
o.module:	h.dynalloc
o.module:	h.ncma_url
o.module:	h.ncma_fx
o.module:	VersionNum
o.module:	h.header
o.module:	C:h.DBUG
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
o.vectors:	C:h.DBUG
o.vectors:	C:h.scinter
o.vectors:	h.header
o.ncma_cmds:	c.ncma_cmds
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	C:h.swis
o.ncma_cmds:	h.ncma_cmds
o.ncma_cmds:	h.ncma_ncma
o.ncma_cmds:	h.ncma_utils
o.ncma_cmds:	h.ncma_url
o.ncma_cmds:	h.ncma_scard
o.ncma_cmds:	h.ncma_cache
o.ncma_cmds:	h.ncma_dial
o.ncma_cmds:	h.machine
o.ncma_cmds:	h.vectors
o.ncma_cmds:	h.kprintf
o.ncma_cmds:	h.dynalloc
o.ncma_cmds:	h.usermode
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	C:h.DBUG
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
o.ncma_swis:	C:h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.dynalloc:	C:h.DBUG
o.dynalloc:	h.kprintf
o.ncma_ncma:	c.ncma_ncma
o.ncma_ncma:	C:h.kernel
o.ncma_ncma:	h.ncma_ncma
o.ncma_ncma:	h.ncma_url
o.ncma_ncma:	h.dynalloc
o.ncma_ncma:	h.module
o.ncma_ncma:	C:h.DBUG
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
o.machine:	h.ncma_cache
o.machine:	h.ncma_bootb
o.machine:	h.ncma_net
o.machine:	h.ncma_funai
o.machine:	h.ncma_fx
o.machine:	h.vectors
o.machine:	C:h.DBUG
o.ncma_url:	c.ncma_url
o.ncma_url:	C:h.kernel
o.ncma_url:	C:h.swis
o.ncma_url:	h.ncma_ncma
o.ncma_url:	h.ncma_url
o.ncma_url:	h.ncma_scard
o.ncma_url:	h.ncma_cache
o.ncma_url:	h.ncma_tags
o.ncma_url:	h.ncma_bootb
o.ncma_url:	h.ncma_utils
o.ncma_url:	h.ncma_net
o.ncma_url:	h.ncma_funai
o.ncma_url:	C:h.smcardCCIJ
o.ncma_url:	h.dynalloc
o.ncma_url:	h.machine
o.ncma_url:	h.vectors
o.ncma_url:	C:h.DBUG
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
o.ncma_scard:	C:h.DBUG
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
o.ncma_utils:	C:h.smcardCCIJ
o.ncma_utils:	C:h.DBUG
o.ncma_dial:	c.ncma_dial
o.ncma_dial:	C:h.kernel
o.ncma_dial:	C:h.swis
o.ncma_dial:	h.machine
o.ncma_dial:	h.vectors
o.ncma_dial:	h.ncma_dial
o.ncma_dial:	C:h.DBUG
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
o.ncma_bootb:	h.ncma_utils
o.ncma_bootb:	C:h.smcardCCIJ
o.ncma_bootb:	C:h.DBUG
o.ncma_funai:	c.ncma_funai
o.ncma_funai:	C:h.kernel
o.ncma_funai:	C:h.swis
o.ncma_funai:	h.ncma_funai
o.ncma_funai:	C:h.smcardCCIJ
o.ncma_funai:	h.ncma_cache
o.ncma_funai:	h.ncma_tags
o.ncma_funai:	h.ncma_bootb
o.ncma_funai:	C:h.DBUG
o.ncma_tags:	c.ncma_tags
o.ncma_tags:	h.ncma_tags
o.ncma_tags:	C:h.DBUG
o.ncma_cache:	c.ncma_cache
o.ncma_cache:	C:h.kernel
o.ncma_cache:	C:h.swis
o.ncma_cache:	C:h.smcardCCIJ
o.ncma_cache:	h.dynalloc
o.ncma_cache:	h.ncma_cache
o.ncma_cache:	h.ncma_utils
o.ncma_cache:	h.ncma_tags
o.ncma_cache:	h.ncma_bootb
o.ncma_cache:	h.kprintf
o.ncma_cache:	C:h.DBUG
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
o.ncma_net:	h.ncma_funai
o.ncma_net:	C:h.smcardCCIJ
o.ncma_net:	C:h.DBUG
o.ncma_fx:	c.ncma_fx
o.ncma_fx:	C:h.kernel
o.ncma_fx:	C:h.swis
o.ncma_fx:	h.ncma_fx
o.header:	cmhg.header
o.header:	h.cmhg_defs
o.header:	VersionNum
