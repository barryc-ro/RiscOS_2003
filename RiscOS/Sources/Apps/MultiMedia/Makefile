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
TDIR      = rm.<UserIF>
TARGET    = ${TDIR}.${COMPONENT}
RESDIR    = ShrunkRes.<UserIF>
UTILS     = Utils
DATA      = Data.<UserIF>
# Note: RESDATA mustn't be in RESDIR !!
RESDATA   = ${DATA}.Data
SHRINKFILE = ${DATA}.MkShrink
SPECIALFILE = ${DATA}.MkSpecial

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
        ${WIPE} ${TDIR} ${WFLAGS}
        ${WIPE} ${RESDIR} ${WFLAGS}
        ${WIPE} ${DATA} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${WIPE} ${DATA} ${WFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${RESDATA} dirs
        set resdata ${RESDATA}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}
        unset resdata

${RESDATA}: ${SHRINKFILE} ${SPECIALFILE}
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        Run ${SHRINKFILE}
        IfThere ${SPECIALFILE} Then Run ${SPECIALFILE}
        ${SCANRES} ${RESDIR} $@

${SHRINKFILE}:
        ${BUILDWRAP} Resources ${RESDIR} $@

${SPECIALFILE}:
        IfThere Special.<UserIF> Then ${BUILDWRAP} Special.<UserIF> ${RESDIR} $@

dirs:
        ${MKDIR} ${TDIR}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${DATA}

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
rm.<UserIF>.MultiMedia: Hdr:ListOpts
rm.<UserIF>.MultiMedia: Hdr:Macros
rm.<UserIF>.MultiMedia: Hdr:System
rm.<UserIF>.MultiMedia: Hdr:SWIs
rm.<UserIF>.MultiMedia: Hdr:CPU.Generic26
rm.<UserIF>.MultiMedia: Hdr:IO.GenericIO
rm.<UserIF>.MultiMedia: Hdr:RISCOS
rm.<UserIF>.MultiMedia: Hdr:ModHand
rm.<UserIF>.MultiMedia: Hdr:Services
rm.<UserIF>.MultiMedia: Hdr:Proc
rm.<UserIF>.MultiMedia: Hdr:ResourceFS
rm.<UserIF>.MultiMedia: Version
rm.<UserIF>.MultiMedia: Hdr:NDRDebug
