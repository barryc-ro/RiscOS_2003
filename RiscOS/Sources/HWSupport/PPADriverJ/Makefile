# Makefile for PPADriverJ
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
COMPONENT  = PPADriverJ
TARGET     = rm.${COMPONENT}
SRC	   = s.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
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
rm.PPADriverJ: Hdr:ListOpts
rm.PPADriverJ: Hdr:Macros
rm.PPADriverJ: Hdr:System
rm.PPADriverJ: Hdr:SWIs
rm.PPADriverJ: Hdr:CPU.Generic26
rm.PPADriverJ: Hdr:IO.GenericIO
rm.PPADriverJ: Hdr:RISCOS
rm.PPADriverJ: Hdr:ModHand
rm.PPADriverJ: Hdr:Parallel
rm.PPADriverJ: Hdr:Buffer
rm.PPADriverJ: Hdr:IZipFS
rm.PPADriverJ: Version
