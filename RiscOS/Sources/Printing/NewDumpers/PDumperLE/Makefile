# Makefile for PDumperLE
#

#
# Paths
#
EXP_HDR = 

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -fah ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:

# DFLAGS should be NULL for any non-debugging release
DFLAGS    =
#DFLAGS   = -DDEBUG

#
# Program specific options:
#
COMPONENT = PDumperLE
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RESDIR    = <resource$dir>.Resources.${COMPONENT}
OBJS      =	\
	o.fcifkelv o.fcintfc o.fckelvin \
	o.fcprint o.fcrotate o.module o.riscos \
	o.rm o.errors o.veneer o.pdumper \
	o.pdsupport o.lexsupport
#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

# ${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.wedge:	c.wedge
o.wedge:	h.common
o.wedge:	C:h.kernel
o.wedge:	C:h.swis
o.wedge:	h.config
o.wedge:	h.riscos
o.wedge:	h.module
o.wedge:	h.hbpxface
o.wedge:	h.hbpfunc
o.wedge:	h.fckelvin
o.wedge:	h.fclocal
o.wedge:	h.pdumper
o.wedge:	C:h.kernel
o.wedge:	h.pdsupport
o.wedge:	C:h.kernel
o.wedge:	h.lexsupport
o.wedge:	C:h.kernel
o.wedge:	h.hbpxface
o.wedge:	h.textprint
o.wedge:	C:h.kernel
o.wedge:	h.wedge
o.wedge:	C:h.kernel
o.textprint:	c.textprint
o.textprint:	h.common
o.textprint:	C:h.kernel
o.textprint:	C:h.swis
o.textprint:	h.config
o.textprint:	h.riscos
o.textprint:	h.module
o.textprint:	h.hbpxface
o.textprint:	h.hbpfunc
o.textprint:	h.fckelvin
o.textprint:	h.fclocal
o.textprint:	h.pdumper
o.textprint:	C:h.kernel
o.textprint:	h.pdsupport
o.textprint:	C:h.kernel
o.textprint:	h.lexsupport
o.textprint:	C:h.kernel
o.textprint:	h.hbpxface
o.textprint:	h.wedge
o.textprint:	C:h.kernel
o.textprint:	h.textprint
o.textprint:	C:h.kernel
o.rawsprite:	c.rawsprite
o.rawsprite:	h.rawsprite
o.rawsprite:	C:h.kernel
o.rawsprite:	C:h.kernel
o.fcifkelv:	c.fcifkelv
o.fcifkelv:	h.common
o.fcifkelv:	C:h.kernel
o.fcifkelv:	C:h.swis
o.fcifkelv:	h.config
o.fcifkelv:	h.riscos
o.fcifkelv:	h.module
o.fcifkelv:	h.hbpxface
o.fcifkelv:	h.hbpfunc
o.fcifkelv:	h.fckelvin
o.fcifkelv:	h.fclocal
o.fcintfc:	c.fcintfc
o.fcintfc:	h.common
o.fcintfc:	C:h.kernel
o.fcintfc:	C:h.swis
o.fcintfc:	h.config
o.fcintfc:	h.riscos
o.fcintfc:	h.module
o.fcintfc:	h.hbpxface
o.fcintfc:	h.hbpfunc
o.fcintfc:	h.fckelvin
o.fcintfc:	h.fclocal
o.fckelvin:	c.fckelvin
o.fckelvin:	h.common
o.fckelvin:	C:h.kernel
o.fckelvin:	C:h.swis
o.fckelvin:	h.config
o.fckelvin:	h.riscos
o.fckelvin:	h.module
o.fckelvin:	h.hbpxface
o.fckelvin:	h.hbpfunc
o.fckelvin:	h.fckelvin
o.fckelvin:	h.fclocal
o.fcprint:	c.fcprint
o.fcprint:	h.common
o.fcprint:	C:h.kernel
o.fcprint:	C:h.swis
o.fcprint:	h.config
o.fcprint:	h.riscos
o.fcprint:	h.module
o.fcprint:	h.hbpxface
o.fcprint:	h.hbpfunc
o.fcprint:	h.fckelvin
o.fcprint:	h.fclocal
o.fcrotate:	c.fcrotate
o.fcrotate:	h.common
o.fcrotate:	C:h.kernel
o.fcrotate:	C:h.swis
o.fcrotate:	h.config
o.fcrotate:	h.riscos
o.fcrotate:	h.module
o.fcrotate:	h.hbpxface
o.fcrotate:	h.hbpfunc
o.fcrotate:	h.fckelvin
o.fcrotate:	h.fclocal
o.module:	c.module
o.module:	h.common
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.config
o.module:	h.riscos
o.module:	h.module
o.module:	h.hbpxface
o.module:	h.hbpfunc
o.module:	h.fckelvin
o.module:	h.fclocal
o.module:	h.kelvswis
o.module:	h.pdumper
o.module:	C:h.kernel
o.riscos:	c.riscos
o.riscos:	h.common
o.riscos:	C:h.kernel
o.riscos:	C:h.swis
o.riscos:	h.config
o.riscos:	h.riscos
o.riscos:	h.module
o.riscos:	h.hbpxface
o.riscos:	h.hbpfunc
o.riscos:	h.fckelvin
o.riscos:	h.fclocal
o.errors: s.errors
o.veneer: s.veneer
o.pdumper:	c.pdumper
o.pdumper:	h.common
o.pdumper:	C:h.kernel
o.pdumper:	C:h.swis
o.pdumper:	h.config
o.pdumper:	h.riscos
o.pdumper:	h.module
o.pdumper:	h.hbpxface
o.pdumper:	h.hbpfunc
o.pdumper:	h.fckelvin
o.pdumper:	h.fclocal
o.pdumper:	h.pdumper
o.pdumper:	C:h.kernel
o.pdumper:	h.pdsupport
o.pdumper:	C:h.kernel
o.pdumper:	h.lexsupport
o.pdumper:	C:h.kernel
o.pdumper:	h.hbpxface
o.pdumper:	h.kelvswis
o.pdumper:	h.rawsprite
o.pdumper:	C:h.kernel
o.pdsupport:	c.pdsupport
o.pdsupport:	h.pdsupport
o.pdsupport:	C:h.kernel
o.pdsupport:	C:h.swis
o.lexsupport:	c.lexsupport
o.lexsupport:	h.lexsupport
o.lexsupport:	C:h.kernel
o.lexsupport:	h.hbpxface
o.lexsupport:	h.kelvswis
o.lexsupport:	h.hbpxface
o.lexsupport:	C:h.kernel
