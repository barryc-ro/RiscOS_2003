# Makefile for HTMLLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 12-Dec-96  KJB          Created.
# 08-Jul-97  BAL          Added build suitable for modules.
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

CCFLAGS = -c -depend !Depend -ffah -throwback ${INCLUDES} ${DFLAGS}
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

#
# Include files
#
INCLUDES  = -IC:
DFLAGS    = -DLIBRARY -DNDEBUG -DNEWSPACING -DTonyTables

#
# Program specific options:
#
COMPONENT  = HTMLLib
COMPONENTZ = HTMLLibzm
SOURCE     = ...
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.display\
 o.malloc\
 o.rules\
 o.veneers\
 o.new_stream\
 o.anchor_inf\
 o.HTTPSuport\
 o.font\
 o.HTMLTables

OBJSZ =\
 z.display\
 z.malloc\
 z.rules\
 z.veneers\
 z.new_stream\
 z.anchor_inf\
 z.HTTPSuport\
 z.font\
 z.HTMLTables

#
# Rule patterns
#
.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${COMPONENT} ${COMPONENTZ}

#
# build a test program:
#
html_riscos2: ${COMPONENT} o.main o.svcprint o.irqs ${CLIB}
	${LD} -aif -o html_riscos2 ${COMPONENT} o.main o.svcprint o.irqs ${CLIB}

#
# RISC OS ROM build rules:
#
rom: ${TARGET} ${TARGETZ}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGETZM}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} z.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} html_riscos2
        @echo ${COMPONENT}: cleaned

export_hdrs: h.html2_ext h.api h.struct h.tags dirs
	Copy h.html2_ext    HTMLLib:h.html2_ext      ~CFLNR~V
	Copy h.api          HTMLLib:h.HTMLLib        ~CFLNR~V
	Copy h.struct       HTMLLib:h.struct         ~CFLNR~V
	Copy h.tags         HTMLLib:h.tags           ~CFLNR~V
	Copy h.tablestruc   HTMLLib:h.tablestruc     ~CFLNR~V
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	Copy ${TARGET}      HTMLLib:o.${TARGET}      ~CFLNR~V
	Copy ${TARGETZ}     HTMLLib:o.${TARGETZ}     ~CFLNR~V
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z
	
dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.HTMLLib
        ${MKDIR} ${LIBDIR}.HTMLLib.h
        ${MKDIR} ${LIBDIR}.HTMLLib.o

#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}
        ${CP} @.${COMPONENT} @.o.${COMPONENT} ${CPFLAGS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}
        ${CP} @.${COMPONENTZ} @.o.${COMPONENTZ} ${CPFLAGS}

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
z.HTMLTables:	c.HTMLTables
z.HTMLTables:	h.HTMLTables
z.HTMLTables:	h.html2_ext
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tags
z.HTMLTables:	h.internal
z.HTMLTables:	h.tags
z.HTMLTables:	h.struct
z.HTMLTables:	h.tablestruc
z.HTMLTables:	C:h.ctype
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tablestruc
z.display:	c.display
z.display:	h.html2_ext
z.display:	h.tablestruc
z.display:	h.tags
z.display:	h.struct
z.display:	C:h.string
z.display:	h.internal
z.display:	h.tags
z.display:	h.struct
z.display:	h.tablestruc
z.display:	C:h.ctype
z.display:	C:h.stdio
z.malloc:	c.malloc
z.malloc:	C:h.swis
z.malloc:	C:h.kernel
z.malloc:	h.struct
z.malloc:	h.html2_ext
z.malloc:	h.tablestruc
z.malloc:	h.internal
z.malloc:	h.tags
z.malloc:	h.struct
z.malloc:	h.tablestruc
z.malloc:	C:h.ctype
z.rules:	c.rules
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	h.internal
z.rules:	h.tags
z.rules:	h.struct
z.rules:	h.tablestruc
z.rules:	C:h.ctype
z.rules:	h.html2
z.rules:	h.tags
z.rules:	h.rules
z.rules:	h.tags
z.rules:	h.html2_ext
z.rules:	h.struct
z.rules:	C:h.string
z.rules:	C:h.ctype
z.veneers:	c.veneers
z.veneers:	h.api
z.veneers:	h.html2_ext
z.veneers:	h.tablestruc
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.internal
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.tablestruc
z.veneers:	C:h.ctype
z.veneers:	C:h.flex
z.veneers:	C:h.stdio
z.veneers:	C:h.kernel
z.veneers:	h.HTMLTables
z.veneers:	h.html2_ext
z.veneers:	h.tags
z.veneers:	h.internal
z.veneers:	h.tablestruc
z.new_stream:	c.new_stream
z.new_stream:	h.html2_ext
z.new_stream:	h.tablestruc
z.new_stream:	h.tags
z.new_stream:	h.struct
z.new_stream:	C:h.string
z.new_stream:	h.internal
z.new_stream:	h.tags
z.new_stream:	h.struct
z.new_stream:	h.tablestruc
z.new_stream:	h.HTMLTables
z.new_stream:	h.html2_ext
z.new_stream:	h.tags
z.new_stream:	h.internal
z.new_stream:	h.tablestruc
z.new_stream:	h.api
z.new_stream:	h.html2_ext
z.new_stream:	h.tags
z.new_stream:	h.struct
z.anchor_inf:	c.anchor_inf
z.anchor_inf:	C:h.stdio
z.anchor_inf:	C:h.stdlib
z.anchor_inf:	C:h.limits
z.anchor_inf:	h.html2_ext
z.anchor_inf:	h.tablestruc
z.anchor_inf:	h.tags
z.anchor_inf:	h.struct
z.anchor_inf:	C:h.string
z.anchor_inf:	h.internal
z.anchor_inf:	h.tags
z.anchor_inf:	h.struct
z.anchor_inf:	h.tablestruc
z.anchor_inf:	C:h.ctype
z.anchor_inf:	h.HTMLTables
z.anchor_inf:	h.html2_ext
z.anchor_inf:	h.tags
z.anchor_inf:	h.internal
z.anchor_inf:	h.tablestruc
z.HTTPSuport:	c.HTTPSuport
z.HTTPSuport:	h.html2_ext
z.HTTPSuport:	h.tablestruc
z.HTTPSuport:	h.tags
z.HTTPSuport:	h.struct
z.HTTPSuport:	C:h.string
z.HTTPSuport:	h.internal
z.HTTPSuport:	h.tags
z.HTTPSuport:	h.struct
z.HTTPSuport:	h.tablestruc
z.HTTPSuport:	C:h.ctype
z.HTTPSuport:	C:h.stdio
z.HTTPSuport:	h.HTTPSuport
z.font:	c.font
z.font:	h.html2_ext
z.font:	h.tablestruc
z.font:	h.struct
z.font:	h.tags
z.font:	h.internal
z.font:	h.tags
z.font:	h.struct
z.font:	h.tablestruc
z.font:	C:h.ctype
o.display:	c.display
o.display:	h.html2_ext
o.display:	h.tablestruc
o.display:	h.tags
o.display:	h.struct
o.display:	C:h.string
o.display:	h.internal
o.display:	h.tags
o.display:	h.struct
o.display:	h.tablestruc
o.display:	C:h.ctype
o.display:	C:h.stdio
o.malloc:	c.malloc
o.malloc:	C:h.swis
o.malloc:	C:h.kernel
o.malloc:	h.struct
o.malloc:	h.html2_ext
o.malloc:	h.tablestruc
o.malloc:	h.internal
o.malloc:	h.tags
o.malloc:	h.struct
o.malloc:	h.tablestruc
o.malloc:	C:h.ctype
o.rules:	c.rules
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	h.internal
o.rules:	h.tags
o.rules:	h.struct
o.rules:	h.tablestruc
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
o.veneers:	h.tablestruc
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.internal
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.tablestruc
o.veneers:	C:h.ctype
o.veneers:	C:h.flex
o.veneers:	C:h.stdio
o.veneers:	C:h.kernel
o.veneers:	h.HTMLTables
o.veneers:	h.html2_ext
o.veneers:	h.tags
o.veneers:	h.internal
o.veneers:	h.tablestruc
o.new_stream:	c.new_stream
o.new_stream:	h.html2_ext
o.new_stream:	h.tablestruc
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	C:h.string
o.new_stream:	h.internal
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	h.tablestruc
o.new_stream:	h.HTMLTables
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.internal
o.new_stream:	h.tablestruc
o.new_stream:	h.api
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.struct
o.anchor_inf:	c.anchor_inf
o.anchor_inf:	C:h.stdio
o.anchor_inf:	C:h.stdlib
o.anchor_inf:	C:h.limits
o.anchor_inf:	h.html2_ext
o.anchor_inf:	h.tablestruc
o.anchor_inf:	h.tags
o.anchor_inf:	h.struct
o.anchor_inf:	C:h.string
o.anchor_inf:	h.internal
o.anchor_inf:	h.tags
o.anchor_inf:	h.struct
o.anchor_inf:	h.tablestruc
o.anchor_inf:	C:h.ctype
o.anchor_inf:	h.HTMLTables
o.anchor_inf:	h.html2_ext
o.anchor_inf:	h.tags
o.anchor_inf:	h.internal
o.anchor_inf:	h.tablestruc
o.HTTPSuport:	c.HTTPSuport
o.HTTPSuport:	h.html2_ext
o.HTTPSuport:	h.tablestruc
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	C:h.string
o.HTTPSuport:	h.internal
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	h.tablestruc
o.HTTPSuport:	C:h.ctype
o.HTTPSuport:	C:h.stdio
o.HTTPSuport:	h.HTTPSuport
o.font:	c.font
o.font:	h.html2_ext
o.font:	h.tablestruc
o.font:	h.struct
o.font:	h.tags
o.font:	h.internal
o.font:	h.tags
o.font:	h.struct
o.font:	h.tablestruc
o.font:	C:h.ctype
o.HTMLTables:	c.HTMLTables
o.HTMLTables:	h.HTMLTables
o.HTMLTables:	h.html2_ext
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tags
o.HTMLTables:	h.internal
o.HTMLTables:	h.tags
o.HTMLTables:	h.struct
o.HTMLTables:	h.tablestruc
o.HTMLTables:	C:h.ctype
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tablestruc
