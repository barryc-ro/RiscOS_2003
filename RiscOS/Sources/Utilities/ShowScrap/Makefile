# Makefile for ShowScrap
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 21-Nov-94  AMcC   Updated for Black build
#

#
# Program specific options:
#
COMPONENT  = ShowScrap
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${INSTDIR}.${APP}

# Generic options:
#
MKDIR   = cdir
CP      = copy
RM      = remove
SQUISH  = squish
WIPE    = -wipe

CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v

FILES  =\
 $(LDIR).!Help\
 $(RDIR).!Run\
 $(LDIR).!Sprites\
 $(LDIR).!Sprites22

#
# GENERIC RULES
#                  
all: $(FILES)
          @echo $(COMPONENT): all build complete

install: $(FILES)
         $(MKDIR) $(INSTDIR)
         Set Alias$CPFD $(CP) %0.%1 $(INSTDIR).%1 $(CPFLAGS)
         CPFD $(LDIR) !Help
         CPFD $(RDIR) !Run
         CPFD $(LDIR) !Sprites
         CPFD $(LDIR) !Sprites22
         Unset Alias$CPFD
         Access ${INSTDIR} /r
         Access ${INSTDIR}.* WR/r
         @echo $(COMPONENT): installed

clean:
         @echo $(COMPONENT): cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
