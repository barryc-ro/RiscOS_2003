# Makefile for RMASpy

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
CMHGFLAGS = -p -depend !Depend ${OPTS}
INCLUDES= -IC:,TCPIPLibs: 

DIRS    = o._dirs

CCFLAGS = -Wp -c ${THROWBACK} -depend !Depend ${INCLUDES} -zM ${TYPE} ${OPTS} ${DEBUG}





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
COMPONENT = RMASpy
TARGET    = aof.${COMPONENT}
TARGETSA  = rm.${COMPONENT}
EXPORTS   = 

# Normal ROM, RAM and debug objects.
BASEOBJS  = header.o Module.o Asm.o

# Instrumentation targets.
OBJSIO = header.o io.Module o.Asm

# Preprocess targets.
OBJSI  = header.o i.Module Asm.o


ROMOBJS = ${BASEOBJS}
NOTROMOBJS = ${BASEOBJS}


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
          ${LD} -o rm.${COMPONENT} -module ${OBJSIO} ${CLIB} ${LIBS} ${LIBSD} ${UNIXLIB} ${SOCKLIB} ${INETLIB} ${RLIB}
          @echo ${COMPONENT}: instrument build complete



#
# Executable target (static link)
#
${TARGETSA}: ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${NOTROMOBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}

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


# Static dependencies:
o.header: cmhg.header
h.header: cmhg.header


# Dynamic dependencies:
