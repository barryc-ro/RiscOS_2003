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
o.driver:	mip.c.driver
o.driver:	mip.h.globals
o.driver:	mip.h.host
o.driver:	ccacorn.h.options
o.driver:	arm.h.target
o.driver:	mip.h.defaults
o.driver:	mip.h.defs
o.driver:	C:CLX.h.ieeeflt
o.driver:	mip.h.compiler
o.driver:	C:CLX.h.fname
o.driver:	mip.h.version
o.driver:	mip.h.drivhelp
o.driver:	mip.h.mcdep
o.driver:	mip.h.cgdefs
o.driver:	mip.h.jopcode
o.driver:	C:CLX.h.prgname
o.driver:	riscos.h.dde
o.aaof:	arm.c.aaof
o.aaof:	mip.h.globals
o.aaof:	mip.h.host
o.aaof:	ccacorn.h.options
o.aaof:	arm.h.target
o.aaof:	mip.h.defaults
o.aaof:	mip.h.defs
o.aaof:	C:CLX.h.ieeeflt
o.aaof:	mip.h.mcdep
o.aaof:	mip.h.cgdefs
o.aaof:	mip.h.jopcode
o.aaof:	arm.h.mcdpriv
o.aaof:	mip.h.codebuf
o.aaof:	mip.h.xrefs
o.aaof:	ccacorn.h.errors
o.aaof:	mip.h.xrefs
o.aaof:	mip.h.store
o.aaof:	mip.h.codebuf
o.aaof:	mip.h.version
o.aaof:	mip.h.builtin
o.aaof:	mip.h.bind
o.aaof:	ccacorn.h.errors
o.aaof:	C:CLX.h.chunkfmt
o.aaof:	C:CLX.h.aof
o.aaof:	mip.h.cg
o.aaof:	arm.h.ops
o.asd:	arm.c.asd
o.asd:	mip.h.globals
o.asd:	mip.h.host
o.asd:	ccacorn.h.options
o.asd:	arm.h.target
o.asd:	mip.h.defaults
o.asd:	mip.h.defs
o.asd:	C:CLX.h.ieeeflt
o.asd:	mip.h.mcdep
o.asd:	mip.h.cgdefs
o.asd:	mip.h.jopcode
o.asd:	arm.h.mcdpriv
o.asd:	mip.h.codebuf
o.asd:	mip.h.xrefs
o.asd:	mip.h.aeops
o.asd:	ccacorn.h.errors
o.asd:	mip.h.xrefs
o.asd:	mip.h.store
o.asd:	mip.h.codebuf
o.asd:	mip.h.regalloc
o.asd:	mip.h.util
o.asd:	mip.h.bind
o.asd:	cfe.h.sem
o.asd:	mip.h.builtin
o.asd:	ccacorn.h.errors
o.asd:	C:CLX.h.asdfmt
o.asm:	arm.c.asm
o.asm:	mip.h.globals
o.asm:	mip.h.host
o.asm:	ccacorn.h.options
o.asm:	arm.h.target
o.asm:	mip.h.defaults
o.asm:	mip.h.defs
o.asm:	C:CLX.h.ieeeflt
o.asm:	mip.h.mcdep
o.asm:	mip.h.cgdefs
o.asm:	mip.h.jopcode
o.asm:	arm.h.mcdpriv
o.asm:	mip.h.codebuf
o.asm:	mip.h.xrefs
o.asm:	mip.h.xrefs
o.asm:	mip.h.store
o.asm:	mip.h.codebuf
o.asm:	arm.h.ops
o.asm:	mip.h.version
o.asm:	mip.h.builtin
o.asm:	ccacorn.h.errors
o.asm:	C:CLX.h.disass
o.gen:	arm.c.gen
o.gen:	mip.h.globals
o.gen:	mip.h.host
o.gen:	ccacorn.h.options
o.gen:	arm.h.target
o.gen:	mip.h.defaults
o.gen:	mip.h.defs
o.gen:	C:CLX.h.ieeeflt
o.gen:	mip.h.mcdep
o.gen:	mip.h.cgdefs
o.gen:	mip.h.jopcode
o.gen:	arm.h.mcdpriv
o.gen:	mip.h.codebuf
o.gen:	mip.h.xrefs
o.gen:	mip.h.aeops
o.gen:	mip.h.xrefs
o.gen:	arm.h.ops
o.gen:	mip.h.jopcode
o.gen:	mip.h.store
o.gen:	mip.h.codebuf
o.gen:	mip.h.regalloc
o.gen:	mip.h.builtin
o.gen:	mip.h.bind
o.gen:	mip.h.cg
o.gen:	mip.h.flowgraf
o.gen:	ccacorn.h.errors
o.mcdep:	arm.c.mcdep
o.mcdep:	mip.h.globals
o.mcdep:	mip.h.host
o.mcdep:	ccacorn.h.options
o.mcdep:	arm.h.target
o.mcdep:	mip.h.defaults
o.mcdep:	mip.h.defs
o.mcdep:	C:CLX.h.ieeeflt
o.mcdep:	mip.h.mcdep
o.mcdep:	mip.h.cgdefs
o.mcdep:	mip.h.jopcode
o.mcdep:	arm.h.mcdpriv
o.mcdep:	mip.h.codebuf
o.mcdep:	mip.h.xrefs
o.mcdep:	mip.h.compiler
o.peephole:	arm.c.peephole
o.peephole:	mip.h.globals
o.peephole:	mip.h.host
o.peephole:	ccacorn.h.options
o.peephole:	arm.h.target
o.peephole:	mip.h.defaults
o.peephole:	mip.h.defs
o.peephole:	C:CLX.h.ieeeflt
o.peephole:	mip.h.mcdep
o.peephole:	mip.h.cgdefs
o.peephole:	mip.h.jopcode
o.peephole:	arm.h.mcdpriv
o.peephole:	mip.h.codebuf
o.peephole:	mip.h.xrefs
o.peephole:	mip.h.aeops
o.peephole:	arm.h.ops
o.peephole:	mip.h.jopcode
o.peephole:	mip.h.store
o.peephole:	ccacorn.h.errors
o.peephole:	mip.h.cg
o.peephole:	mip.h.regalloc
o.peephole:	ccacorn.c.peeppat
o.aetree:	mip.c.aetree
o.aetree:	mip.h.globals
o.aetree:	mip.h.host
o.aetree:	ccacorn.h.options
o.aetree:	arm.h.target
o.aetree:	mip.h.defaults
o.aetree:	mip.h.defs
o.aetree:	C:CLX.h.ieeeflt
o.aetree:	mip.h.aetree
o.aetree:	mip.h.defs
o.aetree:	mip.h.store
o.aetree:	mip.h.aeops
o.aetree:	mip.h.builtin
o.bind:	mip.c.bind
o.bind:	mip.h.globals
o.bind:	mip.h.host
o.bind:	ccacorn.h.options
o.bind:	arm.h.target
o.bind:	mip.h.defaults
o.bind:	mip.h.defs
o.bind:	C:CLX.h.ieeeflt
o.bind:	mip.h.defs
o.bind:	mip.h.aetree
o.bind:	mip.h.util
o.bind:	mip.h.codebuf
o.bind:	mip.h.xrefs
o.bind:	mip.h.cgdefs
o.bind:	mip.h.bind
o.bind:	mip.h.builtin
o.bind:	cfe.h.lex
o.bind:	cfe.h.sem
o.bind:	mip.h.store
o.bind:	cfe.h.vargen
o.bind:	mip.h.xrefs
o.bind:	ccacorn.h.errors
o.bind:	mip.h.aeops
o.builtin:	mip.c.builtin
o.builtin:	mip.h.globals
o.builtin:	mip.h.host
o.builtin:	ccacorn.h.options
o.builtin:	arm.h.target
o.builtin:	mip.h.defaults
o.builtin:	mip.h.defs
o.builtin:	C:CLX.h.ieeeflt
o.builtin:	mip.h.defs
o.builtin:	mip.h.builtin
o.builtin:	cfe.h.sem
o.builtin:	mip.h.bind
o.builtin:	mip.h.store
o.builtin:	mip.h.aeops
o.builtin:	mip.h.aetree
o.cg:	mip.c.cg
o.cg:	mip.h.globals
o.cg:	mip.h.host
o.cg:	ccacorn.h.options
o.cg:	arm.h.target
o.cg:	mip.h.defaults
o.cg:	mip.h.defs
o.cg:	C:CLX.h.ieeeflt
o.cg:	mip.h.cg
o.cg:	mip.h.cgdefs
o.cg:	mip.h.jopcode
o.cg:	mip.h.store
o.cg:	mip.h.codebuf
o.cg:	mip.h.xrefs
o.cg:	mip.h.aeops
o.cg:	mip.h.util
o.cg:	mip.h.xrefs
o.cg:	mip.h.jopcode
o.cg:	mip.h.regalloc
o.cg:	mip.h.regsets
o.cg:	mip.h.cse
o.cg:	mip.h.sr
o.cg:	mip.h.flowgraf
o.cg:	mip.h.mcdep
o.cg:	mip.h.aetree
o.cg:	mip.h.builtin
o.cg:	cfe.h.sem
o.cg:	cfe.h.simplify
o.cg:	mip.h.bind
o.cg:	ccacorn.h.errors
o.codebuf:	mip.c.codebuf
o.codebuf:	mip.h.globals
o.codebuf:	mip.h.host
o.codebuf:	ccacorn.h.options
o.codebuf:	arm.h.target
o.codebuf:	mip.h.defaults
o.codebuf:	mip.h.defs
o.codebuf:	C:CLX.h.ieeeflt
o.codebuf:	mip.h.codebuf
o.codebuf:	mip.h.xrefs
o.codebuf:	mip.h.cgdefs
o.codebuf:	mip.h.store
o.codebuf:	mip.h.xrefs
o.codebuf:	mip.h.bind
o.codebuf:	mip.h.builtin
o.codebuf:	mip.h.util
o.codebuf:	mip.h.mcdep
o.codebuf:	mip.h.jopcode
o.codebuf:	ccacorn.h.errors
o.codebuf:	mip.h.aeops
o.compiler:	mip.c.compiler
o.compiler:	mip.h.globals
o.compiler:	mip.h.host
o.compiler:	ccacorn.h.options
o.compiler:	arm.h.target
o.compiler:	mip.h.defaults
o.compiler:	mip.h.defs
o.compiler:	C:CLX.h.ieeeflt
o.compiler:	mip.h.compiler
o.compiler:	C:CLX.h.fname
o.compiler:	cfe.h.pp
o.compiler:	cfe.h.lex
o.compiler:	cfe.h.syn
o.compiler:	cfe.h.sem
o.compiler:	mip.h.bind
o.compiler:	mip.h.builtin
o.compiler:	cfe.h.vargen
o.compiler:	mip.h.aetree
o.compiler:	mip.h.cg
o.compiler:	mip.h.cgdefs
o.compiler:	mip.h.jopcode
o.compiler:	mip.h.mcdep
o.compiler:	mip.h.aeops
o.compiler:	mip.h.xrefs
o.compiler:	mip.h.store
o.compiler:	mip.h.version
o.compiler:	ccacorn.h.errors
o.compiler:	riscos.h.dde
o.compiler:	riscos.h.riscos
o.compiler:	C:h.kernel
o.cse:	mip.c.cse
o.cse:	mip.h.globals
o.cse:	mip.h.host
o.cse:	ccacorn.h.options
o.cse:	arm.h.target
o.cse:	mip.h.defaults
o.cse:	mip.h.defs
o.cse:	C:CLX.h.ieeeflt
o.cse:	mip.h.cse
o.cse:	mip.h.cgdefs
o.cse:	mip.h.cseguts
o.cse:	mip.h.regsets
o.cse:	mip.h.jopcode
o.cse:	mip.h.store
o.cse:	mip.h.regalloc
o.cse:	mip.h.cg
o.cse:	mip.h.flowgraf
o.cse:	mip.h.mcdep
o.cse:	mip.h.builtin
o.cse:	ccacorn.h.errors
o.cse:	mip.h.aeops
o.csescan:	mip.c.csescan
o.csescan:	mip.h.globals
o.csescan:	mip.h.host
o.csescan:	ccacorn.h.options
o.csescan:	arm.h.target
o.csescan:	mip.h.defaults
o.csescan:	mip.h.defs
o.csescan:	C:CLX.h.ieeeflt
o.csescan:	mip.h.cse
o.csescan:	mip.h.cgdefs
o.csescan:	mip.h.cseguts
o.csescan:	mip.h.regsets
o.csescan:	mip.h.aeops
o.csescan:	mip.h.jopcode
o.csescan:	mip.h.store
o.csescan:	mip.h.cgdefs
o.csescan:	mip.h.builtin
o.csescan:	mip.h.mcdep
o.csescan:	mip.h.flowgraf
o.csescan:	mip.h.regalloc
o.csescan:	ccacorn.h.errors
o.csescan:	mip.h.cg
o.sr:	mip.c.sr
o.sr:	mip.h.globals
o.sr:	mip.h.host
o.sr:	ccacorn.h.options
o.sr:	arm.h.target
o.sr:	mip.h.defaults
o.sr:	mip.h.defs
o.sr:	C:CLX.h.ieeeflt
o.sr:	mip.h.aeops
o.sr:	mip.h.cgdefs
o.sr:	mip.h.jopcode
o.sr:	mip.h.regsets
o.sr:	mip.h.sr
o.sr:	mip.h.store
o.sr:	mip.h.flowgraf
o.sr:	mip.h.builtin
o.sr:	mip.h.cg
o.sr:	mip.h.regalloc
o.sr:	mip.h.bind
o.sr:	mip.h.mcdep
o.sr:	ccacorn.h.errors
o.flowgraf:	mip.c.flowgraf
o.flowgraf:	mip.h.globals
o.flowgraf:	mip.h.host
o.flowgraf:	ccacorn.h.options
o.flowgraf:	arm.h.target
o.flowgraf:	mip.h.defaults
o.flowgraf:	mip.h.defs
o.flowgraf:	C:CLX.h.ieeeflt
o.flowgraf:	mip.h.flowgraf
o.flowgraf:	mip.h.cgdefs
o.flowgraf:	mip.h.jopcode
o.flowgraf:	mip.h.store
o.flowgraf:	mip.h.cg
o.flowgraf:	mip.h.codebuf
o.flowgraf:	mip.h.xrefs
o.flowgraf:	mip.h.regalloc
o.flowgraf:	mip.h.regsets
o.flowgraf:	mip.h.aeops
o.flowgraf:	mip.h.util
o.flowgraf:	mip.h.jopcode
o.flowgraf:	mip.h.mcdep
o.flowgraf:	mip.h.builtin
o.flowgraf:	cfe.h.simplify
o.flowgraf:	mip.h.xrefs
o.flowgraf:	ccacorn.h.errors
o.flowgraf:	mip.h.sr
o.jopprint:	mip.c.jopprint
o.jopprint:	mip.h.globals
o.jopprint:	mip.h.host
o.jopprint:	ccacorn.h.options
o.jopprint:	arm.h.target
o.jopprint:	mip.h.defaults
o.jopprint:	mip.h.defs
o.jopprint:	C:CLX.h.ieeeflt
o.jopprint:	mip.h.cgdefs
o.jopprint:	mip.h.jopcode
o.jopprint:	mip.h.aeops
o.jopprint:	mip.h.aetree
o.jopprint:	mip.h.flowgraf
o.misc:	mip.c.misc
o.misc:	mip.h.globals
o.misc:	mip.h.host
o.misc:	ccacorn.h.options
o.misc:	arm.h.target
o.misc:	mip.h.defaults
o.misc:	mip.h.defs
o.misc:	C:CLX.h.ieeeflt
o.misc:	mip.h.store
o.misc:	mip.h.aeops
o.misc:	mip.h.aetree
o.misc:	cfe.h.lex
o.misc:	mip.h.util
o.misc:	riscos.h.dde
o.misc:	riscos.h.dem
o.misc:	ccacorn.h.errors
o.regalloc:	mip.c.regalloc
o.regalloc:	mip.h.globals
o.regalloc:	mip.h.host
o.regalloc:	ccacorn.h.options
o.regalloc:	arm.h.target
o.regalloc:	mip.h.defaults
o.regalloc:	mip.h.defs
o.regalloc:	C:CLX.h.ieeeflt
o.regalloc:	mip.h.regalloc
o.regalloc:	mip.h.cgdefs
o.regalloc:	ccacorn.h.errors
o.regalloc:	cfe.h.sem
o.regalloc:	mip.h.jopcode
o.regalloc:	mip.h.regsets
o.regalloc:	mip.h.store
o.regalloc:	mip.h.cg
o.regalloc:	mip.h.mcdep
o.regalloc:	mip.h.flowgraf
o.regalloc:	mip.h.builtin
o.regalloc:	mip.h.aeops
o.regalloc:	mip.h.sr
o.regsets:	mip.c.regsets
o.regsets:	mip.h.globals
o.regsets:	mip.h.host
o.regsets:	ccacorn.h.options
o.regsets:	arm.h.target
o.regsets:	mip.h.defaults
o.regsets:	mip.h.defs
o.regsets:	C:CLX.h.ieeeflt
o.regsets:	mip.h.regsets
o.regsets:	mip.h.cgdefs
o.regsets:	mip.h.store
o.store:	mip.c.store
o.store:	mip.h.globals
o.store:	mip.h.host
o.store:	ccacorn.h.options
o.store:	arm.h.target
o.store:	mip.h.defaults
o.store:	mip.h.defs
o.store:	C:CLX.h.ieeeflt
o.store:	mip.h.store
o.store:	mip.h.defs
o.store:	mip.h.mcdep
o.store:	mip.h.cgdefs
o.store:	mip.h.jopcode
o.store:	ccacorn.h.errors
o.lex:	cfe.c.lex
o.lex:	mip.h.globals
o.lex:	mip.h.host
o.lex:	ccacorn.h.options
o.lex:	arm.h.target
o.lex:	mip.h.defaults
o.lex:	mip.h.defs
o.lex:	C:CLX.h.ieeeflt
o.lex:	cfe.h.lex
o.lex:	mip.h.bind
o.lex:	cfe.h.pp
o.lex:	mip.h.store
o.lex:	mip.h.util
o.lex:	mip.h.aeops
o.lex:	ccacorn.h.errors
o.pp:	cfe.c.pp
o.pp:	mip.h.globals
o.pp:	mip.h.host
o.pp:	ccacorn.h.options
o.pp:	arm.h.target
o.pp:	mip.h.defaults
o.pp:	mip.h.defs
o.pp:	C:CLX.h.ieeeflt
o.pp:	cfe.h.pp
o.pp:	cfe.h.syn
o.pp:	mip.h.store
o.pp:	ccacorn.h.errors
o.pp:	ccacorn.c.headers
o.sem:	cfe.c.sem
o.sem:	mip.h.globals
o.sem:	mip.h.host
o.sem:	ccacorn.h.options
o.sem:	arm.h.target
o.sem:	mip.h.defaults
o.sem:	mip.h.defs
o.sem:	C:CLX.h.ieeeflt
o.sem:	cfe.h.sem
o.sem:	mip.h.bind
o.sem:	mip.h.aetree
o.sem:	mip.h.builtin
o.sem:	mip.h.aeops
o.sem:	mip.h.store
o.sem:	ccacorn.h.errors
o.sem:	mip.h.util
o.simplify:	cfe.c.simplify
o.simplify:	mip.h.globals
o.simplify:	mip.h.host
o.simplify:	ccacorn.h.options
o.simplify:	arm.h.target
o.simplify:	mip.h.defaults
o.simplify:	mip.h.defs
o.simplify:	C:CLX.h.ieeeflt
o.simplify:	cfe.h.simplify
o.simplify:	cfe.h.sem
o.simplify:	mip.h.bind
o.simplify:	mip.h.aetree
o.simplify:	mip.h.builtin
o.simplify:	mip.h.store
o.simplify:	mip.h.aeops
o.simplify:	ccacorn.h.errors
o.syn:	cfe.c.syn
o.syn:	mip.h.globals
o.syn:	mip.h.host
o.syn:	ccacorn.h.options
o.syn:	arm.h.target
o.syn:	mip.h.defaults
o.syn:	mip.h.defs
o.syn:	C:CLX.h.ieeeflt
o.syn:	cfe.h.syn
o.syn:	cfe.h.pp
o.syn:	cfe.h.lex
o.syn:	cfe.h.simplify
o.syn:	mip.h.bind
o.syn:	cfe.h.sem
o.syn:	mip.h.aetree
o.syn:	mip.h.builtin
o.syn:	cfe.h.vargen
o.syn:	mip.h.mcdep
o.syn:	mip.h.cgdefs
o.syn:	mip.h.jopcode
o.syn:	mip.h.store
o.syn:	ccacorn.h.errors
o.syn:	mip.h.aeops
o.syn:	mip.h.codebuf
o.syn:	mip.h.xrefs
o.vargen:	cfe.c.vargen
o.vargen:	mip.h.globals
o.vargen:	mip.h.host
o.vargen:	ccacorn.h.options
o.vargen:	arm.h.target
o.vargen:	mip.h.defaults
o.vargen:	mip.h.defs
o.vargen:	C:CLX.h.ieeeflt
o.vargen:	cfe.h.vargen
o.vargen:	cfe.h.lex
o.vargen:	cfe.h.syn
o.vargen:	cfe.h.sem
o.vargen:	cfe.h.simplify
o.vargen:	mip.h.bind
o.vargen:	mip.h.builtin
o.vargen:	mip.h.aetree
o.vargen:	mip.h.codebuf
o.vargen:	mip.h.xrefs
o.vargen:	mip.h.regalloc
o.vargen:	mip.h.cgdefs
o.vargen:	mip.h.mcdep
o.vargen:	mip.h.jopcode
o.vargen:	mip.h.store
o.vargen:	mip.h.aeops
o.vargen:	mip.h.util
o.vargen:	mip.h.xrefs
o.vargen:	ccacorn.h.errors
o.version:	mip.c.version
o.version:	mip.h.globals
o.version:	mip.h.host
o.version:	ccacorn.h.options
o.version:	arm.h.target
o.version:	mip.h.defaults
o.version:	mip.h.defs
o.version:	C:CLX.h.ieeeflt
o.version:	mip.h.version
o.version:	cfe.h.fevsn
o.version:	mip.h.mipvsn
o.version:	arm.h.mcvsn
o.dde:	riscos.c.dde
o.dde:	mip.h.globals
o.dde:	mip.h.host
o.dde:	ccacorn.h.options
o.dde:	arm.h.target
o.dde:	mip.h.defaults
o.dde:	mip.h.defs
o.dde:	C:CLX.h.ieeeflt
o.dde:	C:h.kernel
o.dde:	cfe.h.lex
o.dde:	ccacorn.h.errors
o.dde:	riscos.h.dde
o.dem:	riscos.c.dem
o.dem:	riscos.h.dem
