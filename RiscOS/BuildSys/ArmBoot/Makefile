# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.System
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
RESDIR  = <resource$dir>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 ArmJCompMod \
 ArmBASIC64 \
 ArmSystem \
 ArmFonts \
 ArmFontMerge \
 ArmConfigure \
 ArmBoot

EXPORT =

RESOURCE =

#
# Build rules:
#
clean: ${BUILD}
        @echo clean: Complete

export: ${EXPORT} ${BUILD}
        @echo export: Complete

# disc build
all: ${BUILD}
        @echo all: Complete

install: ${BUILD}
        @echo install: Complete

#
# Component rules:
#

ArmBoot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5 COMPONENT=ArmBoot

ArmConfigure:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources COMPONENT=Configure

ArmFontMerge:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Library
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Library COMPONENT=FontMerge

ArmFonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.Fonts
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources INSTFLAG=all

ArmSystem:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.System
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources 

ArmBASIC64:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules 

ArmJCompMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules
        @${CD} ${SRCDIR}.OS_Core.Video.Render.jcompmod
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules 

