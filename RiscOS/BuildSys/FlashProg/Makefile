# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.BuildSys
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
MAKE    = @amu ${MFLAGS}

BUILD = \
 FlashApp \
 FlashProg

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
        @${CD} ${SYSDIR}.FlashProg.ROMImage
        ${MAKE} INSTDIR=${INSTDIR}.Images

FlashProg:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FlashProg
        @${CD} ${SRCDIR}.Trial.FlashProg.FlashProg
        ${MAKE} INSTDIR=${INSTDIR}.FlashProg

FlashApp:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FlashProg
        @${CD} ${SRCDIR}.Trial.FlashProg.FlashApp
        ${MAKE} INSTDIR=${INSTDIR}.FlashProg

