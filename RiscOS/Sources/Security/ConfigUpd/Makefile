# Makefile for ConfigUpd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When          Who     What
# ----          ---     ----
# 01 Mar 2001   DSC     Created

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
CD      = dir
DEFMOD  = DefMod
AR      = LibFile
STRIP   = -StripDepnd
ZIP     = zip

ASFLAGS   = -depend !Depend -ffah -Stamp -quit -module
CCFLAGS   = -c ${THROWBACK} -zm -zps1 -depend !Depend -ffah ${INCLUDES} ${OPTS}
CPFLAGS   = ~cfr~v
WFLAGS    = ~c~v
CMHGFLAGS = -p -depend !Depend -throwback ${OPTS}
INCLUDES  = -IC: -IC:NCMAlib -IC:AsmUtils, -IC:TCPIPLibs
DIRS      = o._dirs

#
# Libraries
#
LIBDIR     = <Lib$Dir>
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.c_AbsSym
SOCKLIB    = TCPIPLibs:o.socklibzm
INETLIB    = TCPIPLibs:o.inetlibzm
UNIXLIB    = TCPIPLibs:o.unixlibzm
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz
LIBSD      = ${LIBDIR}.DebugLib.o.debuglibzm
NCMALIB    = ${LIBDIR}.ncmalib.o.ncmalibzm
LIBSD      = ${LIBDIR}.DebugLib.o.debuglibzm

#
# Rule patterns
#
.SUFFIXES:  .oz .o .s .c .i .cmhg .inst .io
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<


#
# Program specific options:
#
COMPONENT = ConfigUpd
TARGET    = aof.ConfigUpd
TARGETSA  = rm.ConfigUpd
EXPORTS   = 
OBJS      = o.main o.nvram o.utils o.exclusions o.header o.module o.errors o.validation o.output


#
# Generic rules:
#
all: ${TARGETSA} ${DIRS}
        @echo ${COMPONENT}: all complete

rom: ${TARGET} ${DIRS}
        @echo ${COMPONENT}: rom complete

clean:
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
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
          ${MKDIR} map
          create ${DIRS}
          stamp  ${DIRS}

resources:
        TokenCheck LocalRes:Messages
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied


#
# Executable target (static link)
#
${TARGETSA}: ${OBJS} ${CLIB} ${LIBS} ${NCMALIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${LIBS} ${NCMALIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBS} ${NCMALIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${LIBS} ${NCMALIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
