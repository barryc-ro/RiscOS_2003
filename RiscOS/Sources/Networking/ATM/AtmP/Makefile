# Makefile for ATM25 module
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
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
CHMOD	= access

# ----------------------------------------------------------------- #
# Flags - you can set 'em, if you like - see FlagReadMe
# ----------------------------------------------------------------- #

FLAG_OPTS = -UFIFO_RECOVER -DDEBUG_TRAIL -UDEBUG -URUNS_ON_ARM700 \
		-DLIMIT_TABLES -DPACKET_JUNKING -DUSE_DYNAMIC_AREAS \
		-DNDEBUG 


#FLAG_OPTS = -UFIFO_RECOVER -DDEBUG_TRAIL -DDEBUG -URUNS_ON_ARM700 \
#		-DLIMIT_TABLES -DPACKET_JUNKING -DUSE_DYNAMIC_AREAS
#


AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -wp -depend !Depend ${THROWBACK} -zM -zps1 -ff ${INCLUDES} ${DFLAGS} ${FLAG_OPTS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS  = -DUSE_TINY ${DEBUG}

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = C:debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:,TCPIPlibs:

# Program specific options:
#
COMPONENT = atm25stb
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS      = atmio.o buffers.o mbuf.o module.o panther.o utils.o header.o \
		localarp.o atmswis.o mast.o atmsock.o \
		dci_stuff.o resources.o callproto.o commands.o boson.o \
		listen.o pvc.o nvram.o atm_conf.o ipconv.o nbit_mask.o

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${RMTARGET}
	@echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
        ${RM} h.header
        ${RM} ${RMTARGET}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

# dummy target
h.header: cmhg.header
	@echo

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Relocatable module target
#
${RMTARGET}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

# specific files
h.cmhg: cmhg.header
        ${cmhg} -p -d h.cmhg cmhg.header

#
# Dynamic dependencies:
