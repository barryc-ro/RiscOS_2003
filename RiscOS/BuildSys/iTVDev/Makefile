# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MAKE    = @amu ${MFLAGS}

BUILD = \
 CMOSReset4 \
 CMOSReset3 \
 CMOSReset2 \
 CMOSReset1 \
 ShareFS \
 FwSTB \
 NetConfig \
 ATMstbL \
 Diagnostic

EXPORT =

RESOURCE =

#
# Build rules:
#
clean: ${BUILD}
        @echo clean: Complete

export: ${BUILD} ${EXPORT} dirs
        @echo export: Complete

resources:
        @echo resources: Complete

# podule ROM build
all: ${BUILD}
        @echo all: Complete

install: ROMImage
        @echo install: Complete

#
# Component rules:
#

ROMImage: ${BUILD}
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.Images
        @${CD} ${SYSDIR}.iTVDev.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

Diagnostic:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.Networking.Diagnostic
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

ATMstbL:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.Networking.ATMstbL
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

NetConfig:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.NetConfig2
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

FwSTB:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.FreeWay
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

ShareFS:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.ShareFS
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

CMOSReset1:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

CMOSReset2:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

CMOSReset3:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

CMOSReset4:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.iTVDev
        @${CD} ${SRCDIR}.Trial.iTVDev.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.iTVDev

