# Makefile for ISPCapture
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 08-Aug-97	AJS	Created
# 29-Jun-98	HKS	Added 'OPTS'
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
MKDIR   = -cdir
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
CMHGFLAGS = -p -depend !Depend ${THROWBACK} ${OPTS}
INCLUDES= -IC: -ITCPIPLibs:

#
# Different compile options - choose one
#
# use REMOTE_DEBUG to enable remote debugging
# use ENABLE_DEBUGGING if you want to enable debugging!
# use LOCAL_URL_PATHS to get pages from ispc: instead of ispcScr:
# use DIAL_ON_COMPLETION to dial immediately ISPCapture is completed
#  (orthogonal behaviour is to return to the connect to service screen)

# debugging

ETHERNETDB  =   -DREMOTE_DEBUG -DENABLE_DEBUGGING -DLOCAL_URL_PATHS -UDIAL_ON_COMPLETION

ZIPDB  =        -UREMOTE_DEBUG -DENABLE_DEBUGGING -ULOCAL_URL_PATHS -UDIAL_ON_COMPLETION

NODB  =         -UREMOTE_DEBUG -UENABLE_DEBUGGING -ULOCAL_URL_PATHS -UDIAL_ON_COMPLETION

#
# select one of these
#

#NCTARGETARCH = ${ETHERNETDB}
#NCTARGETARCH = ${ZIPDB}
NCTARGETARCH = ${NODB}

# OPTS flag in build options allows several options to be used :

# CCIJ10 enables the Funai3 card format, otherwise its the Acorn format
# EMAIL_URL lets it be known that we have an EMAIL_URL to fill in the smartcard, it's only
# relevant for Acorn format card.

CCFLAGS = -c ${THROWBACK} -depend !Depend -wp ${INCLUDES} -zM -DCOMPAT_INET4 ${OPTS} ${NCTARGETARCH} ${OPTS}





# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

SOCKLIB	   = TCPIPLibs:o.socklibzm
INETLIB	   = TCPIPLibs:o.inetlibzm

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
COMPONENT = ISPCapture
TARGET    = aof.ISPCapture
TARGETSA  = rm.ISPCapture
EXPORTS   =
BASEOBJS  = module.o header.o ispc_cmds.o dynalloc.o ispc_ispc.o \
	  ispc_url.o kprintf.o ispc_utils.o usermode.o ispc_file.o dbugzm.o

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = o.resources ${DBUGLIB} ${BASEOBJS}

RESDIR = <resource$dir>.Resources.${COMPONENT}
HTMLDIR = <resource$dir>.!Boot.HTML
LDIR = LocalRes:${USERIF}

NOTROMRESDEP = html.ISPC1 html.ISPC2 html.ISPC3 html.ISPC4

ROMRESDEP = html.ISPC1 html.ISPC2 html.ISPC3 html.ISPC4

#
# Component build rules
#

o.resources: s.resources ${NOTROMRESDEP}

oz.resources: s.resources ${ROMRESDEP}

o.header: cmhg.header

h.header: cmhg.header

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
        remove h.cmhg
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${HTMLDIR}
        ${MKDIR} ${HTMLDIR}.ISPCapture
        ${MKDIR} ${HTMLDIR}.MA
        ${CP} ${LDIR}.HTML.connect ${HTMLDIR}.MA.connect ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC1 ${HTMLDIR}.ISPCapture.ISPC1 ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC2 ${HTMLDIR}.ISPCapture.ISPC2 ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${SOCKLIB} ${INETLIB} ${LIBSD} header.h
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${SOCKLIB} ${INETLIB} ${LIBSD}

#
# Exported interface headers
#

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB} header.h
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
