# Makefile for AVIFetchFS
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

COMPONENT       = AVIFetchFS

# ------------------------------------------------------------------------------
# Build rules
#

install: copy init_install
        @echo $(COMPONENT): installed

resources: copy init_resources
        @echo $(COMPONENT): resource files copied

init_install:
        $(MKDIR) $(INSTDIR).$(COMPONENT)
        @Set Alias$CMF IfThere %0 Then $(CP) %0 $(INSTDIR).$(COMPONENT).%1 $(CPFLAGS)

init_resources:
        @RMEnsure ShrinkWrap 0.00 Error *** ShrinkWrap must be loaded ***
        $(MKDIR) $(RESDIR).$(COMPONENT)
        @set Alias$CMF IfThere %0 Then ShrinkWrapZipCompress %0 $(RESDIR).$(COMPONENT).%1

copy:
        CMF Resources.AVIFetchFS AVIFetchFS
        CMF Resources.mappings   mappings
        @Unset Alias$CMF

# ------------------------------------------------------------------------------
# Dynamic dependencies:
