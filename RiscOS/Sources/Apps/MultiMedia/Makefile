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
SPECIALSRC = Special
SPECIALBLD = ${DATA}.Special
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

${RESDATA}: ${SHRINKFILE} ${OPTIONS} u<UserIf> s<Machine>
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        Run ${SHRINKFILE}
        Repeat Run ${SPECIALBLD} -files -verbose
        ${SCANRES} ${RESDIR} $@

${SHRINKFILE}:
        ${BUILDWRAP} Resources ${RESDIR} $@

# Optional extra files
# to use, set in OPTIONS variable in the main build Config
# eg OPTIONS="RealAudio Cinepak"

Cinepak RealAudio u<UserIf> s<Machine>:
        IfThere ${SPECIALSRC}.$@ Then ${BUILDWRAP} ${SPECIALSRC}.$@ ${RESDIR} ${SPECIALBLD}.$@

dirs:
        ${MKDIR} ${TDIR}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${DATA}
        ${MKDIR} ${SPECIALBLD}

# Dynamic dependencies:
