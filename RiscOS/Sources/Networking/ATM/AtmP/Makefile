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
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS} ${FLAG_OPTS
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
o.panther:	c.panther
o.panther:	C:h.kernel
o.panther:	C:h.swis
o.panther:	h.types
o.panther:	h.mast
o.panther:	h.panther
o.panther:	h.atmio
o.panther:	h.macros
o.panther:	H.ASSERTION
o.panther:	h.headers
o.panther:	h.module
o.panther:	h.services
o.panther:	h.atmsock
o.panther:	h.utils
o.panther:	h.boson
o.utils:	c.utils
o.utils:	C:h.swis
o.utils:	C:h.kernel
o.utils:	C:h.kernel
o.utils:	C:h.ctype
o.utils:	h.utils
o.utils:	h.types
o.localarp:	c.localarp
o.localarp:	C:h.kernel
o.localarp:	C:h.swis
o.localarp:	TCPIPlibs:sys.h.dcistructs
o.localarp:	h.types
o.localarp:	h.module
o.localarp:	h.localarp
o.localarp:	h.utils
o.localarp:	h.dci_stuff
o.atmswis:	c.atmswis
o.atmswis:	C:h.kernel
o.atmswis:	C:h.swis
o.atmswis:	h.atmswis
o.atmswis:	h.types
o.atmswis:	TCPIPlibs:sys.h.dcistructs
o.atmswis:	h.mbuf
o.atmswis:	h.mbuftypes
o.atmswis:	C:h.stdlib
o.atmswis:	h.buffers
o.atmswis:	h.module
o.atmswis:	h.mast
o.atmswis:	h.resources
o.atmswis:	h.macros
o.atmswis:	h.macros
o.atmswis:	h.atmio
o.atmswis:	h.services
o.atmswis:	h.boson
o.atmswis:	h.headers
o.atmswis:	h.listen
o.atmswis:	h.panther
o.mast:	c.mast
o.mast:	h.types
o.mast:	h.mast
o.mast:	C:h.ctype
o.mast:	C:h.string
o.mast:	h.atmsock
o.mast:	h.services
o.mast:	h.utils
o.mast:	h.resources
o.mast:	h.atmio
o.mast:	h.macros
o.mast:	h.listen
o.mast:	h.nbit_mask
o.atmsock:	c.atmsock
o.atmsock:	h.headers
o.atmsock:	h.atmio
o.atmsock:	h.services
o.atmsock:	h.utils
o.atmsock:	h.sockcmds
o.atmsock:	h.address
o.atmsock:	h.atypes
o.atmsock:	h.flowspec
o.atmsock:	h.atypes
o.atmsock:	h.macros
o.atmsock:	C:h.kernel
o.atmsock:	C:h.swis
o.atmsock:	h.module
o.atmsock:	h.boson
o.atmsock:	h.ipconv
o.atmsock:	h.atmsock
o.dci_stuff:	c.dci_stuff
o.dci_stuff:	C:h.kernel
o.dci_stuff:	C:h.swis
o.dci_stuff:	h.dci_stuff
o.dci_stuff:	h.mbuftypes
o.dci_stuff:	h.mbuf
o.dci_stuff:	TCPIPlibs:sys.h.dcistructs
o.dci_stuff:	h.types
o.dci_stuff:	h.mast
o.dci_stuff:	h.atmswis
o.dci_stuff:	h.panther
o.dci_stuff:	h.localarp
o.dci_stuff:	h.module
o.dci_stuff:	h.resources
o.dci_stuff:	h.atmio
o.dci_stuff:	h.services
o.dci_stuff:	h.headers
o.dci_stuff:	h.boson
o.resources:	c.resources
o.resources:	h.types
o.resources:	C:h.kernel
o.resources:	C:h.swis
o.resources:	h.mast
o.resources:	h.module
o.resources:	h.resources
o.resources:	h.buffers
o.resources:	h.panther
o.callproto: s.callproto
o.commands:	c.commands
o.commands:	C:h.kernel
o.commands:	C:h.swis
o.commands:	h.module
o.commands:	h.types
o.commands:	h.mast
o.commands:	h.headers
o.commands:	h.utils
o.commands:	h.panther
o.commands:	h.atmswis
o.commands:	h.services
o.commands:	h.listen
o.commands:	h.boson
o.commands:	h.buffers
o.commands:	h.sockcmds
o.commands:	h.address
o.commands:	h.atypes
o.commands:	h.flowspec
o.commands:	h.atypes
o.commands:	C:h.ctype
o.commands:	h.resources
o.commands:	h.macros
o.commands:	h.atmio
o.commands:	h.pvc
o.commands:	h.ipconv
o.commands:	h.nbit_mask
o.boson: s.boson
o.boson: s.registers
o.boson: s.ddi_macros
o.listen:	c.listen
o.listen:	C:h.kernel
o.listen:	C:h.swis
o.listen:	h.types
o.listen:	h.services
o.listen:	h.listen
o.listen:	h.module
o.listen:	h.mast
o.listen:	h.atmsock
o.listen:	h.utils
o.listen:	h.nbit_mask
o.pvc:	c.pvc
o.pvc:	C:h.kernel
o.pvc:	C:h.swis
o.pvc:	h.headers
o.pvc:	h.module
o.pvc:	h.types
o.pvc:	h.utils
o.pvc:	h.mast
o.pvc:	h.atmio
o.pvc:	h.macros
o.pvc:	h.services
o.pvc:	h.panther
o.pvc:	H.ASSERTION
o.pvc:	h.sockcmds
o.pvc:	h.address
o.pvc:	h.atypes
o.pvc:	h.flowspec
o.pvc:	h.atypes
o.pvc:	h.resources
o.pvc:	h.boson
o.pvc:	h.listen
o.pvc:	h.pvc
o.pvc:	h.nvram
o.pvc:	h.ipconv
o.pvc:	h.nbit_mask
o.nvram: s.nvram
o.nvram: s.registers
o.atm_conf:	c.atm_conf
o.atm_conf:	C:h.stdio
o.atm_conf:	C:h.stdlib
o.atm_conf:	C:h.ctype
o.atm_conf:	C:h.kernel
o.atm_conf:	C:h.swis
o.atm_conf:	h.types
o.atm_conf:	h.module
o.atm_conf:	h.nvram
o.atm_conf:	h.atm_conf
o.ipconv: s.ipconv
o.ipconv: s.registers
o.nbit_mask: s.nbit_mask
o.nbit_mask: s.registers
o.atmio:	c.atmio
o.atmio:	C:h.kernel
o.atmio:	C:h.swis
o.atmio:	h.headers
o.atmio:	h.module
o.atmio:	h.services
o.atmio:	h.atmio
o.atmio:	h.utils
o.buffers:	c.buffers
o.buffers:	C:h.kernel
o.buffers:	C:h.swis
o.buffers:	C:h.stdlib
o.buffers:	h.boson
o.buffers:	h.module
o.buffers:	h.buffers
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	h.types
o.mbuf:	h.module
o.mbuf:	h.mbuf
o.mbuf:	h.mbuftypes
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.cmhg
o.module:	h.headers
o.module:	h.module
o.module:	h.mbuf
o.module:	h.mbuftypes
o.module:	h.buffers
o.module:	h.types
o.module:	h.utils
o.module:	TCPIPlibs:sys.h.dcistructs
o.module:	h.dci_stuff
o.module:	h.mast
o.module:	h.localarp
o.module:	h.atmio
o.module:	h.macros
o.module:	h.services
o.module:	h.panther
o.module:	h.atmswis
o.module:	H.ASSERTION
o.module:	h.sockcmds
o.module:	h.address
o.module:	h.atypes
o.module:	h.flowspec
o.module:	h.atypes
o.module:	h.resources
o.module:	h.commands
o.module:	h.boson
o.module:	h.listen
o.module:	h.nvram
o.module:	h.pvc
o.module:	h.atm_conf
o.module:	h.ipconv
o.module:	h.nbit_mask
