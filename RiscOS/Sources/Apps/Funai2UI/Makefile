# Makefile for Funai2UI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Sep-98  SBF          Created from FunaiUI Makefile
#

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cf~vr

#
# Program specific options:
#
COMPONENT = Funai2UI
APP       = !Boot
SOURCE    =
TARGET    =

#
# Export Paths for Messages module
#
EXPORTDIR = $(RESDIR).^
RESAPP    = $(EXPORTDIR).$(APP)
RESRES    = $(EXPORTDIR).Resources

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: ROM module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: ROM module installed

clean:
        @echo ${COMPONENT}: cleaned

local_clean:
        $(WIPE) Translated $(WFLAGS)
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        @echo ** Creating directory structure
        $(MKDIR) $(EXPORTDIR)
        $(MKDIR) $(RESAPP)
        $(MKDIR) $(RESRES)
        $(MKDIR) $(RESRES).NVRAM
        $(MKDIR) $(RESRES).Wimp
        $(CP) Resources.$(USERIF).UK.$(APP) $(RESAPP) T~V~CFQR

resources: dirs
        ${CP} Resources.<UserIf>.${LOCALE}.${APP} ${RESAPP} ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.NVRAM.Tags ${RESRES}.NVRAM.Tags ${CPFLAGS}
        ${CP} LocalUserIFRes:Wimp.Sprites ${RESRES}.Wimp.Sprites ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied
        
# Dynamic dependencies:
