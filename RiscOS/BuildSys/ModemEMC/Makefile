# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MSGDIR  = ${SRCDIR}.ModemEMC.EMCMsgs
RESDIR  = ${MSGDIR}.Resources.<Locale>.<System>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 UARTDriver \
 CMOSReset \
 Messages

EXPORT =

RESOURCE = \
 EMCRemote \
 ModemBoot

#
# Build rules:
#
clean: ${BUILD}
        @echo clean: Complete

export: ${BUILD} ${EXPORT} dirs
        @echo export: Complete

resources: ${BUILD} ${RESOURCE} dirs
        @echo resources: Complete

# podule ROM build
all: ${BUILD}
        @echo all: Complete

install: ROMImage
        @echo install: Complete

dirs:
        set Resource$Dir ${MSGDIR}.Resources.<Locale>.<System>
        ${MKDIR} ${MSGDIR}.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Resources
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Resources2
        ${MKDIR} ${MSGDIR}.Resources.${LOCALE}.${SYSTEM}.Apps
#
# Component rules:
#

ROMImage: ${BUILD}
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.Images
        @${CD} ${SYSDIR}.ModemEMC.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

ModemBoot:
        @echo $@ ...
        @${CD} ${SRCDIR}.ModemEMC.ModemBoot
        ${MAKE}

EMCRemote:
        @echo $@ ...
        @${CD} ${SRCDIR}.ModemEMC.EMCRemote
        ${MAKE}

Messages:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.EMCRom
        @${CD} ${SRCDIR}.ModemEMC.EMCMsgs
        ${MAKE} INSTDIR=${INSTDIR}.EMCRom

CMOSReset:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.EMCRom
        @${CD} ${SRCDIR}.ModemEMC.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.EMCRom

UARTDriver:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.EMCRom
        @${CD} ${SRCDIR}.HWSupport.UARTDriver
        ${MAKE} INSTDIR=${INSTDIR}.EMCRom

