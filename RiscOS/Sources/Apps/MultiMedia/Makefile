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
TDIR      = rm.<Build>
TARGET    = ${TDIR}.${COMPONENT}
RESDIR    = ShrunkRes.<Build>
UTILS     = Utils
DATA      = Data.<Build>
# Note: RESDATA mustn't be in RESDIR !!
RESDATA   = ${DATA}.Data
SHRINKFILE = ${DATA}.MkShrink
SPECIALFILE = ${DATA}.MkSpecial
SPECIALMFILE = ${DATA}.MkSpecialM

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

${RESDATA}: ${SHRINKFILE} ${SPECIALFILE} ${SPECIALMFILE}
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        Run ${SHRINKFILE}
        IfThere ${SPECIALFILE} Then Run ${SPECIALFILE}
        IfThere ${SPECIALMFILE} Then Run ${SPECIALMFILE}
        ${SCANRES} ${RESDIR} $@

${SHRINKFILE}:
        ${BUILDWRAP} Resources ${RESDIR} $@

${SPECIALFILE}:
        IfThere Special.<UserIF> Then ${BUILDWRAP} Special.<UserIF> ${RESDIR} $@

${SPECIALMFILE}:
        IfThere SpecialM.<Machine> Then ${BUILDWRAP} SpecialM.<Machine> ${RESDIR} $@

dirs:
        ${MKDIR} ${TDIR}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${DATA}

# Dynamic dependencies:
