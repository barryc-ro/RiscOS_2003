# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.System
SRCDIR  = <src$dir>
INSTDIR = <discinstall$dir>
RESDIR  = <resource$dir>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 SharePhotoCD \
 ShareJCompMod \
 ShareBASIC64 \
 ShareSystem \
 ShareScrap \
 ShareFonts \
 ShareFontMerge \
 ShareConfigure \
 ShareBoot \
 ArmPhotoCD \
 ArmJCompMod \
 ArmBASIC64 \
 ArmSystem \
 ArmFonts \
 ArmFontMerge \
 ArmConfigure \
 ArmBoot \
 MchConfig

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

MchConfig:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc4
        @Access ${INSTDIR}.FDisc4 /r
        @${CD} ${SRCDIR}.SystemRes.MchConfig
        ${MAKE} INSTDIR=${INSTDIR}.FDisc4 

ArmBoot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5 COMPONENT=ArmBoot

ArmConfigure:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources COMPONENT=Configure

ArmFontMerge:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Library
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Library /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Library COMPONENT=FontMerge

ArmFonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Fonts
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources INSTFLAG=all

ArmSystem:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.System
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources 

ArmBASIC64:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules 

ArmJCompMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.OS_Core.Video.Render.jcompmod
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.350.Modules 

ArmPhotoCD:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc5
        @Access ${INSTDIR}.FDisc5 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot
        @Access ${INSTDIR}.FDisc5.!ArmBoot /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.300
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.300 /r
        @${MKDIR} ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.300.Modules
        @Access ${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.300.Modules /r
        @${CD} ${SRCDIR}.Apps.PhotoView
        ${MAKE} INSTDIR=${INSTDIR}.FDisc5.!ArmBoot.Resources.!System.300.Modules COMPONENT=PhotoCD

ShareBoot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6 COMPONENT=ShareBoot

ShareConfigure:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources COMPONENT=Configure

ShareFontMerge:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Library
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Library /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Library COMPONENT=FontMerge

ShareFonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Fonts
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources INSTFLAG=all

ShareScrap:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Scrap
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources 

ShareSystem:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.System
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources 

ShareBASIC64:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules 

ShareJCompMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.OS_Core.Video.Render.jcompmod
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.350.Modules 

SharePhotoCD:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.FDisc6
        @Access ${INSTDIR}.FDisc6 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot
        @Access ${INSTDIR}.FDisc6.!ShareBoot /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.300
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.300 /r
        @${MKDIR} ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.300.Modules
        @Access ${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.300.Modules /r
        @${CD} ${SRCDIR}.Apps.PhotoView
        ${MAKE} INSTDIR=${INSTDIR}.FDisc6.!ShareBoot.Resources.!System.300.Modules COMPONENT=PhotoCD

