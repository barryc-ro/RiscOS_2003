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
STRIP   = stripdepnd

THROWBACK = -Throwback


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p -depend !Depend -throwback
INCLUDES= -IC:,TCPIPLibs: -IC:AsmUtils

DIRS    = o._dirs

CCFLAGS = -Wp -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${TYPE} ${OPTS} ${DEBUG} -DUserIF_${USERIF}





# Libraries
#
LIBDIR	   = <Lib$Dir>
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.c_AbsSym
SOCKLIB	   = TCPIPLibs:o.socklibzm
INETLIB	   = TCPIPLibs:o.inetlibzm
UNIXLIB    = TCPIPLibs:o.unixlibzm
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz
LIBSD      = ${LIBDIR}.DebugLib.o.debuglibzm
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
	    areacode.o dbugzm.o ncma_fx.o init.o

# Instrumentation targets.
OBJSIO = io.module header.o io.vectors io.ncma_cmds io.ncma_swis \
	    io.dynalloc io.ncma_ncma io.machine io.ncma_url io.ncma_scard \
	    io.ncma_utils io.ncma_dial io.kprintf  \
	    io.ncma_bootb io.task \
	    io.ncma_tags io.ncma_cache io.ncma_uid io.ncma_net \
	    areacode.o io.dbugzm io.ncma_fx io.init io.instlib

# Preprocess targets.
OBJSI  = i.module header.o i.vectors i.ncma_cmds i.ncma_swis \
	    i.dynalloc i.ncma_ncma i.machine i.ncma_url i.ncma_scard \
	    i.ncma_utils i.ncma_dial i.kprintf  \
	    i.ncma_bootb i.task \
	    i.ncma_tags i.ncma_cache i.ncma_uid i.ncma_net \
	    areacode.o i.dbugzm i.ncma_fx i.init


ROMOBJS = ${BASEOBJS}
NOTROMOBJS = ${BASEOBJS}

#
# Component build rules
#
#o.resources: s.resources ${NOTROMRESDEP}
#oz.resources: s.resources ${ROMRESDEP}


#
# Generic rules:
#
all: ${TARGETSA} ${DIRS}
        @echo ${COMPONENT}: all complete

rom: ${TARGET} ${DIRS}
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
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# Create local directories
#
${DIRS}:
          ${MKDIR} o
          ${MKDIR} oz
          ${MKDIR} aof
          ${MKDIR} rm
          ${MKDIR} i
          ${MKDIR} io
          ${MKDIR} inst
          ${MKDIR} linked
          ${MKDIR} map
          create ${DIRS}
          stamp  ${DIRS}

#
# Preprocess target
#
preprocess: ${OBJSI} ${DIRS}
          @echo ${COMPONENT}: Preprocess build complete


#
# Instrumentation target
#
instrument: ${OBJSIO} ${DIRS}
          ${LD} -o rm.${COMPONENT} -module ${OBJSIO} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}
          @echo ${COMPONENT}: instrument build complete



#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}

#
# Exported interface headers
#
${EXP_C_H}.smtags:	h.smtags
	${CP} h.smtags $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${C++LIB}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS} ${C++LIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
