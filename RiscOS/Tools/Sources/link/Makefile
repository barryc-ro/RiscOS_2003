# Makefile for Link
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
VPATH=  @ riscos

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

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
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
INCLUDES  = -IC: -Iriscos -IC:CLX
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = link
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	o.aifcode o.hostdep o.link o.dem 

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


# Static dependencies:

o.dem: riscos.c.dem
    ${CC} ${CCFLAGS} -pcc -o $@ riscos.c.dem

# Dynamic dependencies:
o.aifcode:	c.aifcode
o.aifcode:	C:CLX.h.aif
o.aifcode:	h.aifcode
o.aifcode:	C:CLX.h.aif
o.aifcode:	c.aifcode
o.aifcode:	C:CLX.h.aif
o.aifcode:	h.aifcode
o.aifcode:	C:CLX.h.aif
o.hostdep:	c.hostdep
o.hostdep:	h.options
o.hostdep:	h.hostdep
o.hostdep:	h.options
o.hostdep:	C:h.kernel
o.link:	c.link
o.link:	h.options
o.link:	h.hostdep
o.link:	h.options
o.link:	h.msgnos
o.link:	C:CLX.h.aof
o.link:	C:CLX.h.prgname
o.link:	riscos.h.dem
o.link:	C:CLX.h.libflfmt
o.link:	C:CLX.h.chunkfmt
o.link:	C:CLX.h.aif
o.link:	h.aifcode
o.link:	C:CLX.h.aif
o.link:	C:CLX.h.asdfmt
o.link:	C:CLX.h.ovrflfmt
o.dem:	riscos.c.dem
o.dem:	riscos.h.dem
