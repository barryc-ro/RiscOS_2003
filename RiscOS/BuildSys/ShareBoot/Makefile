# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.System
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
RESDIR  = <resource$dir>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 ShareJCompMod \
 ShareBASIC64 \
 ShareSystem \
 ShareScrap \
 ShareFonts \
 ShareFontMerge \
 ShareConfigure \
 ShareBoot

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

ShareBoot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6 COMPONENT=ShareBoot

ShareConfigure:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources COMPONENT=Configure

ShareFontMerge:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Library
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Library COMPONENT=FontMerge

ShareFonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.Fonts
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources INSTFLAG=all

ShareScrap:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.Scrap
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources 

ShareSystem:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${CD} ${SRCDIR}.SystemRes.System
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources 

ShareBASIC64:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules 

ShareJCompMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules
        @${CD} ${SRCDIR}.OS_Core.Video.Render.jcompmod
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules 

