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
TOUCH   = create
WIPE    = -wipe

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
INCLUDES  = -Itbox:,C:,TCPIPLibs:
DFLAGS    = -DLIBRARY -DNDEBUG -DNEWSPACING -DTonyTables -DLINEONE_HACK

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
 o.HTTPSuport\
 o.font\
 o.HTMLTables\
 o.attributes\
 o.attrvenrs\
 o.utf8
 
OBJSZ =\
 z.display\
 z.malloc\
 z.rules\
 z.veneers\
 z.new_stream\
 z.HTTPSuport\
 z.font\
 z.HTMLTables\
 z.attributes\
 z.attrvenrs\
 z.utf8

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
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        ${RM} html_riscos2
        ${RM} local_dirs
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
	${TOUCH} local_dirs
	
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

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.utf8:	c.utf8
o.utf8:	h.utf8
o.utf8:	h.iso10646
z.utf8:	c.utf8
z.utf8:	h.utf8
z.utf8:	h.iso10646
z.attributes:	c.attributes
z.attributes:	C:h.stdio
z.attributes:	C:h.stdlib
z.attributes:	C:h.limits
z.attributes:	h.html2_ext
z.attributes:	h.tablestruc
z.attributes:	h.tags
z.attributes:	h.struct
z.attributes:	h.html2_ext
z.attributes:	C:h.string
z.attributes:	h.internal
z.attributes:	h.tags
z.attributes:	h.struct
z.attributes:	h.tablestruc
z.attributes:	C:h.ctype
z.attributes:	h.rules
z.attributes:	h.tags
z.attributes:	TCPIPLibs:h.unixlib
z.attributes:	h.HTMLTables
z.attributes:	h.html2_ext
z.attributes:	h.tags
z.attributes:	h.internal
z.attributes:	h.tablestruc
z.attributes:	h.colourlist
z.attrvenrs:	c.attrvenrs
z.attrvenrs:	h.api
z.attrvenrs:	h.html2_ext
z.attrvenrs:	h.tablestruc
z.attrvenrs:	h.tags
z.attrvenrs:	h.struct
z.attrvenrs:	h.html2_ext
z.attrvenrs:	h.internal
z.attrvenrs:	h.tags
z.attrvenrs:	h.struct
z.attrvenrs:	h.tablestruc
z.attrvenrs:	C:h.ctype
z.attrvenrs:	h.elementinf
o.display:	c.display
o.display:	h.html2_ext
o.display:	h.tablestruc
o.display:	h.tags
o.display:	h.struct
o.display:	h.html2_ext
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
o.malloc:	h.html2_ext
o.malloc:	h.internal
o.malloc:	h.tags
o.malloc:	h.struct
o.malloc:	h.tablestruc
o.malloc:	C:h.ctype
o.veneers:	c.veneers
o.veneers:	h.api
o.veneers:	h.html2_ext
o.veneers:	h.tablestruc
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.html2_ext
o.veneers:	h.internal
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.tablestruc
o.veneers:	C:h.ctype
o.veneers:	tbox:h.flex
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
o.new_stream:	h.html2_ext
o.new_stream:	C:h.string
o.new_stream:	h.internal
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	h.tablestruc
o.new_stream:	h.elementinf
o.new_stream:	h.HTMLTables
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.internal
o.new_stream:	h.tablestruc
o.new_stream:	h.api
o.new_stream:	h.html2_ext
o.new_stream:	h.tags
o.new_stream:	h.struct
o.HTTPSuport:	c.HTTPSuport
o.HTTPSuport:	h.html2_ext
o.HTTPSuport:	h.tablestruc
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	h.html2_ext
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
o.font:	h.html2_ext
o.font:	h.tags
o.font:	h.internal
o.font:	h.tags
o.font:	h.struct
o.font:	h.tablestruc
o.font:	C:h.ctype
o.attributes:	c.attributes
o.attributes:	C:h.stdio
o.attributes:	C:h.stdlib
o.attributes:	C:h.limits
o.attributes:	h.html2_ext
o.attributes:	h.tablestruc
o.attributes:	h.tags
o.attributes:	h.struct
o.attributes:	h.html2_ext
o.attributes:	C:h.string
o.attributes:	h.internal
o.attributes:	h.tags
o.attributes:	h.struct
o.attributes:	h.tablestruc
o.attributes:	C:h.ctype
o.attributes:	h.rules
o.attributes:	h.tags
o.attributes:	TCPIPLibs:h.unixlib
o.attributes:	h.HTMLTables
o.attributes:	h.html2_ext
o.attributes:	h.tags
o.attributes:	h.internal
o.attributes:	h.tablestruc
o.attributes:	h.colourlist
o.attrvenrs:	c.attrvenrs
o.attrvenrs:	h.api
o.attrvenrs:	h.html2_ext
o.attrvenrs:	h.tablestruc
o.attrvenrs:	h.tags
o.attrvenrs:	h.struct
o.attrvenrs:	h.html2_ext
o.attrvenrs:	h.internal
o.attrvenrs:	h.tags
o.attrvenrs:	h.struct
o.attrvenrs:	h.tablestruc
o.attrvenrs:	C:h.ctype
o.attrvenrs:	h.elementinf
z.display:	c.display
z.display:	h.html2_ext
z.display:	h.tablestruc
z.display:	h.tags
z.display:	h.struct
z.display:	h.html2_ext
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
z.malloc:	h.html2_ext
z.malloc:	h.internal
z.malloc:	h.tags
z.malloc:	h.struct
z.malloc:	h.tablestruc
z.malloc:	C:h.ctype
z.veneers:	c.veneers
z.veneers:	h.api
z.veneers:	h.html2_ext
z.veneers:	h.tablestruc
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.html2_ext
z.veneers:	h.internal
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.tablestruc
z.veneers:	C:h.ctype
z.veneers:	tbox:h.flex
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
z.new_stream:	h.html2_ext
z.new_stream:	C:h.string
z.new_stream:	h.internal
z.new_stream:	h.tags
z.new_stream:	h.struct
z.new_stream:	h.tablestruc
z.new_stream:	h.elementinf
z.new_stream:	h.HTMLTables
z.new_stream:	h.html2_ext
z.new_stream:	h.tags
z.new_stream:	h.internal
z.new_stream:	h.tablestruc
z.new_stream:	h.api
z.new_stream:	h.html2_ext
z.new_stream:	h.tags
z.new_stream:	h.struct
z.HTTPSuport:	c.HTTPSuport
z.HTTPSuport:	h.html2_ext
z.HTTPSuport:	h.tablestruc
z.HTTPSuport:	h.tags
z.HTTPSuport:	h.struct
z.HTTPSuport:	h.html2_ext
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
z.font:	h.html2_ext
z.font:	h.tags
z.font:	h.internal
z.font:	h.tags
z.font:	h.struct
z.font:	h.tablestruc
z.font:	C:h.ctype
o.rules:	c.rules
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	h.internal
o.rules:	h.tags
o.rules:	h.struct
o.rules:	h.html2_ext
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
z.rules:	c.rules
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	h.internal
z.rules:	h.tags
z.rules:	h.struct
z.rules:	h.html2_ext
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
o.HTMLTables:	c.HTMLTables
o.HTMLTables:	h.HTMLTables
o.HTMLTables:	h.html2_ext
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tags
o.HTMLTables:	h.internal
o.HTMLTables:	h.tags
o.HTMLTables:	h.struct
o.HTMLTables:	h.html2_ext
o.HTMLTables:	h.tablestruc
o.HTMLTables:	C:h.ctype
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tablestruc
z.HTMLTables:	c.HTMLTables
z.HTMLTables:	h.HTMLTables
z.HTMLTables:	h.html2_ext
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tags
z.HTMLTables:	h.internal
z.HTMLTables:	h.tags
z.HTMLTables:	h.struct
z.HTMLTables:	h.html2_ext
z.HTMLTables:	h.tablestruc
z.HTMLTables:	C:h.ctype
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tablestruc
