# Makefile for CMHG
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Oct-98  SJM          Created.
#

#
# Paths
#
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
CLXLIB    = C:CLX.o.clxlib

#
# Include files
#
INCLUDES  = -IC: -IC:CLX
DFLAGS    = -DDDE

#
# Program specific options:
#
COMPONENT  = cmhg
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS=	o.cmhg o.outaof o.veneers o.throwback 

LIBS=	${CLXLIB} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

local_dirs: 
	${MKDIR} o

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}

# Dynamic dependencies:
o.cmhg:	c.cmhg
o.cmhg:	h.outaof
o.cmhg:	C:CLX.h.fname
o.cmhg:	C:h.swis
o.cmhg:	C:h.kernel
o.cmhg:	h.throwback
o.cmhg:	c.cmhg
o.cmhg:	h.outaof
o.cmhg:	C:CLX.h.fname
o.cmhg:	C:h.swis
o.cmhg:	C:h.kernel
o.cmhg:	h.throwback
o.outaof:	c.outaof
o.outaof:	h.outaof
o.outaof:	C:CLX.h.aof
o.outaof:	C:CLX.h.chunkfmt
o.veneers: s.veneers
o.throwback:	c.throwback
o.throwback:	C:h.swis
o.throwback:	C:h.kernel
o.throwback:	h.throwback
