# Makefile for MultiMedia module
#

#
# Generic options:
#
AS      = aasm
CP      = copy
MKDIR   = cdir
RM      = remove
WIPE    = -wipe
MAKE    = amu
BUILDWRAP = Utils.BuildWrap

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT = MultiMedia
SOURCE    = s.Messages
TARGET    = rm.${COMPONENT}
RESDIR    = ShrunkRes
UTILS     = Utils
DATA      = Data
# Note: RESDATA mustn't be in RESDIR !!
RESDATA   = ${DATA}.Data
SHRINKFILE = ${DATA}.MkShrink

#
# Utility programs:
#
SCANRES = ${UTILS}.ScanRes

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

clean:
        ${RM} ${TARGET}
        ${WIPE} ${RESDIR}.* ${WFLAGS}
        ${WIPE} ${DATA}.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${WIPE} ${DATA}.* ${WFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${RESDATA}
        set resdata ${RESDATA}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}
        unset resdata

${RESDATA}: ${SHRINKFILE}
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        Run ${SHRINKFILE}
        ${SCANRES} ${RESDIR} $@

${SHRINKFILE}:
        ${BUILDWRAP} Resources ${RESDIR} $@


# Dynamic dependencies:
rm.MultiMedia: Hdr:ListOpts
rm.MultiMedia: Hdr:Macros
rm.MultiMedia: Hdr:System
rm.MultiMedia: Hdr:SWIs
rm.MultiMedia: Hdr:CPU.Generic26
rm.MultiMedia: Hdr:IO.GenericIO
rm.MultiMedia: Hdr:RISCOS
rm.MultiMedia: Hdr:ModHand
rm.MultiMedia: Hdr:Services
rm.MultiMedia: Hdr:Proc
rm.MultiMedia: Hdr:ResourceFS
rm.MultiMedia: Version
rm.MultiMedia: Hdr:NDRDebug
