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
STRIP   = stripdepnd
# THROWBACK = -throwback


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p -depend !Depend ${THROWBACK} ${OPTS}
INCLUDES= -IC: -ITCPIPLibs: -IC:AsmUtils -IC:NCMAlib

DIRS    = o._dirs

#
# Different compile options - choose one
#
# use LOCAL_URL_PATHS to get pages from ispc: instead of ispcScr:
# use DIAL_ON_COMPLETION to dial immediately ISPCapture is completed
#  (orthogonal behaviour is to return to the connect to service screen)


NCTARGETARCH = -ULOCAL_URL_PATHS -UDIAL_ON_COMPLETION

# OPTS flag in build options allows several options to be used :

# CCIJ10 enables the Funai3 card format, otherwise its the Acorn format
# EMAIL_URL lets it be known that we have an EMAIL_URL to fill in the smartcard, it's only
# relevant for Acorn format card.

CCFLAGS = -c ${THROWBACK} -depend !Depend -wp ${INCLUDES} -zM -DCOMPAT_INET4 ${OPTS} ${NCTARGETARCH} ${OPTS} ${DEBUG}





# Libraries
#
LIBDIR     = <Lib$Dir>
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
SOCKLIB	   = TCPIPLibs:o.socklibzm
INETLIB	   = TCPIPLibs:o.inetlibzm
UNIXLIB    = TCPIPLibs:o.unixlibzm
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz
NCMALIB    = ${LIBDIR}.ncmalib.o.ncmalibzm
LIBSD      = <Lib$Dir>.DebugLib.o.debuglibzm
ASMUTILS   = C:AsmUtils.o.AsmUtilsZM

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
COMPONENT = ISPCapture
TARGET    = aof.ISPCapture
TARGETSA  = rm.ISPCapture
EXPORTS   =

# Normal targets
BASEOBJS  = module.o header.o ispc_cmds.o ispc_ispc.o \
            ispc_url.o kprintf.o ispc_file.o \
            dbugzm.o

# Preprocess targets.
OBJSI  = i.module header.o i.ispc_cmds i.ispc_ispc \
         i.ispc_url i.kprintf i.ispc_utils i.ispc_file i.dbugzm o.resources

# Instrumentation targets.
OBJSIO = io.module header.o io.ispc_cmds io.ispc_ispc \
         io.ispc_url io.kprintf io.ispc_utils io.ispc_file \
         io.dbugzm io.instlib

ROMOBJS = oz.resources ${BASEOBJS}
NOTROMOBJS = o.resources ${DBUGLIB} ${BASEOBJS}
NOTROMOBJSIO = o.resources ${DBUGLIB} ${OBJSIO}

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
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${STRIP}
        remove h.header
        @echo ${COMPONENT}: cleaned

#
# Create local directories
#
${DIRS}:
          ${MKDIR} o
          ${MKDIR} oz
          ${MKDIR} aof
          ${MKDIR} rm
          ${MKDIR} linked
          ${MKDIR} i
          ${MKDIR} io
          ${MKDIR} inst
          create ${DIRS}
          stamp  ${DIRS}

#
# Preprocess target - create preprocess C code.
#
preprocess: ${OBJSI} ${DIRS}
          @echo ${COMPONENT}: Preprocess build complete

#
# Instrumentation target - create a McCabe-enabled instrumented executable
#
instrument: ${OBJSIO} ${DIRS}
          ${LD} -o rm.${COMPONENT} -module ${NOTROMOBJSIO} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}
          @echo ${COMPONENT}: instrument build complete


#
# resources - copy resources over into correct location.
#
resources:
        ${MKDIR} ${HTMLDIR}
        ${MKDIR} ${HTMLDIR}.ISPCapture
        ${MKDIR} ${HTMLDIR}.MA
        ${CP} ${LDIR}.HTML.connect ${HTMLDIR}.MA.connect ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC1 ${HTMLDIR}.ISPCapture.ISPC1 ${CPFLAGS}
        ${CP} ${LDIR}.HTML.ISPC2 ${HTMLDIR}.ISPCapture.ISPC2 ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

BBETYPE = ispcapture
bbe-ispcapture: bbe-generic-resources-get-alias
        BBE_Export_Dir Resources.${LOCALE}.${USERIF}

#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB} ${NCMALIB} ${DIRS} header.h 
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${NCMALIB} ${RLIB} ${ASMUTILS}


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${NCMALIB} ${DIRS} header.h
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${ASMUTILS} ${NCMALIB} ${LIBSD}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} ${LIBSD} ${LIBS} ${NCMALIB} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
