# Makefile for ARWork
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 17 Nov 2000   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
CC              = cc
RM              = remove
LD              = link
WIPE            = -wipe

CFLAGS          = -c -depend !Depend $(THROWBACK) -ffah $(INCLUDES) $(DFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK) -Stamp -quit
CPFLAGS         = ~CFR~V
LDFLAGS         = 
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:
DFLAGS          = -DNDEBUG

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = ARWork
FILES           = LocalRes:!ReadMe \
                  Resources.!Boot \
                  LocalRes:!Help \
                  Resources.!Run \
                  Resources.!Sprites \
                  Resources.!Sprites22 \
                  Resources.!Sprites23

LIBS            = 
OBJS            = 

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(FILES)
        @echo $(COMPONENT): all complete

install: $(FILES)
        $(MKDIR) $(INSTDIR).!$(COMPONENT)
        |
        $(CP) LocalRes:!ReadMe      $(INSTDIR).!ReadMe                  $(CPFLAGS)
        |
        $(CP) Resources.!Boot       $(INSTDIR).!$(COMPONENT).!Boot      $(CPFLAGS)
        $(CP) LocalRes:!Help        $(INSTDIR).!$(COMPONENT).!Help      $(CPFLAGS)
        $(CP) Resources.!Run        $(INSTDIR).!$(COMPONENT).!Run       $(CPFLAGS)
        $(CP) Resources.!Sprites    $(INSTDIR).!$(COMPONENT).!Sprites   $(CPFLAGS)
        $(CP) Resources.!Sprites22  $(INSTDIR).!$(COMPONENT).!Sprites22 $(CPFLAGS)
        $(CP) Resources.!Sprites23  $(INSTDIR).!$(COMPONENT).!Sprites23 $(CPFLAGS)
        $(MKDIR) $(INSTDIR).!$(COMPONENT).Work.AREncode
        |
        @echo $(COMPONENT): installed

clean:
        @echo $(COMPONENT): cleaned

# ------------------------------------------------------------------------------
# Dynamic dependencies:
