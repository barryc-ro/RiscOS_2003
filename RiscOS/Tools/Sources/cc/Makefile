# Makefile for cc
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
VPATH   = @ arm cfe mip riscos util
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
INCLUDES  = -Iriscos -IC:CLX -Iarm -Icfe -Imip -Iutil -Iccacorn -IC:
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = cc
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS= o.driver o.aaof o.asd o.asm o.gen o.mcdep o.peephole o.aetree o.bind \
    o.builtin o.cg o.codebuf o.compiler o.cse o.csescan o.sr o.flowgraf \
    o.jopprint o.misc o.regalloc o.regsets o.store o.lex o.pp o.sem o.simplify \
    o.syn o.vargen o.version o.dde o.dem

HDRS=ccacorn.c.headers ccacorn.c.peeppat ccacorn.h.errors

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

riscos.h.errors: ccacorn.h.errors
        @echo

riscos.c.peeppat: ccacorn.c.peeppat
        @echo
        @echo

ccacorn.h.errors: mip.h.miperrs cfe.h.feerrs arm.h.mcerrs genhdrs
        genhdrs -eccacorn.h.errors -qmip.h.miperrs -qcfe.h.feerrs -qarm.h.mcerrs -s

ccacorn.c.headers: $(HDRPATHS) genhdrs
        genhdrs -occacorn.c.headers -Icl.stdh. h.assert h.ctype h.errno h.float h.limits h.locale h.math h.setjmp  h.signal h.stdarg h.stddef h.stdio h.stdlib h.string h.time

genhdrs: util.c.genhdrs
        ${CC} -c -o o.genhdrs util.c.genhdrs
        ${LD} -o genhdrs o.genhdrs ${CLIB}

ccacorn.c.peeppat: arm.peeppat peepgen
        peepgen arm.peeppat ccacorn.c.peeppat

peepgen: util.c.peepgen mip.h.jopcode arm.h.mcdpriv
        ${CC} -c -o o.peepgen util.c.peepgen -Imip -Iarm
        ${LD} -o peepgen o.peepgen ${CLIB}

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
        ${RM} genhdrs
        ${RM} peepgen
        ${RM} ccacorn.c.headers 
        ${RM} ccacorn.c.peeppat 
        ${RM} ccacorn.h.errors
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
${TARGET}: ${OBJS} ${LIBS} ${HDRS} local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}

# Static dependencies:

o.dem: riscos.c.dem
    ${CC} ${CCFLAGS} -pcc -o $@ riscos.c.dem

# Dynamic dependencies:
