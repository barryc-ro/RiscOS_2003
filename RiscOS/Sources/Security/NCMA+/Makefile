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
# THROWBACK = -Throwback # Throwback is NOT the default.  Only do throwback when requested.


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
# use POINTLESS_BUTTONS if you want managed access to ignore the card going in and out and wait for the user
#                       to click on various 'continue' buttons. Undefine this for any sane implementation
# use AUTHENTICATE_SIGNATURES if you want managed access to be strict about signed contents
# use SHOW_BOOTBLOCK_TRANSITIONS if you want really verbose debugging of what NCBootblock's doing with your data
# use CHIMPANZEE_INTERFACE if you're a big American corporation and you need special hacks in Managed Access to accomodate
# the fact that you bought 40,000 of the wrong type of smartcard
# use DIALING_FROM_UK to frig numbers locally and tell the server to frig them for UK international access
# use DEFEAT_BROWSER_LIMIT to prevent MA parsing NCIBrowserMode tags for restricting URLs
# use SECURE_RELATIONSHIP_SERVER to speak to the relationship server via SSL
# use MANUFACTURER_KEY_X509 if the root public key is in an x.509 certificate, otherwise raw DER RSA key is assumed
# use LIMIT_RUNAWAY_UIDS if you've got development NVRAM/OTP, otherwise weird stuff may happen
# use HIDEOUS_PROXY_HACK if you want https proxy to always be set to whatever the http proxy is (undef to use real
#                        security proxy information, if available in your area
# use NVRAM_DEBUG if you know what it does, otherwise don't
# use ETHERNET to disable all the modem crap and use a proper network interface
# use TEST_CRON to test cron - normal cron service will be unusual
# use SEND_SERIALID_LENGTH to include length data as first 2 bytes of the serial ID string
# use ENABLE_HTTP_PROXY / ENABLE_HTTPS_PROXY to enable ROM based proxies for http and/or SSL http
# use PRODUCTION_KEYS to use final 'live' RSA crypto stuff (MFRKEYlive from resources, not MFRKEY)
# use DEBUG_DISCONNECTS to set all PPP line drop timeouts to a minute
# use MODEM_RACK for the weird alternative dial-up details for some geezer at NCI
# use NAVIO_SERVER for the Navio server

# debugging

ETHERNETDB  =   -DREMOTE_DEBUG -DENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -DAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-DSHOW_BOOTBLOCK_TRANSITIONS -DDIALING_FROM_UK -DSECURE_RELATIONSHIP_SERVER \
		-UMANUFACTURER_KEY_X509 -DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-DETHERNET -DTEST_CRON -DSEND_SERIALID_LENGTH -UNAVIO_SERVER \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY -UPRODUCTION_KEYS -DMODEM_RACK


ZIPDB  =        -UREMOTE_DEBUG -DENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -DAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-DSHOW_BOOTBLOCK_TRANSITIONS -DDIALING_FROM_UK -DSECURE_RELATIONSHIP_SERVER \
		-UMANUFACTURER_KEY_X509 -DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-UETHERNET -DTEST_CRON -DSEND_SERIALID_LENGTH -UNAVIO_SERVER \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY -UPRODUCTION_KEYS -DMODEM_RACK


NODB  =         -UREMOTE_DEBUG -UENABLE_DEBUGGING -DCHIMPANZEE_INTERFACE \
		-DPOINTLESS_BUTTONS -DAUTHENTICATE_SIGNATURES -UDEFEAT_BROWSER_LIMIT \
		-USHOW_BOOTBLOCK_TRANSITIONS -DDIALING_FROM_UK -DSECURE_RELATIONSHIP_SERVER \
		-UMANUFACTURER_KEY_X509 -DLIMIT_RUNAWAY_UIDS -UHIDEOUS_PROXY_HACK -UNVRAM_DEBUG \
		-UETHERNET -UTEST_CRON -DSEND_SERIALID_LENGTH -UNAVIO_SERVER \
		-DENABLE_HTTP_PROXY -DENABLE_HTTPS_PROXY -UPRODUCTION_KEYS -DMODEM_RACK \
		-DRELSERVER_PORT=:8087 -DRELSERVER_PROXY_PORT=:8081

#
# select one of these
#

#NCTARGETARCH = ${ETHERNETDB}
#NCTARGETARCH = ${ZIPDB}
NCTARGETARCH = ${NODB}

CCFLAGS = -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${NCTARGETARCH} ${NCMAFLAGS}





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
COMPONENT = NCMA+
TARGET    = aof.NCMA+
TARGETSA  = rm.NCMA+
EXPORTS   =
BASEOBJS  = module.o header.o vectors.o ncma_cmds.o ncma_swis.o \
	  dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	  ncma_utils.o ncma_dial.o kprintf.o ncma_smurf.o \
	  ncma_cron.o ncma_exec.o ncma_atr.o ncma_bootb.o ncma_mime.o \
	  ncma_tags.o ncma_nvram.o ncma_cache.o ncma_uid.o ncma_net.o \
	  usermode.o ncma_time.o areacode.o ncma_auth.o ncma_fx.o ncma_sock.o

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
        ${CP} <NCMA$SourceKey> <resource$dir>.Resources.NCMA.MFRKEY ${CPFLAGS}
        ${CP} Resources.Boot <resource$dir>.Resources.NCMA.Boot ${CPFLAGS}
        ${CP} Resources.BootFast <resource$dir>.Resources.NCMA.BootFast ${CPFLAGS}
        ${CP} Resources.BootUpdate <resource$dir>.Resources.NCMA.BootUpdate ${CPFLAGS}
        
        @echo ${COMPONENT}: resource files copied

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${DBUGLIB} ${SOCKLIB} ${INETLIB}
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
o.ncma_time:	c.ncma_time
o.ncma_time:	C:h.kernel
o.ncma_time:	C:h.swis
o.ncma_time:	h.ncma_time
o.ncma_time:	h.ncma_tags
o.ncma_time:	h.ncma_utils
o.ncma_time:	h.ncma_nvram
o.ncma_time:	h.smcard
o.ncma_time:	h.DBUG
o.ncma_auth:	c.ncma_auth
o.ncma_auth:	C:h.kernel
o.ncma_auth:	C:h.swis
o.ncma_auth:	h.ncma_auth
o.ncma_auth:	h.ncma_ncma
o.ncma_auth:	h.ncma_bootb
o.ncma_auth:	h.DBUG
o.ncma_fx:	c.ncma_fx
o.ncma_fx:	C:h.kernel
o.ncma_fx:	C:h.swis
o.ncma_fx:	h.ncma_fx
o.ncma_sock:	c.ncma_sock
o.ncma_sock:	C:h.kernel
o.ncma_sock:	C:h.swis
o.ncma_sock:	C:h.ctype
o.ncma_sock:	h.module
o.ncma_sock:	TCPIPLibs:h.socklib
o.ncma_sock:	TCPIPLibs:sys.h.types
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	TCPIPLibs:machine.h.endian
o.ncma_sock:	TCPIPLibs:machine.h.ansi
o.ncma_sock:	TCPIPLibs:machine.h.types
o.ncma_sock:	TCPIPLibs:sys.h.socket
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	TCPIPLibs:sys.h.stat
o.ncma_sock:	TCPIPLibs:sys.h.time
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	TCPIPLibs:sys.h.time
o.ncma_sock:	TCPIPLibs:sys.h.uio
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	C:h.kernel
o.ncma_sock:	TCPIPLibs:h.inetlib
o.ncma_sock:	TCPIPLibs:sys.h.types
o.ncma_sock:	TCPIPLibs:netinet.h.in
o.ncma_sock:	TCPIPLibs:sys.h.ioctl
o.ncma_sock:	TCPIPLibs:sys.h.ttycom
o.ncma_sock:	TCPIPLibs:sys.h.ioccom
o.ncma_sock:	TCPIPLibs:sys.h.ioccom
o.ncma_sock:	TCPIPLibs:sys.h.filio
o.ncma_sock:	TCPIPLibs:sys.h.ioccom
o.ncma_sock:	TCPIPLibs:sys.h.sockio
o.ncma_sock:	TCPIPLibs:sys.h.ioccom
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	TCPIPLibs:sys.h.errno
o.ncma_sock:	C:h.kernel
o.ncma_sock:	TCPIPLibs:h.netdb
o.ncma_sock:	TCPIPLibs:sys.h.cdefs
o.ncma_sock:	h.ncma_sock
o.ncma_sock:	h.ncma_cache
o.ncma_sock:	h.DBUG
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
o.module:	h.DBUG
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
o.vectors:	h.ncma_sock
o.ncma_cmds:	c.ncma_cmds
o.ncma_cmds:	C:h.kernel
o.ncma_cmds:	C:h.swis
o.ncma_cmds:	h.ncma_cmds
o.ncma_cmds:	h.ncma_ncma
o.ncma_cmds:	h.ncma_utils
o.ncma_cmds:	h.ncma_url
o.ncma_cmds:	h.ncma_scard
o.ncma_cmds:	h.ncma_cache
o.ncma_cmds:	h.ncma_mime
o.ncma_cmds:	h.ncma_dial
o.ncma_cmds:	h.ncma_ROM
o.ncma_cmds:	h.smcard
o.ncma_cmds:	h.machine
o.ncma_cmds:	h.ncma_cron
o.ncma_cmds:	h.ncma_time
o.ncma_cmds:	h.vectors
o.ncma_cmds:	h.kprintf
o.ncma_cmds:	h.dynalloc
o.ncma_cmds:	h.DBUG
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
o.machine:	h.ncma_cron
o.machine:	h.ncma_exec
o.machine:	h.ncma_smurf
o.machine:	h.ncma_atr
o.machine:	h.ncma_cache
o.machine:	h.ncma_bootb
o.machine:	h.ncma_mime
o.machine:	h.ncma_nvram
o.machine:	h.ncma_auth
o.machine:	h.ncma_net
o.machine:	h.ncma_fx
o.machine:	h.ncma_sock
o.machine:	h.vectors
o.machine:	h.DBUG
o.ncma_url:	c.ncma_url
o.ncma_url:	C:h.kernel
o.ncma_url:	C:h.swis
o.ncma_url:	h.ncma_ncma
o.ncma_url:	h.ncma_url
o.ncma_url:	h.ncma_scard
o.ncma_url:	h.ncma_cache
o.ncma_url:	h.ncma_tags
o.ncma_url:	h.ncma_bootb
o.ncma_url:	h.ncma_ROM
o.ncma_url:	h.smcard
o.ncma_url:	h.ncma_uid
o.ncma_url:	h.ncma_nvram
o.ncma_url:	h.ncma_net
o.ncma_url:	h.smcard
o.ncma_url:	h.dynalloc
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
o.ncma_utils:	h.ncma_smurf
o.ncma_utils:	h.smcard
o.ncma_utils:	h.DBUG
o.ncma_dial:	c.ncma_dial
o.ncma_dial:	C:h.kernel
o.ncma_dial:	C:h.swis
o.ncma_dial:	h.machine
o.ncma_dial:	h.vectors
o.ncma_dial:	h.ncma_dial
o.ncma_dial:	h.DBUG
o.kprintf:	c.kprintf
o.kprintf:	C:h.kernel
o.kprintf:	C:h.swis
o.kprintf:	h.kprintf
o.ncma_smurf:	c.ncma_smurf
o.ncma_smurf:	C:h.kernel
o.ncma_smurf:	C:h.swis
o.ncma_smurf:	h.ncma_smurf
o.ncma_smurf:	h.ncma_scard
o.ncma_smurf:	h.ncma_cache
o.ncma_smurf:	h.ncma_utils
o.ncma_smurf:	h.DBUG
o.ncma_cron:	c.ncma_cron
o.ncma_cron:	C:h.kernel
o.ncma_cron:	C:h.swis
o.ncma_cron:	h.ncma_nvram
o.ncma_cron:	h.ncma_smurf
o.ncma_cron:	h.ncma_exec
o.ncma_cron:	h.ncma_ncma
o.ncma_cron:	h.ncma_dial
o.ncma_cron:	h.machine
o.ncma_cron:	h.ncma_cron
o.ncma_cron:	h.DBUG
o.ncma_exec:	c.ncma_exec
o.ncma_exec:	C:h.kernel
o.ncma_exec:	C:h.swis
o.ncma_exec:	h.ncma_exec
o.ncma_exec:	h.ncma_smurf
o.ncma_exec:	h.DBUG
o.ncma_atr:	c.ncma_atr
o.ncma_atr:	C:h.kernel
o.ncma_atr:	C:h.swis
o.ncma_atr:	h.ncma_atr
o.ncma_atr:	h.ncma_scard
o.ncma_atr:	h.DBUG
o.ncma_bootb:	c.ncma_bootb
o.ncma_bootb:	C:h.kernel
o.ncma_bootb:	C:h.swis
o.ncma_bootb:	h.ncma_bootb
o.ncma_bootb:	h.ncma_ROM
o.ncma_bootb:	h.smcard
o.ncma_bootb:	h.ncma_tags
o.ncma_bootb:	h.ncma_scard
o.ncma_bootb:	h.ncma_cache
o.ncma_bootb:	h.ncma_url
o.ncma_bootb:	h.ncma_uid
o.ncma_bootb:	h.ncma_utils
o.ncma_bootb:	h.ncma_nvram
o.ncma_bootb:	h.ncma_auth
o.ncma_bootb:	h.DBUG
o.ncma_mime:	c.ncma_mime
o.ncma_mime:	C:h.kernel
o.ncma_mime:	C:h.swis
o.ncma_mime:	h.machine
o.ncma_mime:	h.dynalloc
o.ncma_mime:	h.ncma_mime
o.ncma_mime:	h.ncma_tags
o.ncma_mime:	h.ncma_nvram
o.ncma_mime:	h.ncma_utils
o.ncma_mime:	h.ncma_bootb
o.ncma_mime:	h.ncma_url
o.ncma_mime:	h.ncma_cache
o.ncma_mime:	h.ncma_scard
o.ncma_mime:	h.ncma_net
o.ncma_mime:	h.ncma_time
o.ncma_mime:	h.ncma_auth
o.ncma_mime:	h.smcard
o.ncma_mime:	h.DBUG
o.ncma_tags:	c.ncma_tags
o.ncma_tags:	h.ncma_tags
o.ncma_tags:	h.ncma_utils
o.ncma_tags:	h.DBUG
o.ncma_nvram:	c.ncma_nvram
o.ncma_nvram:	C:h.kernel
o.ncma_nvram:	C:h.swis
o.ncma_nvram:	h.ncma_nvram
o.ncma_nvram:	h.ncma_bootb
o.ncma_nvram:	h.ncma_tags
o.ncma_nvram:	h.areacode
o.ncma_nvram:	h.smcard
o.ncma_nvram:	h.ncma_utils
o.ncma_nvram:	h.ncma_time
o.ncma_nvram:	h.dynalloc
o.ncma_nvram:	h.DBUG
o.ncma_cache:	c.ncma_cache
o.ncma_cache:	C:h.kernel
o.ncma_cache:	C:h.swis
o.ncma_cache:	h.smcard
o.ncma_cache:	h.dynalloc
o.ncma_cache:	h.ncma_cache
o.ncma_cache:	h.ncma_utils
o.ncma_cache:	h.ncma_tags
o.ncma_cache:	h.ncma_bootb
o.ncma_cache:	h.ncma_time
o.ncma_cache:	h.kprintf
o.ncma_cache:	h.DBUG
o.ncma_uid:	c.ncma_uid
o.ncma_uid:	C:h.kernel
o.ncma_uid:	C:h.swis
o.ncma_uid:	h.smcard
o.ncma_uid:	h.ncma_ROM
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
o.ncma_net:	h.ncma_smurf
o.ncma_net:	h.smcard
o.ncma_net:	h.DBUG
