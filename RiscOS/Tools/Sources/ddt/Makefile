# Makefile for DDT
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 18-May-98  KJB          Created.
# 19-Nov-99  ADH          For TML debug, build the alldebug target and
#                         use 'Printf()' from C source in #ifdef TRACE.
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

CCFLAGS  = -c -depend !Depend -ffa -APCS 3/fpe3 ${INCLUDES} ${DFLAGS} ${THROWBACK}
CCDFLAGS = ${CCFLAGS} -DTRACE
ASBFLAGS = -depend !Depend -CPU ARM7M -Stamp -quit ${THROWBACK}
ASFLAGS  = ${ASBFLAGS} -PD "TRACE SETL {FALSE}"
ASDFLAGS = ${ASBFLAGS} -PD "TRACE SETL {TRUE}"
CPFLAGS  = ~cfr~v
LDFLAGS  = -rmf -s linktab
WFLAGS   = ~CFR~V

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
INCLUDES  = -IC:,TCPIPLibs:
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

OBJSD= \
  od.ddt \
  od.debug \
  od.os \
  od.step \
  od.control \
  od.inst \
  od.lowlevel \
  od.lib \
  od.lookup \
  od.program \
  od.source \
  od.output \
  od.window \
  od.templates \
  od.quickdraw \
  od.respond \
  od.libs \
  od.display \
  od.evaluate \
  od.errors \
  od.print \
  od.help \
  o.palette \
  o.template \
  od.readexpr \
  od.setjmp \
  od.core \
  od.dem \
  od.cplusplus \
  od.svcprint

LIBS =

LIBSD = ${LIBS} TCPIPLibs:o.inetlib

#
# Rule patterns
#
.SUFFIXES:
.SUFFIXES: .o .od .s .c .h .cmhg

.c.o:;     ${CC} ${CCFLAGS}  -o $@ $<
.c.od:;    ${CC} ${CCDFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS}  -o $@ $<
.s.od:;    ${AS} ${ASDFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS}  -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS}  -d $@ $<

#
# Special rules
#
asmdefs: asmdefs.c
        $(CC) $(DFLAGS) -o asmdefs asmdefs.c

s.defs: asmdefs
        asmdefs $@

palette.o: !palette
        binaof !palette o.palette Palette$$data palette -D -RW

template.o: templates
        binaof templates o.template Template$$data template -D -RW

o.control: s.defs
o.ddt:     s.defs
o.libs:    s.defs
o.os:      s.defs
o.setjmp:  s.defs
o.step:    s.defs

#
# build the module:
#
all:		${TARGET}
alldebug:	${TARGET}d

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o  ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${RM} asmdefs
        ${RM} s.defs
        ${RM} linktab
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENT}d
        @echo ${COMPONENT}: cleaned

export_hdrs:
	@echo ${COMPONENT}: export complete (hdrs)

export_libs:
	@echo ${COMPONENT}: export complete (libs)

o.local_dirs:
	${MKDIR} o
	${MKDIR} od
	Create o.local_dirs
	Stamp o.local_dirs

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} o.local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQUEEZE} ${COMPONENT}

${TARGET}d: ${OBJSD} ${LIBSD} o.local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT}d ${OBJSD} ${LIBSD}

# Dynamic dependencies:
