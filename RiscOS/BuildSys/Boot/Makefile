# System Makefile

CD      = dir
MKDIR   = cdir
SYSDIR  = <build$dir>.System
SRCDIR  = <src$dir>
INSTDIR = <install$dir>
RESDIR  = <resource$dir>.Resources
MAKE    = @amu ${MFLAGS} RESDIR=${RESDIR}

BUILD = \
 ResetBoot \
 Public \
 PhotoCD \
 JCompMod \
 BASIC64 \
 System \
 Scrap \
 Fonts \
 FontMerge \
 Configure \
 ARMovie \
 PartNumber \
 Boot

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

Boot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4 COMPONENT=Boot

PartNumber:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${CD} ${SRCDIR}.SystemRes.PartNumber
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot PART=HardDisc

ARMovie:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.ARMovie
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources 

Configure:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources COMPONENT=Configure

FontMerge:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Library
        @Access ${INSTDIR}.HardDisc4.!Boot.Library /r
        @${CD} ${SRCDIR}.SystemRes.Configure
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Library COMPONENT=FontMerge

Fonts:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Fonts
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources INSTFLAG=all

Scrap:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.Scrap
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources 

System:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${CD} ${SRCDIR}.SystemRes.System
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources 

BASIC64:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.Programmer.BASIC
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules 

JCompMod:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules /r
        @${CD} ${SRCDIR}.OS_Core.Video.Render.jcompmod
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources.!System.350.Modules 

PhotoCD:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot
        @Access ${INSTDIR}.HardDisc4.!Boot /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.300
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.300 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.!Boot.Resources.!System.300.Modules
        @Access ${INSTDIR}.HardDisc4.!Boot.Resources.!System.300.Modules /r
        @${CD} ${SRCDIR}.Apps.PhotoView
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.!Boot.Resources.!System.300.Modules COMPONENT=PhotoCD

Public:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.Public
        @Access ${INSTDIR}.HardDisc4.Public /r
        @${CD} ${SRCDIR}.SystemRes.Public
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.Public 

ResetBoot:
        @echo $@ ...
        @${MKDIR} ${INSTDIR}.HardDisc4
        @Access ${INSTDIR}.HardDisc4 /r
        @${MKDIR} ${INSTDIR}.HardDisc4.Diversions
        @Access ${INSTDIR}.HardDisc4.Diversions /r
        @${MKDIR} ${INSTDIR}.HardDisc4.Diversions.Tools
        @Access ${INSTDIR}.HardDisc4.Diversions.Tools /r
        @${CD} ${SRCDIR}.SystemRes.Boot
        ${MAKE} INSTDIR=${INSTDIR}.HardDisc4.Diversions.Tools COMPONENT=ResetBoot

