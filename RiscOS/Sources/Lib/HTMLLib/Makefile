# Makefile for HTMLLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 12-Dec-96  KJB          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>

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
WIPE    = wipe

CCFLAGS = -c -depend !Depend -fah -throwback ${INCLUDES} ${DFLAGS}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c -o
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
INCLUDES = -IC:
DFLAGS    = -DLIBRARY -DNDEBUG -DNEWSPACING

#
# Program specific options:
#
COMPONENT = HTMLLib
SOURCE    = ...
TARGET    = ${COMPONENT}
EXPORTS   = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.display\
 o.malloc\
 o.rules\
 o.veneers\
 o.new_stream\
 o.anchor_inf\
 o.HTTPSuport\
 o.font
 
#
# Rule patterns
#
.SUFFIXES: .o .s .h .cmhg .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all: ${COMPONENT}

#
# build a test program:
#
html_riscos2: ${COMPONENT} o.main o.svcprint o.irqs ${CLIB}
	${LD} -aif -o html_riscos2 ${COMPONENT} o.main o.svcprint o.irqs ${CLIB}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} html_riscos2
        @echo ${COMPONENT}: cleaned

export_hdrs: h.html2_ext h.api h.struct h.tags dirs
	Copy h.html2_ext    HTMLLib:h.html2_ext      ~CFLNR~V
	Copy h.api          HTMLLib:h.HTMLLib        ~CFLNR~V
	Copy h.struct       HTMLLib:h.struct         ~CFLNR~V
	Copy h.tags         HTMLLib:h.tags           ~CFLNR~V
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} dirs
	Copy ${TARGET}      HTMLLib:o.${TARGET}      ~CFLNR~V
        @echo ${COMPONENT}: export complete (libs)

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.HTMLLib
        ${MKDIR} ${LIBDIR}.HTMLLib.h
        ${MKDIR} ${LIBDIR}.HTMLLib.o
        
#
# Final link
#
${COMPONENT}: ${OBJS}
        ${LB} ${LBFLAGS} ${COMPONENT} ${OBJS}
        ${CP} @.${COMPONENT} @.o.${COMPONENT} ${CPFLAGS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.irqs: s.irqs
o.irqs: Hdr:ListOpts
o.irqs: Hdr:Macros
o.irqs: Hdr:System
o.irqs: Hdr:SWIs
o.irqs: Hdr:CPU.Generic26
o.irqs: Hdr:IO.GenericIO
o.irqs: Hdr:RISCOS
o.display:	c.display
o.display:	h.html2_ext
o.display:	h.tags
o.display:	h.struct
o.display:	C:h.string
o.display:	h.internal
o.display:	h.tags
o.display:	h.struct
o.display:	C:h.ctype
o.display:	C:h.stdio
o.malloc:	c.malloc
o.malloc:	C:h.swis
o.malloc:	C:h.kernel
o.malloc:	h.struct
o.malloc:	h.html2_ext
o.malloc:	h.internal
o.malloc:	h.tags
o.malloc:	h.struct
o.malloc:	C:h.ctype
o.rules:	c.rules
o.rules:	h.html2_ext
o.rules:	h.internal
o.rules:	h.tags
o.rules:	h.struct
o.rules:	C:h.ctype
o.rules:	h.html2
o.rules:	h.tags
o.rules:	h.rules
o.rules:	h.tags
o.rules:	h.html2_ext
o.rules:	h.struct
o.rules:	C:h.string
o.rules:	C:h.ctype
o.veneers:	c.veneers
o.veneers:	h.api
o.veneers:	h.html2_ext
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.internal
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	C:h.ctype
o.veneers:	C:h.flex
o.veneers:	C:h.stdio
o.veneers:	C:h.kernel
o.new_stream:	c.new_stream
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	C:h.string
o.new_stream:	h.internal
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	C:h.ctype
o.new_stream:	h.api
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.struct
o.anchor_inf:	c.anchor_inf
o.anchor_inf:	C:h.stdio
o.anchor_inf:	C:h.stdlib
o.anchor_inf:	C:h.limits
o.anchor_inf:	h.html2_ext
o.anchor_inf:	h.tags
o.anchor_inf:	h.struct
o.anchor_inf:	C:h.string
o.anchor_inf:	h.internal
o.anchor_inf:	h.tags
o.anchor_inf:	h.struct
o.anchor_inf:	C:h.ctype
o.HTTPSuport:	c.HTTPSuport
o.HTTPSuport:	h.html2_ext
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	C:h.string
o.HTTPSuport:	h.internal
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	C:h.ctype
o.HTTPSuport:	C:h.stdio
o.HTTPSuport:	h.HTTPSuport
o.font:	c.font
o.font:	h.html2_ext
o.font:	h.struct
o.font:	h.tags
o.font:	h.internal
o.font:	h.tags
o.font:	h.struct
o.font:	C:h.ctype
