# Makefile for NCBBnvram
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 7-Apr-97	AJS	Created
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
TOUCH   = Create
STRIP	= StripDepnd


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: 

#
# Different compile options - choose one
#
# AUTHENTICATE_SIGNATURES will cause NCBootblock to reject inauthentic smartcard data
#

# Tiny stubs, no debugging
CCFLAGS  = -c -Wp -depend !Depend ${INCLUDES} -zM ${THROWBACK} ${OPTS}


# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = NCBootBlk
TARGET    = aof.NCBootBlk
TARGETSA  = rm.NCBootblk
EXPORTS   = 
OBJS      = header.o module.o bootblock.o
DIRS	  = o._dirs


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
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
	${STRIP}
        @echo ${COMPONENT}: cleaned

#
# Manual dependencies
#
o.module: h.header

#
# Exported interface headers
#
${EXP_C_H}.smcard:	h.smcard
	${CP} h.smcard $@ ${CPFLAGS}


${TARGETSA}: ${OBJS} ${CLIB} ${LIBS} ${LIBSD} ${DIRS}
	${MKDIR} rm
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${LIBS} ${LIBSD}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${LIBS} ${LIBSD}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${DIRS}:
	${MKDIR} o
	${TOUCH} ${DIRS}


# Dynamic dependencies:
