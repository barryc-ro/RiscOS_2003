# Makefile for MovieFS Resources
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 13-Jul-01      BJGA   Created
#

#
# Generic options:
#

include StdTools

#
# Program specific options:
#

COMPONENT = MovieFS
RESFSDIR  = ${RESDIR}.^.Apps.!MovieFS

#
# Build rules:
#

install: copy ${OPTIONS} init_install
        @echo ${COMPONENT}: installed

resources: copy ${OPTIONS} init_resources
        @echo ${COMPONENT}: resource files copied

init_install: init_common
        ${MKDIR} ${INSTDIR}
        If "${OPTIONS}"="FrontEnd RealAudio" then ${MKDIR} ${INSTDIR}.^.!RASetup
        @Set Alias$CMF IfThere %0 Then ${CP} %0 ${INSTDIR}.%1 ${CPFLAGS}

init_resources: init_common
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        ${MKDIR} ${RESFSDIR}
        @set Alias$CMF IfThere %0 Then ShrinkWrapZipCompress %0 ${RESFSDIR}.%1

init_common:
        @Set MovieFSLevel2$Path Resources.
        @Set MovieFSRes$Path MovieFSLevel2:
        @Set MovieFSLocalRes$Path MovieFSRes:<Locale>.,MovieFSRes:UK.

FrontEnd:
        @Set MovieFSLevel2$Path Resources.FrontEnd.,Resources.

RealAudio:
        @Set MovieFSRes$Path MovieFSLevel2:RealAudio.,MovieFSLevel2:

copy:
        CMF MovieFSRes:!AutoRun       !AutoRun
        CMF MovieFSRes:!Boot          !Boot
        CMF MovieFSRes:!Changes       !Changes
        CMF MovieFSRes:!FrontRun      !FrontRun
        CMF MovieFSLocalRes:!Help     !Help
        CMF MovieFSRes:!Run           !Run
        CMF MovieFSRes:!Sprites       !Sprites
        CMF MovieFSRes:!Sprites22     !Sprites22
        CMF MovieFSRes:AVIMap         AVIMap
        CMF MovieFSRes:BarSprite      BarSprite
        CMF MovieFSRes:Configure      Configure
        CMF MovieFSRes:RMFile         RMFile
        CMF MovieFSLocalRes:Templates Templates
        CMF MovieFSRes:!RunRAS        ^.!RASetup.!Run
        CMF MovieFSRes:!SpriteRAS     ^.!RASetup.!Sprites
        @Unset MovieFSLevel2$Path
        @Unset MovieFSRes$Path
        @Unset MovieFSLocalRes$Path
        @Unset Alias$CMF

#
# Dynamic dependencies:
