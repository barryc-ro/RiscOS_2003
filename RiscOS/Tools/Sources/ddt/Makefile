# Makefile for DDT
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 18-May-98  KJB          Created.
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
SQUEEZE = modsqz

CCFLAGS = -c -depend !Depend -ffa -APCS 3/fpe3 ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -CPU ARM7M -Stamp -quit ${THROWBACK}
CPFLAGS = ~cfr~v
LDFLAGS = -rmf -s linktab
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
INCLUDES  = -IC:
DFLAGS    = -DERROR_BACKTRACE -DStrongARM -DUSE_DAS

#
# Program specific options:
#
COMPONENT  = DDT
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS= \
  ddt.o \
  debug.o \
  os.o \
  step.o \
  control.o \
  inst.o \
  lowlevel.o \
  lib.o \
  lookup.o \
  program.o \
  source.o \
  output.o \
  window.o \
  templates.o \
  quickdraw.o \
  respond.o \
  libs.o \
  display.o \
  evaluate.o \
  errors.o \
  print.o \
  help.o \
  palette.o \
  template.o \
  readexpr.o \
  setjmp.o \
  core.o \
  dem.o \
  cplusplus.o

LIBS=	

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# Special rules
#
binaof: utils.c.binaof
        $(CC) -o binaof utils.c.binaof

asmdefs:
        $(CC) $(DFLAGS) -o asmdefs asmdefs.c

s.defs: asmdefs
        asmdefs

palette.o:      !palette binaof
        binaof !palette o.palette palette

template.o: templates binaof
        binaof templates o.template template

o.control: s.defs
o.ddt:     s.defs
o.libs:    s.defs
o.os:      s.defs
o.setjmp:  s.defs
o.step:    s.defs

#
# build the module:
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
        ${WIPE} o ${WFLAGS}
        ${RM} binaof
        ${RM} asmdefs
        ${RM} s.defs
        ${RM} linktab
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
	${SQUEEZE} ${COMPONENT}

# Dynamic dependencies:
o.ddt: s.ddt
o.ddt: s.defs
o.debug:	c.debug
o.debug:	h.defs
o.debug:	h.os
o.debug:	h.asd
o.debug:	h.lowlevel
o.debug:	h.control
o.debug:	h.ddt
o.debug:	h.lookup
o.debug:	h.inst
o.debug:	h.lib
o.debug:	h.program
o.debug:	h.source
o.debug:	h.output
o.debug:	h.quickdraw
o.debug:	h.window
o.debug:	h.display
o.debug:	h.templates
o.debug:	h.respond
o.debug:	h.help
o.debug:	h.bin
o.debug:	h.errors
o.debug:	h.debug
o.debug:	h.cplusplus
o.debug:	h.dem
o.os: s.os
o.os: s.defs
o.step: s.step
o.step: s.defs
o.control: s.control
o.control: s.defs
o.inst:	c.inst
o.inst:	h.defs
o.inst:	h.os
o.inst:	h.asd
o.inst:	h.lookup
o.inst:	h.quickdraw
o.inst:	h.window
o.inst:	h.debug
o.inst:	h.lowlevel
o.inst:	h.control
o.inst:	h.lib
o.inst:	h.program
o.inst:	h.cplusplus
o.inst:	h.dem
o.inst:	h.inst
o.lowlevel:	c.lowlevel
o.lowlevel:	h.defs
o.lowlevel:	h.os
o.lowlevel:	h.asd
o.lowlevel:	h.errors
o.lowlevel:	h.lowlevel
o.lowlevel:	h.control
o.lowlevel:	h.ddt
o.lowlevel:	h.lib
o.lowlevel:	h.output
o.lib:	c.lib
o.lib:	h.defs
o.lib:	h.os
o.lib:	h.asd
o.lib:	h.lowlevel
o.lib:	h.control
o.lib:	h.output
o.lib:	h.errors
o.lib:	h.lib
o.lookup:	c.lookup
o.lookup:	h.defs
o.lookup:	h.os
o.lookup:	h.asd
o.lookup:	h.quickdraw
o.lookup:	h.window
o.lookup:	h.debug
o.lookup:	h.lowlevel
o.lookup:	h.control
o.lookup:	h.lib
o.lookup:	h.lookup
o.program:	c.program
o.program:	h.defs
o.program:	h.asd
o.program:	h.os
o.program:	h.lowlevel
o.program:	h.control
o.program:	h.lib
o.program:	h.output
o.program:	h.quickdraw
o.program:	h.window
o.program:	h.respond
o.program:	h.lookup
o.program:	h.debug
o.program:	h.errors
o.program:	h.display
o.program:	h.print
o.program:	h.cplusplus
o.program:	h.dem
o.program:	h.program
o.source:	c.source
o.source:	h.defs
o.source:	h.os
o.source:	h.asd
o.source:	h.lowlevel
o.source:	h.control
o.source:	h.lib
o.source:	h.quickdraw
o.source:	h.window
o.source:	h.respond
o.source:	h.display
o.source:	h.program
o.output:	c.output
o.output:	h.defs
o.output:	h.os
o.output:	h.asd
o.output:	h.lowlevel
o.output:	h.control
o.output:	h.lib
o.output:	h.quickdraw
o.output:	h.window
o.output:	h.output
o.window:	c.window
o.window:	h.defs
o.window:	h.os
o.window:	h.asd
o.window:	h.lowlevel
o.window:	h.control
o.window:	h.lib
o.window:	h.quickdraw
o.window:	h.bin
o.window:	h.ddt
o.window:	h.source
o.window:	h.window
o.templates:	c.templates
o.templates:	h.defs
o.templates:	h.quickdraw
o.templates:	h.window
o.templates:	h.os
o.templates:	h.asd
o.templates:	h.lowlevel
o.templates:	h.control
o.templates:	h.lib
o.templates:	h.ddt
o.templates:	h.bin
o.templates:	h.templates
o.quickdraw:	c.quickdraw
o.quickdraw:	h.defs
o.quickdraw:	h.os
o.quickdraw:	h.asd
o.quickdraw:	h.lowlevel
o.quickdraw:	h.control
o.quickdraw:	h.lib
o.quickdraw:	h.bin
o.quickdraw:	h.ddt
o.quickdraw:	h.quickdraw
o.respond:	c.respond
o.respond:	h.defs
o.respond:	h.os
o.respond:	h.quickdraw
o.respond:	h.asd
o.respond:	h.lowlevel
o.respond:	h.control
o.respond:	h.lib
o.respond:	h.window
o.respond:	h.debug
o.respond:	h.output
o.respond:	h.templates
o.respond:	h.ddt
o.respond:	h.respond
o.respond:	h.program
o.respond:	h.evaluate
o.respond:	h.errors
o.respond:	h.source
o.respond:	h.print
o.respond:	h.help
o.respond:	h.cplusplus
o.respond:	h.dem
o.respond:	h.display
o.respond:	h.readexpr
o.libs: s.libs
o.libs: s.defs
o.display:	c.display
o.display:	h.defs
o.display:	h.os
o.display:	h.ddt
o.display:	h.asd
o.display:	h.lowlevel
o.display:	h.control
o.display:	h.program
o.display:	h.source
o.display:	h.lookup
o.display:	h.inst
o.display:	h.quickdraw
o.display:	h.window
o.display:	h.debug
o.display:	h.respond
o.display:	h.lib
o.display:	h.errors
o.display:	h.cplusplus
o.display:	h.dem
o.display:	h.display
o.display:	h.output
o.evaluate:	c.evaluate
o.evaluate:	h.defs
o.evaluate:	h.asd
o.evaluate:	h.lookup
o.evaluate:	h.quickdraw
o.evaluate:	h.window
o.evaluate:	h.respond
o.evaluate:	h.os
o.evaluate:	h.lowlevel
o.evaluate:	h.control
o.evaluate:	h.lib
o.evaluate:	h.errors
o.evaluate:	h.program
o.evaluate:	h.setjmp
o.evaluate:	h.print
o.evaluate:	h.evaluate
o.evaluate:	h.output
o.errors:	c.errors
o.errors:	h.defs
o.errors:	h.os
o.errors:	h.asd
o.errors:	h.lowlevel
o.errors:	h.control
o.errors:	h.lib
o.errors:	h.errors
o.print:	c.print
o.print:	h.defs
o.print:	h.asd
o.print:	h.os
o.print:	h.lowlevel
o.print:	h.control
o.print:	h.evaluate
o.print:	h.output
o.print:	h.lib
o.print:	h.quickdraw
o.print:	h.window
o.print:	h.respond
o.print:	h.lookup
o.print:	h.debug
o.print:	h.program
o.print:	h.errors
o.print:	h.ddt
o.print:	h.templates
o.print:	h.cplusplus
o.print:	h.dem
o.print:	h.print
o.help:	c.help
o.help:	h.defs
o.help:	h.ddt
o.help:	h.help
o.readexpr:	c.readexpr
o.readexpr:	h.defs
o.readexpr:	h.os
o.readexpr:	h.asd
o.readexpr:	h.lowlevel
o.readexpr:	h.control
o.readexpr:	h.evaluate
o.readexpr:	h.lib
o.readexpr:	h.lookup
o.readexpr:	h.program
o.readexpr:	h.errors
o.readexpr:	h.setjmp
o.readexpr:	h.cplusplus
o.readexpr:	h.dem
o.readexpr:	h.readexpr
o.setjmp: s.setjmp
o.setjmp: s.defs
o.core:	c.core
o.core:	h.defs
o.core:	h.os
o.core:	h.asd
o.core:	h.lowlevel
o.core:	h.control
o.core:	h.lib
o.core:	h.core
o.dem:	c.dem
o.dem:	h.asd
o.dem:	h.lowlevel
o.dem:	h.os
o.dem:	h.control
o.dem:	h.lib
o.dem:	h.dem
o.cplusplus:	c.cplusplus
o.cplusplus:	h.asd
o.cplusplus:	h.lowlevel
o.cplusplus:	h.os
o.cplusplus:	h.control
o.cplusplus:	h.lib
o.cplusplus:	h.lookup
o.cplusplus:	h.cplusplus
o.cplusplus:	h.dem
