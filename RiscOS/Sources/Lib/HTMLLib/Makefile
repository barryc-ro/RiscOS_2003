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
INCLUDES  = -Iencodings,tbox:,C:,TCPIPLibs:
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
 encodings.o.utf8\
 encodings.o.encoding\
 encodings.o.iso2022\
 encodings.o.iso8859-1\
 encodings.o.iso8859-2\
 encodings.o.iso8859-3\
 encodings.o.iso8859-4\
 encodings.o.iso8859-5\
 encodings.o.iso8859-7\
 encodings.o.iso8859-8\
 encodings.o.iso8859-9\
 encodings.o.iso8859-10\
 encodings.o.iso8859-13\
 encodings.o.sami\
 encodings.o.koi8r\
 encodings.o.welsh\
 encodings.o.microsoft\
 encodings.o.apple\
 encodings.o.shiftjis\
 encodings.o.bigfive\
 encodings.o.euc-jp\
 encodings.o.euc-kr\
 encodings.o.euc-cn\
 encodings.o.enc_utf8\
 encodings.o.enc_ascii\
 encodings.o.enc_utf16\
 encodings.o.enc_ucs4
 
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
 encodings.z.utf8\
 encodings.z.encoding\
 encodings.z.iso2022\
 encodings.z.iso8859-1\
 encodings.z.iso8859-2\
 encodings.z.iso8859-3\
 encodings.z.iso8859-4\
 encodings.z.iso8859-5\
 encodings.z.iso8859-7\
 encodings.z.iso8859-8\
 encodings.z.iso8859-9\
 encodings.z.iso8859-10\
 encodings.z.iso8859-13\
 encodings.z.sami\
 encodings.z.koi8r\
 encodings.z.welsh\
 encodings.z.microsoft\
 encodings.z.apple\
 encodings.z.shiftjis\
 encodings.z.bigfive\
 encodings.z.euc-jp\
 encodings.z.euc-kr\
 encodings.z.euc-cn\
 encodings.z.enc_utf8\
 encodings.z.enc_ascii\
 encodings.z.enc_utf16\
 encodings.z.enc_ucs4


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
        ${WIPE} encodings.o ${WFLAGS}
        ${WIPE} encodings.z ${WFLAGS}
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
	Copy encodings.h.iso10646       HTMLLib:h.iso10646                 ~CFLNR~V
	Copy encodings.h.utf8           HTMLLib:h.utf8                     ~CFLNR~V
	Every.Every2 "Resources.Encodings...* -x ..CVS ..CVS.*" Copy %0 HTMLLib:%0 ~CFLN~R~V
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	Copy ${TARGET}      HTMLLib:o.${TARGET}      ~CFLNR~V
	Copy ${TARGETZ}     HTMLLib:o.${TARGETZ}     ~CFLNR~V
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z
	${MKDIR} encodings.o
	${MKDIR} encodings.z
	${TOUCH} local_dirs
	
dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.HTMLLib
        ${MKDIR} ${LIBDIR}.HTMLLib.h
        ${MKDIR} ${LIBDIR}.HTMLLib.o
        ${MKDIR} ${LIBDIR}.HTMLLib.Resources
        ${MKDIR} ${LIBDIR}.HTMLLib.Resources.Encodings

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
o.display:	h.tags
o.display:	h.struct
o.display:	h.tablestruc
o.display:	encodings.h.encoding
o.display:	encodings.h.iso10646
o.display:	C:h.ctype
o.display:	C:h.stdio
o.display:	encodings.h.iso10646
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
o.malloc:	encodings.h.encoding
o.malloc:	encodings.h.iso10646
o.malloc:	C:h.ctype
o.rules:	c.rules
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	h.internal
o.rules:	h.tags
o.rules:	h.struct
o.rules:	h.html2_ext
o.rules:	h.tablestruc
o.rules:	encodings.h.encoding
o.rules:	encodings.h.iso10646
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
o.veneers:	h.html2_ext
o.veneers:	h.internal
o.veneers:	h.tags
o.veneers:	h.struct
o.veneers:	h.tablestruc
o.veneers:	encodings.h.encoding
o.veneers:	encodings.h.iso10646
o.veneers:	C:h.ctype
o.veneers:	tbox:h.flex
o.veneers:	C:h.stdio
o.veneers:	C:h.swis
o.veneers:	C:h.kernel
o.veneers:	h.HTMLTables
o.veneers:	h.html2_ext
o.veneers:	h.tags
o.veneers:	h.internal
o.veneers:	h.tablestruc
o.veneers:	encodings.h.encoding
o.veneers:	encodings.h.iso10646
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
o.new_stream:	encodings.h.encoding
o.new_stream:	encodings.h.iso10646
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
o.HTTPSuport:	encodings.h.encoding
o.HTTPSuport:	encodings.h.iso10646
o.HTTPSuport:	C:h.ctype
o.HTTPSuport:	C:h.stdio
o.HTTPSuport:	encodings.h.encoding
o.HTTPSuport:	encodings.h.iso10646
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
o.font:	encodings.h.encoding
o.font:	encodings.h.iso10646
o.font:	C:h.ctype
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
o.HTMLTables:	encodings.h.encoding
o.HTMLTables:	encodings.h.iso10646
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
o.attributes:	h.tags
o.attributes:	h.struct
o.attributes:	h.tablestruc
o.attributes:	encodings.h.encoding
o.attributes:	encodings.h.iso10646
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
o.attrvenrs:	encodings.h.encoding
o.attrvenrs:	encodings.h.iso10646
o.attrvenrs:	C:h.ctype
o.attrvenrs:	h.elementinf
encodings.o.utf8:	encodings.c.utf8
encodings.o.utf8:	encodings.h.utf8
encodings.o.utf8:	encodings.h.iso10646
encodings.o.encoding:	encodings.c.encoding
encodings.o.encoding:	C:h.swis
encodings.o.encoding:	C:h.kernel
encodings.o.encoding:	encodings.h.encoding
encodings.o.encoding:	encodings.h.iso10646
encodings.o.encoding:	encodings.h.enc_ascii
encodings.o.encoding:	encodings.h.iso8859-1
encodings.o.encoding:	encodings.h.iso8859-2
encodings.o.encoding:	encodings.h.iso8859-3
encodings.o.encoding:	encodings.h.iso8859-4
encodings.o.encoding:	encodings.h.iso8859-5
encodings.o.encoding:	encodings.h.iso8859-7
encodings.o.encoding:	encodings.h.iso8859-8
encodings.o.encoding:	encodings.h.iso8859-9
encodings.o.encoding:	encodings.h.iso8859-10
encodings.o.encoding:	encodings.h.iso8859-13
encodings.o.encoding:	encodings.h.sami
encodings.o.encoding:	encodings.h.welsh
encodings.o.encoding:	encodings.h.iso2022
encodings.o.encoding:	encodings.h.shiftjis
encodings.o.encoding:	encodings.h.bigfive
encodings.o.encoding:	encodings.h.koi8r
encodings.o.encoding:	encodings.h.microsoft
encodings.o.encoding:	encodings.h.apple
encodings.o.encoding:	encodings.h.euc-jp
encodings.o.encoding:	encodings.h.euc-kr
encodings.o.encoding:	encodings.h.euc-cn
encodings.o.encoding:	encodings.h.enc_utf8
encodings.o.encoding:	encodings.h.enc_utf16
encodings.o.encoding:	encodings.h.enc_ucs4
encodings.o.iso2022:	encodings.c.iso2022
encodings.o.iso2022:	encodings.h.encoding
encodings.o.iso2022:	encodings.h.iso10646
encodings.o.iso2022:	C:h.swis
encodings.o.iso2022:	C:h.kernel
encodings.o.iso2022:	encodings.h.iso2022
encodings.o.iso8859-1:	encodings.c.iso8859-1
encodings.o.iso8859-1:	encodings.h.encoding
encodings.o.iso8859-1:	encodings.h.iso10646
encodings.o.iso8859-1:	encodings.h.utf8
encodings.o.iso8859-1:	encodings.h.iso10646
encodings.o.iso8859-1:	encodings.h.iso2022
encodings.o.iso8859-1:	encodings.h.iso8859-1
encodings.o.iso8859-2:	encodings.c.iso8859-2
encodings.o.iso8859-2:	encodings.h.encoding
encodings.o.iso8859-2:	encodings.h.iso10646
encodings.o.iso8859-2:	encodings.h.utf8
encodings.o.iso8859-2:	encodings.h.iso10646
encodings.o.iso8859-2:	encodings.h.iso2022
encodings.o.iso8859-2:	encodings.h.iso8859-2
encodings.o.iso8859-3:	encodings.c.iso8859-3
encodings.o.iso8859-3:	encodings.h.encoding
encodings.o.iso8859-3:	encodings.h.iso10646
encodings.o.iso8859-3:	encodings.h.utf8
encodings.o.iso8859-3:	encodings.h.iso10646
encodings.o.iso8859-3:	encodings.h.iso2022
encodings.o.iso8859-3:	encodings.h.iso8859-3
encodings.o.iso8859-4:	encodings.c.iso8859-4
encodings.o.iso8859-4:	encodings.h.encoding
encodings.o.iso8859-4:	encodings.h.iso10646
encodings.o.iso8859-4:	encodings.h.utf8
encodings.o.iso8859-4:	encodings.h.iso10646
encodings.o.iso8859-4:	encodings.h.iso2022
encodings.o.iso8859-4:	encodings.h.iso8859-4
encodings.o.iso8859-5:	encodings.c.iso8859-5
encodings.o.iso8859-5:	encodings.h.encoding
encodings.o.iso8859-5:	encodings.h.iso10646
encodings.o.iso8859-5:	encodings.h.utf8
encodings.o.iso8859-5:	encodings.h.iso10646
encodings.o.iso8859-5:	encodings.h.iso2022
encodings.o.iso8859-5:	encodings.h.iso8859-5
encodings.o.iso8859-7:	encodings.c.iso8859-7
encodings.o.iso8859-7:	encodings.h.encoding
encodings.o.iso8859-7:	encodings.h.iso10646
encodings.o.iso8859-7:	encodings.h.utf8
encodings.o.iso8859-7:	encodings.h.iso10646
encodings.o.iso8859-7:	encodings.h.iso2022
encodings.o.iso8859-7:	encodings.h.iso8859-7
encodings.o.iso8859-8:	encodings.c.iso8859-8
encodings.o.iso8859-8:	encodings.h.encoding
encodings.o.iso8859-8:	encodings.h.iso10646
encodings.o.iso8859-8:	encodings.h.utf8
encodings.o.iso8859-8:	encodings.h.iso10646
encodings.o.iso8859-8:	encodings.h.iso2022
encodings.o.iso8859-8:	encodings.h.iso8859-8
encodings.o.iso8859-9:	encodings.c.iso8859-9
encodings.o.iso8859-9:	encodings.h.encoding
encodings.o.iso8859-9:	encodings.h.iso10646
encodings.o.iso8859-9:	encodings.h.utf8
encodings.o.iso8859-9:	encodings.h.iso10646
encodings.o.iso8859-9:	encodings.h.iso2022
encodings.o.iso8859-9:	encodings.h.iso8859-9
encodings.o.iso8859-10:	encodings.c.iso8859-10
encodings.o.iso8859-10:	encodings.h.encoding
encodings.o.iso8859-10:	encodings.h.iso10646
encodings.o.iso8859-10:	encodings.h.iso2022
encodings.o.iso8859-10:	encodings.h.iso8859-10
encodings.o.iso8859-13:	encodings.c.iso8859-13
encodings.o.iso8859-13:	encodings.h.encoding
encodings.o.iso8859-13:	encodings.h.iso10646
encodings.o.iso8859-13:	encodings.h.iso2022
encodings.o.iso8859-13:	encodings.h.iso8859-13
encodings.o.sami:	encodings.c.sami
encodings.o.sami:	encodings.h.encoding
encodings.o.sami:	encodings.h.iso10646
encodings.o.sami:	encodings.h.iso2022
encodings.o.sami:	encodings.h.sami
encodings.o.koi8r:	encodings.c.koi8r
encodings.o.koi8r:	encodings.h.encoding
encodings.o.koi8r:	encodings.h.iso10646
encodings.o.koi8r:	encodings.h.koi8r
encodings.o.welsh:	encodings.c.welsh
encodings.o.welsh:	encodings.h.encoding
encodings.o.welsh:	encodings.h.iso10646
encodings.o.welsh:	encodings.h.utf8
encodings.o.welsh:	encodings.h.iso10646
encodings.o.welsh:	encodings.h.iso2022
encodings.o.welsh:	encodings.h.welsh
encodings.o.microsoft:	encodings.c.microsoft
encodings.o.microsoft:	encodings.h.encoding
encodings.o.microsoft:	encodings.h.iso10646
encodings.o.microsoft:	encodings.h.koi8r
encodings.o.microsoft:	encodings.h.microsoft
encodings.o.apple:	encodings.c.apple
encodings.o.apple:	encodings.h.encoding
encodings.o.apple:	encodings.h.iso10646
encodings.o.apple:	encodings.h.koi8r
encodings.o.apple:	encodings.h.apple
encodings.o.shiftjis:	encodings.c.shiftjis
encodings.o.shiftjis:	encodings.h.encoding
encodings.o.shiftjis:	encodings.h.iso10646
encodings.o.shiftjis:	encodings.h.utf8
encodings.o.shiftjis:	encodings.h.iso10646
encodings.o.shiftjis:	encodings.h.iso2022
encodings.o.shiftjis:	encodings.h.shiftjis
encodings.o.bigfive:	encodings.c.bigfive
encodings.o.bigfive:	encodings.h.encoding
encodings.o.bigfive:	encodings.h.iso10646
encodings.o.bigfive:	encodings.h.utf8
encodings.o.bigfive:	encodings.h.iso10646
encodings.o.bigfive:	encodings.h.iso2022
encodings.o.bigfive:	encodings.h.bigfive
encodings.o.euc-jp:	encodings.c.euc-jp
encodings.o.euc-jp:	encodings.h.encoding
encodings.o.euc-jp:	encodings.h.iso10646
encodings.o.euc-jp:	encodings.h.utf8
encodings.o.euc-jp:	encodings.h.iso10646
encodings.o.euc-jp:	encodings.h.iso2022
encodings.o.euc-jp:	encodings.h.euc-jp
encodings.o.euc-kr:	encodings.c.euc-kr
encodings.o.euc-kr:	encodings.h.encoding
encodings.o.euc-kr:	encodings.h.iso10646
encodings.o.euc-kr:	encodings.h.utf8
encodings.o.euc-kr:	encodings.h.iso10646
encodings.o.euc-kr:	encodings.h.iso2022
encodings.o.euc-kr:	encodings.h.euc-kr
encodings.o.euc-cn:	encodings.c.euc-cn
encodings.o.euc-cn:	encodings.h.encoding
encodings.o.euc-cn:	encodings.h.iso10646
encodings.o.euc-cn:	encodings.h.utf8
encodings.o.euc-cn:	encodings.h.iso10646
encodings.o.euc-cn:	encodings.h.iso2022
encodings.o.euc-cn:	encodings.h.euc-cn
encodings.o.enc_utf8:	encodings.c.enc_utf8
encodings.o.enc_utf8:	encodings.h.encoding
encodings.o.enc_utf8:	encodings.h.iso10646
encodings.o.enc_utf8:	encodings.h.utf8
encodings.o.enc_utf8:	encodings.h.iso10646
encodings.o.enc_utf8:	encodings.h.iso2022
encodings.o.enc_utf8:	encodings.h.enc_utf8
encodings.o.enc_ascii:	encodings.c.enc_ascii
encodings.o.enc_ascii:	encodings.h.encoding
encodings.o.enc_ascii:	encodings.h.iso10646
encodings.o.enc_ascii:	encodings.h.enc_ascii
encodings.o.enc_utf16:	encodings.c.enc_utf16
encodings.o.enc_utf16:	encodings.h.encoding
encodings.o.enc_utf16:	encodings.h.iso10646
encodings.o.enc_utf16:	encodings.h.iso2022
encodings.o.enc_utf16:	encodings.h.enc_utf16
encodings.o.enc_ucs4:	encodings.c.enc_ucs4
encodings.o.enc_ucs4:	encodings.h.encoding
encodings.o.enc_ucs4:	encodings.h.iso10646
encodings.o.enc_ucs4:	encodings.h.enc_ucs4
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
z.display:	encodings.h.encoding
z.display:	encodings.h.iso10646
z.display:	C:h.ctype
z.display:	C:h.stdio
z.display:	encodings.h.iso10646
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
z.malloc:	encodings.h.encoding
z.malloc:	encodings.h.iso10646
z.malloc:	C:h.ctype
z.rules:	c.rules
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	h.internal
z.rules:	h.tags
z.rules:	h.struct
z.rules:	h.html2_ext
z.rules:	h.tablestruc
z.rules:	encodings.h.encoding
z.rules:	encodings.h.iso10646
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
z.veneers:	h.tags
z.veneers:	h.struct
z.veneers:	h.tablestruc
z.veneers:	encodings.h.encoding
z.veneers:	encodings.h.iso10646
z.veneers:	C:h.ctype
z.veneers:	tbox:h.flex
z.veneers:	C:h.stdio
z.veneers:	C:h.swis
z.veneers:	C:h.kernel
z.veneers:	h.HTMLTables
z.veneers:	h.html2_ext
z.veneers:	h.tags
z.veneers:	h.internal
z.veneers:	h.tablestruc
z.veneers:	encodings.h.encoding
z.veneers:	encodings.h.iso10646
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
z.new_stream:	encodings.h.encoding
z.new_stream:	encodings.h.iso10646
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
z.HTTPSuport:	encodings.h.encoding
z.HTTPSuport:	encodings.h.iso10646
z.HTTPSuport:	C:h.ctype
z.HTTPSuport:	C:h.stdio
z.HTTPSuport:	encodings.h.encoding
z.HTTPSuport:	encodings.h.iso10646
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
z.font:	encodings.h.encoding
z.font:	encodings.h.iso10646
z.font:	C:h.ctype
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
z.HTMLTables:	encodings.h.encoding
z.HTMLTables:	encodings.h.iso10646
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
z.attributes:	h.tags
z.attributes:	h.struct
z.attributes:	h.tablestruc
z.attributes:	encodings.h.encoding
z.attributes:	encodings.h.iso10646
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
z.attrvenrs:	encodings.h.encoding
z.attrvenrs:	encodings.h.iso10646
z.attrvenrs:	C:h.ctype
z.attrvenrs:	h.elementinf
encodings.z.utf8:	encodings.c.utf8
encodings.z.utf8:	encodings.h.utf8
encodings.z.utf8:	encodings.h.iso10646
encodings.z.encoding:	encodings.c.encoding
encodings.z.encoding:	C:h.swis
encodings.z.encoding:	C:h.kernel
encodings.z.encoding:	encodings.h.encoding
encodings.z.encoding:	encodings.h.iso10646
encodings.z.encoding:	encodings.h.enc_ascii
encodings.z.encoding:	encodings.h.iso8859-1
encodings.z.encoding:	encodings.h.iso8859-2
encodings.z.encoding:	encodings.h.iso8859-3
encodings.z.encoding:	encodings.h.iso8859-4
encodings.z.encoding:	encodings.h.iso8859-5
encodings.z.encoding:	encodings.h.iso8859-7
encodings.z.encoding:	encodings.h.iso8859-8
encodings.z.encoding:	encodings.h.iso8859-9
encodings.z.encoding:	encodings.h.iso8859-10
encodings.z.encoding:	encodings.h.iso8859-13
encodings.z.encoding:	encodings.h.sami
encodings.z.encoding:	encodings.h.welsh
encodings.z.encoding:	encodings.h.iso2022
encodings.z.encoding:	encodings.h.shiftjis
encodings.z.encoding:	encodings.h.bigfive
encodings.z.encoding:	encodings.h.koi8r
encodings.z.encoding:	encodings.h.microsoft
encodings.z.encoding:	encodings.h.apple
encodings.z.encoding:	encodings.h.euc-jp
encodings.z.encoding:	encodings.h.euc-kr
encodings.z.encoding:	encodings.h.euc-cn
encodings.z.encoding:	encodings.h.enc_utf8
encodings.z.encoding:	encodings.h.enc_utf16
encodings.z.encoding:	encodings.h.enc_ucs4
encodings.z.iso2022:	encodings.c.iso2022
encodings.z.iso2022:	encodings.h.encoding
encodings.z.iso2022:	encodings.h.iso10646
encodings.z.iso2022:	C:h.swis
encodings.z.iso2022:	C:h.kernel
encodings.z.iso2022:	encodings.h.iso2022
encodings.z.iso8859-1:	encodings.c.iso8859-1
encodings.z.iso8859-1:	encodings.h.encoding
encodings.z.iso8859-1:	encodings.h.iso10646
encodings.z.iso8859-1:	encodings.h.utf8
encodings.z.iso8859-1:	encodings.h.iso10646
encodings.z.iso8859-1:	encodings.h.iso2022
encodings.z.iso8859-1:	encodings.h.iso8859-1
encodings.z.iso8859-2:	encodings.c.iso8859-2
encodings.z.iso8859-2:	encodings.h.encoding
encodings.z.iso8859-2:	encodings.h.iso10646
encodings.z.iso8859-2:	encodings.h.utf8
encodings.z.iso8859-2:	encodings.h.iso10646
encodings.z.iso8859-2:	encodings.h.iso2022
encodings.z.iso8859-2:	encodings.h.iso8859-2
encodings.z.iso8859-3:	encodings.c.iso8859-3
encodings.z.iso8859-3:	encodings.h.encoding
encodings.z.iso8859-3:	encodings.h.iso10646
encodings.z.iso8859-3:	encodings.h.utf8
encodings.z.iso8859-3:	encodings.h.iso10646
encodings.z.iso8859-3:	encodings.h.iso2022
encodings.z.iso8859-3:	encodings.h.iso8859-3
encodings.z.iso8859-4:	encodings.c.iso8859-4
encodings.z.iso8859-4:	encodings.h.encoding
encodings.z.iso8859-4:	encodings.h.iso10646
encodings.z.iso8859-4:	encodings.h.utf8
encodings.z.iso8859-4:	encodings.h.iso10646
encodings.z.iso8859-4:	encodings.h.iso2022
encodings.z.iso8859-4:	encodings.h.iso8859-4
encodings.z.iso8859-5:	encodings.c.iso8859-5
encodings.z.iso8859-5:	encodings.h.encoding
encodings.z.iso8859-5:	encodings.h.iso10646
encodings.z.iso8859-5:	encodings.h.utf8
encodings.z.iso8859-5:	encodings.h.iso10646
encodings.z.iso8859-5:	encodings.h.iso2022
encodings.z.iso8859-5:	encodings.h.iso8859-5
encodings.z.iso8859-7:	encodings.c.iso8859-7
encodings.z.iso8859-7:	encodings.h.encoding
encodings.z.iso8859-7:	encodings.h.iso10646
encodings.z.iso8859-7:	encodings.h.utf8
encodings.z.iso8859-7:	encodings.h.iso10646
encodings.z.iso8859-7:	encodings.h.iso2022
encodings.z.iso8859-7:	encodings.h.iso8859-7
encodings.z.iso8859-8:	encodings.c.iso8859-8
encodings.z.iso8859-8:	encodings.h.encoding
encodings.z.iso8859-8:	encodings.h.iso10646
encodings.z.iso8859-8:	encodings.h.utf8
encodings.z.iso8859-8:	encodings.h.iso10646
encodings.z.iso8859-8:	encodings.h.iso2022
encodings.z.iso8859-8:	encodings.h.iso8859-8
encodings.z.iso8859-9:	encodings.c.iso8859-9
encodings.z.iso8859-9:	encodings.h.encoding
encodings.z.iso8859-9:	encodings.h.iso10646
encodings.z.iso8859-9:	encodings.h.utf8
encodings.z.iso8859-9:	encodings.h.iso10646
encodings.z.iso8859-9:	encodings.h.iso2022
encodings.z.iso8859-9:	encodings.h.iso8859-9
encodings.z.iso8859-10:	encodings.c.iso8859-10
encodings.z.iso8859-10:	encodings.h.encoding
encodings.z.iso8859-10:	encodings.h.iso10646
encodings.z.iso8859-10:	encodings.h.iso2022
encodings.z.iso8859-10:	encodings.h.iso8859-10
encodings.z.iso8859-13:	encodings.c.iso8859-13
encodings.z.iso8859-13:	encodings.h.encoding
encodings.z.iso8859-13:	encodings.h.iso10646
encodings.z.iso8859-13:	encodings.h.iso2022
encodings.z.iso8859-13:	encodings.h.iso8859-13
encodings.z.sami:	encodings.c.sami
encodings.z.sami:	encodings.h.encoding
encodings.z.sami:	encodings.h.iso10646
encodings.z.sami:	encodings.h.iso2022
encodings.z.sami:	encodings.h.sami
encodings.z.koi8r:	encodings.c.koi8r
encodings.z.koi8r:	encodings.h.encoding
encodings.z.koi8r:	encodings.h.iso10646
encodings.z.koi8r:	encodings.h.koi8r
encodings.z.welsh:	encodings.c.welsh
encodings.z.welsh:	encodings.h.encoding
encodings.z.welsh:	encodings.h.iso10646
encodings.z.welsh:	encodings.h.utf8
encodings.z.welsh:	encodings.h.iso10646
encodings.z.welsh:	encodings.h.iso2022
encodings.z.welsh:	encodings.h.welsh
encodings.z.microsoft:	encodings.c.microsoft
encodings.z.microsoft:	encodings.h.encoding
encodings.z.microsoft:	encodings.h.iso10646
encodings.z.microsoft:	encodings.h.koi8r
encodings.z.microsoft:	encodings.h.microsoft
encodings.z.apple:	encodings.c.apple
encodings.z.apple:	encodings.h.encoding
encodings.z.apple:	encodings.h.iso10646
encodings.z.apple:	encodings.h.koi8r
encodings.z.apple:	encodings.h.apple
encodings.z.shiftjis:	encodings.c.shiftjis
encodings.z.shiftjis:	encodings.h.encoding
encodings.z.shiftjis:	encodings.h.iso10646
encodings.z.shiftjis:	encodings.h.utf8
encodings.z.shiftjis:	encodings.h.iso10646
encodings.z.shiftjis:	encodings.h.iso2022
encodings.z.shiftjis:	encodings.h.shiftjis
encodings.z.bigfive:	encodings.c.bigfive
encodings.z.bigfive:	encodings.h.encoding
encodings.z.bigfive:	encodings.h.iso10646
encodings.z.bigfive:	encodings.h.utf8
encodings.z.bigfive:	encodings.h.iso10646
encodings.z.bigfive:	encodings.h.iso2022
encodings.z.bigfive:	encodings.h.bigfive
encodings.z.euc-jp:	encodings.c.euc-jp
encodings.z.euc-jp:	encodings.h.encoding
encodings.z.euc-jp:	encodings.h.iso10646
encodings.z.euc-jp:	encodings.h.utf8
encodings.z.euc-jp:	encodings.h.iso10646
encodings.z.euc-jp:	encodings.h.iso2022
encodings.z.euc-jp:	encodings.h.euc-jp
encodings.z.euc-kr:	encodings.c.euc-kr
encodings.z.euc-kr:	encodings.h.encoding
encodings.z.euc-kr:	encodings.h.iso10646
encodings.z.euc-kr:	encodings.h.utf8
encodings.z.euc-kr:	encodings.h.iso10646
encodings.z.euc-kr:	encodings.h.iso2022
encodings.z.euc-kr:	encodings.h.euc-kr
encodings.z.euc-cn:	encodings.c.euc-cn
encodings.z.euc-cn:	encodings.h.encoding
encodings.z.euc-cn:	encodings.h.iso10646
encodings.z.euc-cn:	encodings.h.utf8
encodings.z.euc-cn:	encodings.h.iso10646
encodings.z.euc-cn:	encodings.h.iso2022
encodings.z.euc-cn:	encodings.h.euc-cn
encodings.z.enc_utf8:	encodings.c.enc_utf8
encodings.z.enc_utf8:	encodings.h.encoding
encodings.z.enc_utf8:	encodings.h.iso10646
encodings.z.enc_utf8:	encodings.h.utf8
encodings.z.enc_utf8:	encodings.h.iso10646
encodings.z.enc_utf8:	encodings.h.iso2022
encodings.z.enc_utf8:	encodings.h.enc_utf8
encodings.z.enc_ascii:	encodings.c.enc_ascii
encodings.z.enc_ascii:	encodings.h.encoding
encodings.z.enc_ascii:	encodings.h.iso10646
encodings.z.enc_ascii:	encodings.h.enc_ascii
encodings.z.enc_utf16:	encodings.c.enc_utf16
encodings.z.enc_utf16:	encodings.h.encoding
encodings.z.enc_utf16:	encodings.h.iso10646
encodings.z.enc_utf16:	encodings.h.iso2022
encodings.z.enc_utf16:	encodings.h.enc_utf16
encodings.z.enc_ucs4:	encodings.c.enc_ucs4
encodings.z.enc_ucs4:	encodings.h.encoding
encodings.z.enc_ucs4:	encodings.h.iso10646
encodings.z.enc_ucs4:	encodings.h.enc_ucs4
