# Makefile for CDFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 03-Jan-95  AMcC         Now exports hdr.CDFS
# 27-Feb-95  AMcC   Resources now copied to CDFSRes
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = CDFS
EXPORTS   = ${EXP_HDR}.CDFS
SOURCE    = s.Main
TARGET    = rm.CDFS
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}
RESDIR    = ^.CDFSRes.Resources.${Locale}.Resources.${COMPONENT}

#
# Generic options:
#
MKDIR    = cdir
AS       = aasm
CP       = copy
RM       = remove
STRIP    = stripdepnd

CFLAGS   = -c -depend !Depend -IC:
AFLAGS   = -depend !Depend -Stamp -quit -module
CPFLAGS  = ~cfr~v

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @echo ${COMPONENT}: Resources are in CDFSRes

clean:
        ${RM} ${TARGET}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${AFLAGS} -To $@ -From ${SOURCE}

${EXP_HDR}.CDFS: hdr.CDFS
        ${CP} hdr.CDFS $@ ${CPFLAGS}

# Dynamic dependencies:
