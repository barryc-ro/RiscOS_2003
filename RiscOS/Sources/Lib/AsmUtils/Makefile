# Makefile for AsmUtils
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20-Nov-99  SNB          Created.
#

#
# Paths
#
LIBDIR = <Lib$Dir>.AsmUtils

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe

# No specific preference
OBJASMCPU =

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS}
ASFLAGS = -depend !Depend -Stamp -quit ${OBJASMCPU}
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

#
# Include files
#
INCLUDES  = -IC:
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = AsmUtils
COMPONENTZ = AsmUtilszm
TARGET     = alf.${COMPONENT}
TARGETZ    = alf.${COMPONENTZ}
SOURCE     = 
EXPORTS    = 

OBJS=   o.callbacks o.sixtyfour o.heapswis o.rminfo o.irqs

OBJSZ=  oz.callbacks oz.sixtyfour oz.heapswis oz.rminfo oz.irqs

HDRS=   h.callbacks h.sixtyfour h.heapswis h.rminfo h.irqs

LIBS = ${TARGET} ${TARGETZ}

LOCALDIRS = o._dirs

#
# Rule patterns
#
.SUFFIXES: .oz
.s.o:;     ${AS} ${ASFLAGS} -o $@ $< -PreDefine "ROM SETL {FALSE}"
.s.oz:;    ${AS} ${ASFLAGS} -o $@ $< -PreDefine "ROM SETL {TRUE}"

#
# build a the library:
#
all:    ${LIBS}

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
        ${WIPE} oz ${WFLAGS}
        ${WIPE} alf ${WFLAGS}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} export_dirs
	${CP} h.callbacks  ${LIBDIR}.h.callbacks ${CPFLAGS}
	${CP} h.sixtyfour  ${LIBDIR}.h.sixtyfour ${CPFLAGS}
	${CP} h.rminfo     ${LIBDIR}.h.rminfo    ${CPFLAGS}
	${CP} h.heapswis   ${LIBDIR}.h.heapswis  ${CPFLAGS}
	${CP} h.irqs       ${LIBDIR}.h.irqs      ${CPFLAGS}
	${CP} VersionNum   ${LIBDIR}.VersionNum  ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} export_dirs
	${CP} alf ${LIBDIR}.o ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

${LOCALDIRS}:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} alf
        ${TOUCH} $@

export_dirs:
        ${MKDIR} ${LIBDIR}.h

#
# Final link
#
${TARGET}: ${OBJS} ${LOCALDIRS}
        ${LB} ${LBFLAGS} -o $@ ${OBJS}

${TARGETZ}: ${OBJSZ} ${LOCALDIRS}
	${LB} ${LBFLAGS} -o $@ ${OBJSZ}

# Dynamic dependencies:
