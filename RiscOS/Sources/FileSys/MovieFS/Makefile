# Makefile for MovieFS
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 15 Jan 2001   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
CP              = copy
RM              = remove
WIPE            = -wipe

CPFLAGS         = ~CFR~V
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = MovieFS

# ------------------------------------------------------------------------------
# Build rules
#

install: copy $(OPTIONS) init_install
        @echo $(COMPONENT): installed

resources: copy $(OPTIONS) init_resources
        @echo $(COMPONENT): resource files copied

init_install: init_common
        $(MKDIR) $(INSTDIR).!$(COMPONENT)
        @Set Alias$CMF IfThere %0 Then $(CP) %0 $(INSTDIR).!$(COMPONENT).%1 $(CPFLAGS)

init_resources: init_common
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        $(MKDIR) $(RESDIR).!$(COMPONENT)
        @set Alias$CMF IfThere %0 Then ShrinkWrapZipCompress %0 $(RESDIR).!$(COMPONENT).%1

init_common:
        @Set MovieFSLevel1$Path Resources.
        @Set MovieFSLevel2$Path MovieFSLevel1:
        @Set MovieFSRes$Path MovieFSLevel2:
        @Set MovieFSLocalRes$Path MovieFSRes:<Locale>.,MovieFSRes:UK.

FrontEnd:
        @Set MovieFSLevel1$Path Resources.FrontEnd.,Resources.

RealAudio:
        @Set MovieFSLevel2$Path MovieFSLevel1:RealAudio.,MovieFSLevel1:

Cinepak:
        @Set MovieFSRes$Path MovieFSLevel2:Cinepak.,MovieFSLevel2:

copy:
        CMF MovieFSRes:!AutoRun       !AutoRun
        CMF MovieFSRes:!Boot          !Boot
        CMF MovieFSRes:!Changes       !Changes
        CMF MovieFSRes:!FrontEnd      !FrontEnd
        CMF MovieFSRes:!FrontRun      !FrontRun
        CMF MovieFSLocalRes:!Help     !Help
        CMF MovieFSRes:!Run           !Run
        CMF MovieFSRes:!Sprites       !Sprites
        CMF MovieFSRes:!Sprites22     !Sprites22
        CMF MovieFSRes:AVIMap         AVIMap
        CMF MovieFSRes:BarSprite      BarSprite
        CMF MovieFSRes:Configure      Configure
        CMF MovieFSRes:Kickstart      Kickstart
        CMF MovieFSRes:MFSRA          MFSRA
        CMF MovieFSRes:MFSUniv        MFSUniv
        CMF MovieFSRes:MovieFSMod     MovieFSMod
        CMF MovieFSRes:PNA            PNA
        CMF MovieFSRes:RMFile         RMFile
        CMF MovieFSLocalRes:Templates Templates
        @Unset MovieFSLevel1$Path
        @Unset MovieFSLevel2$Path
        @Unset MovieFSRes$Path
        @Unset MovieFSLocalRes$Path
        @Unset Alias$CMF

# ------------------------------------------------------------------------------
# Dynamic dependencies:
