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
 encodings.o.acorn\
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
 z.object\
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
 encodings.z.acorn\
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
