# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MAKE    = @amu ${MFLAGS}

BUILD = \
 CMOSReset \
 CMOSReset7 \
 CMOSReset6 \
 CMOSReset5 \
 CMOSReset4 \
 CMOSReset3 \
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
        @${CD} ${SYSDIR}.CCTrial.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

Diagnostic:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.Networking.Diagnostic
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

ATMstbL:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.Networking.ATMstbL
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

NetConfig:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.Networking.NetConfig
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset3:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset4:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset5:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset6:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset7:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

CMOSReset:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.CCTrial
        @${CD} ${SRCDIR}.Trial.CCTrial.CMOSReset
        ${MAKE} INSTDIR=${INSTDIR}.CCTrial

