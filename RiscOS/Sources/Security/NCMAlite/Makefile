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
INCLUDES= -IC:,TCPIPLibs: 

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
# use DISALLOW_USER_CONFIRMATION_OF_LOGOUTS to disallow user confirmation of logouts. If this is defined, performing a logout will just go ahead and log the user out without bringing up the confirmation dialogue.
# use DISALLOW_USER_CONFIRMATION_OF_STANDBY to disallow user confirmation of standby. If this is defined, pressing standby will just go into standby without bringing up the confirmation dialogue.

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

CCFLAGS = -Wp -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${NCTARGETARCH} ${OPTS} ${DEBUG}





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
UNIXLIB    = TCPIPLibs:o.unixlib
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz
LIBSD      = ${LIBDIR}.DebugLib.o.debuglibzm

#
# Rule patterns
#
.SUFFIXES:  .oz .o .s .c .i .cmhg .inst .io
.c.o:;      ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"
.c.i:;      ${CC} ${CCFLAGS} -c -C -E $< >> $@
.inst.io:;  ${CC} ${CCFLAGS} -C++ -W -o $@ $<

#
# Program specific options:
#
COMPONENT = NCMAlite
TARGET    = aof.NCMAlite
TARGETSA  = rm.NCMAlite
EXPORTS   = ${EXP_C_H}.smtags

# Normal ROM, RAM an denug objects.
BASEOBJS  = module.o header.o vectors.o ncma_cmds.o ncma_swis.o \
	    dynalloc.o ncma_ncma.o machine.o ncma_url.o ncma_scard.o \
	    ncma_utils.o ncma_dial.o kprintf.o  \
	    ncma_bootb.o task.o \
	    ncma_tags.o  ncma_cache.o ncma_uid.o ncma_net.o \
	    usermode.o areacode.o dbugzm.o ncma_fx.o init.o

# Instrumentation targets.
OBJSIO = io.module header.o io.vectors io.ncma_cmds io.ncma_swis \
	    io.dynalloc io.ncma_ncma io.machine io.ncma_url io.ncma_scard \
	    io.ncma_utils io.ncma_dial io.kprintf  \
	    io.ncma_bootb io.task \
	    io.ncma_tags io.ncma_cache io.ncma_uid io.ncma_net \
	    usermode.o areacode.o io.dbugzm io.ncma_fx io.init io.instlib

# Preprocess targets.
OBJSI  = i.module header.o i.vectors i.ncma_cmds i.ncma_swis \
	    i.dynalloc i.ncma_ncma i.machine i.ncma_url i.ncma_scard \
	    i.ncma_utils i.ncma_dial i.kprintf  \
	    i.ncma_bootb i.task \
	    i.ncma_tags i.ncma_cache i.ncma_uid i.ncma_net \
	    usermode.o areacode.o i.dbugzm i.ncma_fx i.init


ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = oz.resources ${BASEOBJS}

#
# Component build rules
#
o.resources: s.resources ${NOTROMRESDEP}
oz.resources: s.resources ${ROMRESDEP}


#
# Generic rules:
#
all: ${TARGETSA} local_dirs
        @echo ${COMPONENT}: all complete

rom: ${TARGET} local_dirs
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}        
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# Create local directories
#
local_dirs:
          ${MKDIR} o
          ${MKDIR} oz
          ${MKDIR} aof
          ${MKDIR} rm
          ${MKDIR} i
          ${MKDIR} io
          ${MKDIR} inst
          ${MKDIR} linked
          ${MKDIR} map


#
# Preprocess target
#
preprocess: ${OBJSI} local_dirs
          @echo ${COMPONENT}: Preprocess build complete


#
# Instrumentation target
#
instrument: ${OBJSIO} local_dirs
          ${LD} -o rm.${COMPONENT} -module ${OBJSIO} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB}
          @echo ${COMPONENT}: instrument build complete



#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}

#
# Exported interface headers
#
${EXP_C_H}.smtags:	h.smtags
	${CP} h.smtags $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${C++LIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${C++LIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
