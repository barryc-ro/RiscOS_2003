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
 o.HTTPSuport\
 o.font\
 o.HTMLTables\
 o.attributes\
 o.attrvenrs\
 o.object\
 o.ol\
 o.URLModule
 
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
 z.object\
 z.ol\
 z.URLModule


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
all:	${COMPONENT} #${COMPONENTZ}

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
	Copy h.html2_ext                HTMLLib:h.html2_ext                ~CFLNR~V
	Copy h.api                      HTMLLib:h.HTMLLib                  ~CFLNR~V
	Copy h.struct                   HTMLLib:h.struct                   ~CFLNR~V
	Copy h.tags                     HTMLLib:h.tags                     ~CFLNR~V
	Copy h.tablestruc               HTMLLib:h.tablestruc               ~CFLNR~V
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
o.display:	c.display
o.display:	h.html2_ext
o.display:	h.tablestruc
o.display:	h.tags
o.display:	h.struct
o.display:	h.html2_ext
o.display:	C:h.string
o.display:	h.internal
o.display:	C:h.kernel
o.display:	h.tags
o.display:	h.struct
o.display:	h.tablestruc
o.display:	C:Unicode.h.encoding
o.display:	C:Unicode.h.iso10646
o.display:	C:h.ctype
o.display:	C:h.stdio
o.display:	C:Unicode.h.iso10646
o.malloc:	c.malloc
o.malloc:	C:h.swis
o.malloc:	C:h.kernel
o.malloc:	h.struct
o.malloc:	h.html2_ext
o.malloc:	h.tablestruc
o.malloc:	h.html2_ext
o.malloc:	h.internal
o.malloc:	C:h.kernel
o.malloc:	h.tags
o.malloc:	h.struct
o.malloc:	h.tablestruc
o.malloc:	C:Unicode.h.encoding
o.malloc:	C:Unicode.h.iso10646
o.malloc:	C:h.ctype
o.veneers:	c.veneers
o.veneers:	h.api
o.veneers:	h.html2_ext
o.veneers:	h.tablestruc
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.html2_ext
o.veneers:	h.internal
o.veneers:	C:h.kernel
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.tablestruc
o.veneers:	C:Unicode.h.encoding
o.veneers:	C:Unicode.h.iso10646
o.veneers:	C:h.ctype
o.veneers:	tbox:h.flex
o.veneers:	C:h.stdio
o.veneers:	C:h.swis
o.veneers:	h.HTMLTables
o.veneers:	h.html2_ext
o.veneers:	h.tags
o.veneers:	h.internal
o.veneers:	h.tablestruc
o.veneers:	C:Unicode.h.encoding
o.HTTPSuport:	c.HTTPSuport
o.HTTPSuport:	h.html2_ext
o.HTTPSuport:	h.tablestruc
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	h.html2_ext
o.HTTPSuport:	C:h.string
o.HTTPSuport:	h.internal
o.HTTPSuport:	C:h.kernel
o.HTTPSuport:	h.tags
o.HTTPSuport:	h.struct
o.HTTPSuport:	h.tablestruc
o.HTTPSuport:	C:Unicode.h.encoding
o.HTTPSuport:	C:Unicode.h.iso10646
o.HTTPSuport:	C:h.ctype
o.HTTPSuport:	C:h.stdio
o.HTTPSuport:	C:Unicode.h.encoding
o.HTTPSuport:	h.HTTPSuport
o.font:	c.font
o.font:	h.html2_ext
o.font:	h.tablestruc
o.font:	h.struct
o.font:	h.html2_ext
o.font:	h.tags
o.font:	h.internal
o.font:	C:h.kernel
o.font:	h.tags
o.font:	h.struct
o.font:	h.tablestruc
o.font:	C:Unicode.h.encoding
o.font:	C:Unicode.h.iso10646
o.font:	C:h.ctype
o.HTMLTables:	c.HTMLTables
o.HTMLTables:	h.HTMLTables
o.HTMLTables:	h.html2_ext
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tags
o.HTMLTables:	h.internal
o.HTMLTables:	C:h.kernel
o.HTMLTables:	h.tags
o.HTMLTables:	h.struct
o.HTMLTables:	h.html2_ext
o.HTMLTables:	h.tablestruc
o.HTMLTables:	C:Unicode.h.encoding
o.HTMLTables:	C:Unicode.h.iso10646
o.HTMLTables:	C:h.ctype
o.HTMLTables:	h.tablestruc
o.HTMLTables:	h.tablestruc
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
o.attributes:	C:h.kernel
o.attributes:	h.tags
o.attributes:	h.struct
o.attributes:	h.tablestruc
o.attributes:	C:Unicode.h.encoding
o.attributes:	C:Unicode.h.iso10646
o.attributes:	C:h.ctype
o.attributes:	h.rules
o.attributes:	h.tags
o.attributes:	TCPIPLibs:h.unixlib
o.attributes:	h.elementinf
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
o.attrvenrs:	C:h.kernel
o.attrvenrs:	h.tags
o.attrvenrs:	h.struct
o.attrvenrs:	h.tablestruc
o.attrvenrs:	C:Unicode.h.encoding
o.attrvenrs:	C:Unicode.h.iso10646
o.attrvenrs:	C:h.ctype
o.attrvenrs:	h.elementinf
o.object:	c.object
o.object:	h.html2_ext
o.object:	h.tablestruc
o.object:	h.tags
o.object:	h.struct
o.object:	h.html2_ext
o.object:	C:h.string
o.object:	h.internal
o.object:	C:h.kernel
o.object:	h.tags
o.object:	h.struct
o.object:	h.tablestruc
o.object:	C:Unicode.h.encoding
o.object:	C:Unicode.h.iso10646
o.object:	h.elementinf
o.object:	h.HTMLTables
o.object:	h.html2_ext
o.object:	h.tags
o.object:	h.internal
o.object:	h.tablestruc
o.ol:	c.ol
o.ol:	h.html2_ext
o.ol:	h.tablestruc
o.ol:	h.tags
o.ol:	h.internal
o.ol:	C:h.kernel
o.ol:	h.tags
o.ol:	h.struct
o.ol:	h.html2_ext
o.ol:	h.tablestruc
o.ol:	C:Unicode.h.encoding
o.ol:	C:Unicode.h.iso10646
o.ol:	C:h.ctype
o.ol:	h.elementinf
o.URLModule:	c.URLModule
o.URLModule:	C:h.kernel
o.URLModule:	C:h.swis
o.URLModule:	h.internal
o.URLModule:	C:h.kernel
o.URLModule:	h.tags
o.URLModule:	h.struct
o.URLModule:	h.html2_ext
o.URLModule:	h.tablestruc
o.URLModule:	h.tablestruc
o.URLModule:	C:Unicode.h.encoding
o.URLModule:	C:Unicode.h.iso10646
o.URLModule:	C:h.ctype
z.display:	c.display
z.display:	h.html2_ext
z.display:	h.tablestruc
z.display:	h.tags
z.display:	h.struct
z.display:	h.html2_ext
z.display:	C:h.string
z.display:	h.internal
z.display:	C:h.kernel
z.display:	h.tags
z.display:	h.struct
z.display:	h.tablestruc
z.display:	C:Unicode.h.encoding
z.display:	C:Unicode.h.iso10646
z.display:	C:h.ctype
z.display:	C:h.stdio
z.display:	C:Unicode.h.iso10646
z.malloc:	c.malloc
z.malloc:	C:h.swis
z.malloc:	C:h.kernel
z.malloc:	h.struct
z.malloc:	h.html2_ext
z.malloc:	h.tablestruc
z.malloc:	h.html2_ext
z.malloc:	h.internal
z.malloc:	C:h.kernel
z.malloc:	h.tags
z.malloc:	h.struct
z.malloc:	h.tablestruc
z.malloc:	C:Unicode.h.encoding
z.malloc:	C:Unicode.h.iso10646
z.malloc:	C:h.ctype
z.rules:	c.rules
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	h.internal
z.rules:	C:h.kernel
z.rules:	h.tags
z.rules:	h.struct
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	C:Unicode.h.encoding
z.rules:	C:Unicode.h.iso10646
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
z.veneers:	h.html2_ext
z.veneers:	h.internal
z.veneers:	C:h.kernel
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.tablestruc
z.veneers:	C:Unicode.h.encoding
z.veneers:	C:Unicode.h.iso10646
z.veneers:	C:h.ctype
z.veneers:	tbox:h.flex
z.veneers:	C:h.stdio
z.veneers:	C:h.swis
z.veneers:	h.HTMLTables
z.veneers:	h.html2_ext
z.veneers:	h.tags
z.veneers:	h.internal
z.veneers:	h.tablestruc
z.veneers:	C:Unicode.h.encoding
z.HTTPSuport:	c.HTTPSuport
z.HTTPSuport:	h.html2_ext
z.HTTPSuport:	h.tablestruc
z.HTTPSuport:	h.tags
z.HTTPSuport:	h.struct
z.HTTPSuport:	h.html2_ext
z.HTTPSuport:	C:h.string
z.HTTPSuport:	h.internal
z.HTTPSuport:	C:h.kernel
z.HTTPSuport:	h.tags
z.HTTPSuport:	h.struct
z.HTTPSuport:	h.tablestruc
z.HTTPSuport:	C:Unicode.h.encoding
z.HTTPSuport:	C:Unicode.h.iso10646
z.HTTPSuport:	C:h.ctype
z.HTTPSuport:	C:h.stdio
z.HTTPSuport:	C:Unicode.h.encoding
z.HTTPSuport:	h.HTTPSuport
z.font:	c.font
z.font:	h.html2_ext
z.font:	h.tablestruc
z.font:	h.struct
z.font:	h.html2_ext
z.font:	h.tags
z.font:	h.internal
z.font:	C:h.kernel
z.font:	h.tags
z.font:	h.struct
z.font:	h.tablestruc
z.font:	C:Unicode.h.encoding
z.font:	C:Unicode.h.iso10646
z.font:	C:h.ctype
z.HTMLTables:	c.HTMLTables
z.HTMLTables:	h.HTMLTables
z.HTMLTables:	h.html2_ext
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tags
z.HTMLTables:	h.internal
z.HTMLTables:	C:h.kernel
z.HTMLTables:	h.tags
z.HTMLTables:	h.struct
z.HTMLTables:	h.html2_ext
z.HTMLTables:	h.tablestruc
z.HTMLTables:	C:Unicode.h.encoding
z.HTMLTables:	C:Unicode.h.iso10646
z.HTMLTables:	C:h.ctype
z.HTMLTables:	h.tablestruc
z.HTMLTables:	h.tablestruc
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
z.attributes:	C:h.kernel
z.attributes:	h.tags
z.attributes:	h.struct
z.attributes:	h.tablestruc
z.attributes:	C:Unicode.h.encoding
z.attributes:	C:Unicode.h.iso10646
z.attributes:	C:h.ctype
z.attributes:	h.rules
z.attributes:	h.tags
z.attributes:	TCPIPLibs:h.unixlib
z.attributes:	h.elementinf
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
z.attrvenrs:	C:h.kernel
z.attrvenrs:	h.tags
z.attrvenrs:	h.struct
z.attrvenrs:	h.tablestruc
z.attrvenrs:	C:Unicode.h.encoding
z.attrvenrs:	C:Unicode.h.iso10646
z.attrvenrs:	C:h.ctype
z.attrvenrs:	h.elementinf
z.object:	c.object
z.object:	h.html2_ext
z.object:	h.tablestruc
z.object:	h.tags
z.object:	h.struct
z.object:	h.html2_ext
z.object:	C:h.string
z.object:	h.internal
z.object:	C:h.kernel
z.object:	h.tags
z.object:	h.struct
z.object:	h.tablestruc
z.object:	C:Unicode.h.encoding
z.object:	C:Unicode.h.iso10646
z.object:	h.elementinf
z.object:	h.HTMLTables
z.object:	h.html2_ext
z.object:	h.tags
z.object:	h.internal
z.object:	h.tablestruc
z.ol:	c.ol
z.ol:	h.html2_ext
z.ol:	h.tablestruc
z.ol:	h.tags
z.ol:	h.internal
z.ol:	C:h.kernel
z.ol:	h.tags
z.ol:	h.struct
z.ol:	h.html2_ext
z.ol:	h.tablestruc
z.ol:	C:Unicode.h.encoding
z.ol:	C:Unicode.h.iso10646
z.ol:	C:h.ctype
z.ol:	h.elementinf
z.URLModule:	c.URLModule
z.URLModule:	C:h.kernel
z.URLModule:	C:h.swis
z.URLModule:	h.internal
z.URLModule:	C:h.kernel
z.URLModule:	h.tags
z.URLModule:	h.struct
z.URLModule:	h.html2_ext
z.URLModule:	h.tablestruc
z.URLModule:	h.tablestruc
z.URLModule:	C:Unicode.h.encoding
z.URLModule:	C:Unicode.h.iso10646
z.URLModule:	C:h.ctype
o.rules:	c.rules
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	h.internal
o.rules:	C:h.kernel
o.rules:	h.tags
o.rules:	h.struct
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	C:Unicode.h.encoding
o.rules:	C:Unicode.h.iso10646
o.rules:	C:h.ctype
o.rules:	h.html2
o.rules:	h.tags
o.rules:	h.rules
o.rules:	h.tags
o.rules:	h.html2_ext
o.rules:	h.struct
o.rules:	C:h.string
o.rules:	C:h.ctype
o.new_stream:	c.new_stream
o.new_stream:	h.html2_ext
o.new_stream:	h.tablestruc
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	h.html2_ext
o.new_stream:	C:h.string
o.new_stream:	h.internal
o.new_stream:	C:h.kernel
o.new_stream:	h.tags
o.new_stream:	h.struct
o.new_stream:	h.tablestruc
o.new_stream:	C:Unicode.h.encoding
o.new_stream:	C:Unicode.h.iso10646
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
z.new_stream:	c.new_stream
z.new_stream:	h.html2_ext
z.new_stream:	h.tablestruc
z.new_stream:	h.tags
z.new_stream:	h.struct
z.new_stream:	h.html2_ext
z.new_stream:	C:h.string
z.new_stream:	h.internal
z.new_stream:	C:h.kernel
z.new_stream:	h.tags
z.new_stream:	h.struct
z.new_stream:	h.tablestruc
z.new_stream:	C:Unicode.h.encoding
z.new_stream:	C:Unicode.h.iso10646
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
