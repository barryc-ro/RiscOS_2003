# Makefile for IZipFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 04-Apr-1997 Rich Buckley  Created
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
COMPONENT  = IZipFS
TARGET     = rm.${COMPONENT}
SRC	   = s.${COMPONENT}

#
# Generic rules:
#
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
        ${RM} ${ZIPFS}
        ${RM} ${ZIP_FILER}
        ${RM} ${PPA}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

dirs:
        ${MKDIR} rm

${TARGET}: ${SRC}
        ${AS} ${ASFLAGS} ${SRC}

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
