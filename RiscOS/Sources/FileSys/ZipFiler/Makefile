# Makefile for ZipFiler
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Apr-1997 Rich Buckley  Created
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = ZipFiler
TARGET     = rm.${COMPONENT}
TARGETSA   = ${COMPONENT}
SRC	   = s.${COMPONENT}AB
SRCSA	   = s.standalone
#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} hdr.${COMPONENT} ${EXP_HDR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        ${CP} Resources.ZipFree ${RESDIR}.${COMPONENT}.ZipFree ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

dirs:
        ${MKDIR} rm

${TARGET}: ${SRC}
        ${AS} ${ASFLAGS} ${SRC}

${TARGETSA}: ${SRCSA}
        ${AS} ${ASFLAGS} ${SRCSA}

# Dynamic dependencies:
rm.ZipFS: Hdr:ListOpts
rm.ZipFS: Hdr:Macros
rm.ZipFS: Hdr:System
rm.ZipFS: Hdr:SWIs
rm.ZipFS: Hdr:CPU.Generic26
rm.ZipFS: Hdr:IO.GenericIO
rm.ZipFS: Hdr:RISCOS
rm.ZipFS: Hdr:ModHand
rm.ZipFS: Hdr:Parallel
rm.ZipFS: Hdr:FileCore
rm.ZipFS: Hdr:Free
rm.ZipFS: Hdr:ZipFS
rm.IZipFS: s.AcornHdr
rm.IZipFS: Hdr:ListOpts
rm.IZipFS: Hdr:Macros
rm.IZipFS: Hdr:System
rm.IZipFS: Hdr:SWIs
rm.IZipFS: Hdr:CPU.Generic26
rm.IZipFS: Hdr:IO.GenericIO
rm.IZipFS: Hdr:RISCOS
rm.IZipFS: Hdr:ModHand
rm.IZipFS: Hdr:Parallel
rm.IZipFS: Hdr:Buffer
rm.IZipFS: Hdr:FileCore
rm.IZipFS: Hdr:Free
rm.IZipFS: Hdr:IZipFS
rm.IZipFS: Version
rm.ZipFiler: s.ZipFilCore
rm.ZipFiler: s.AcornHdr
rm.ZipFiler: Hdr:ListOpts
rm.ZipFiler: Hdr:Macros
rm.ZipFiler: Hdr:System
rm.ZipFiler: Hdr:SWIs
rm.ZipFiler: Hdr:CPU.Generic26
rm.ZipFiler: Hdr:IO.GenericIO
rm.ZipFiler: Hdr:RISCOS
rm.ZipFiler: Hdr:ModHand
rm.ZipFiler: Hdr:Parallel
rm.ZipFiler: Hdr:Buffer
rm.ZipFiler: Hdr:Wimp
rm.ZipFiler: Hdr:ResourceFS
rm.ZipFiler: Hdr:MsgTrans
rm.ZipFiler: Hdr:Hourglass
rm.ZipFiler: Hdr:IZipFS
rm.ZipFiler: Version
ZipFiler: s.ZipFilerAB
ZipFiler: s.ZipFilCore
ZipFiler: s.AcornHdr
ZipFiler: Hdr:ListOpts
ZipFiler: Hdr:Macros
ZipFiler: Hdr:System
ZipFiler: Hdr:SWIs
ZipFiler: Hdr:CPU.Generic26
ZipFiler: Hdr:IO.GenericIO
ZipFiler: Hdr:RISCOS
ZipFiler: Hdr:ModHand
ZipFiler: Hdr:Parallel
ZipFiler: Hdr:Buffer
ZipFiler: Hdr:Wimp
ZipFiler: Hdr:ResourceFS
ZipFiler: Hdr:MsgTrans
ZipFiler: Hdr:Hourglass
ZipFiler: Hdr:IZipFS
ZipFiler: Version
