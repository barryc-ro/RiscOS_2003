# Makefile for Brainlink server
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Apr-01  NB           Created

#
# Paths
#
#EXP_HDR = <cexport$dir>

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = BLServer
CMHGNAME  = header  

EXPORTS = ${EXP_HDR}.h.${COMPONENT}

DEPENDS = -depend !Depend ${THROWBACK}

DEFINES = ${OPTIONS}

AFLAGS = ${DEPENDS} -Stamp -quit -PreDefine "ATPCS32 SETL {TRUE}"
CFLAGS  = -ff -c -zM ${DEFINES} ${INCLUDES} ${DEPENDS}
CMHGFLAGS = -p ${DEPENDS}

DIRS = o._dirs

#
# Libraries
#
include Makefiles:ModuleLibs

SERIALLIB = <Lib$Dir>.BrainLink.serial.o.BLserial
ZMODEMLIB = <Lib$Dir>.BrainLink.zmodem.o.BLzmodem

#
# Include files
#
INCLUDES = -Itbox:,C:,<Lib$Dir>.BrainLink

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =	\
 o.main		\
 o.terminal     \
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build a RAM module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
	${MKDIR} ${INSTDIR}
#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${CLIB} ${SERIALLIB} ${ZMODEMLIB} ${WIMPLIB} ${ASMUTILS} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS} ${CLIB} ${SERIALLIB} ${ZMODEMLIB} ${WIMPLIB} ${ASMUTILS}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${ROMCSTUBS} ${SERIALLIB}ZM ${ZMODEMLIB}ZM ${WIMPLIB} ${ASMUTILS} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${SERIALLIB}ZM ${WIMPLIB} ${ASMUTILS} ${ZMODEMLIB}ZM 

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


o.header: cmhg.header

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

o.main: VersionNum h.${CMHGNAME}

h.${CMHGNAME}:
	cmhg -p -depend !Depend -throwback -d h.${CMHGNAME} cmhg.${CMHGNAME}

# Dynamic dependencies:
