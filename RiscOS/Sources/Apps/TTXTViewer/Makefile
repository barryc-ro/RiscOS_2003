# Makefile for TTXTView

#
# Program specific options:
#
COMPONENT  = TTXTView
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

# Generic options:
#
MKDIR   = do mkdir -p
CC      = cc
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

CFLAGS  = ${THROWBACK} -c -depend !Depend -fah -fussy ${INCLUDES} ${DFLAGS}
CPFLAGS = ~c~fnr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v
LINKFLAGS =

#
# Libraries
#
CLIB       = CLib:o.stubs
WIMPLIB    = tbox:o.wimplib


#
# Include files
#
INCLUDES = -Itbox:,C:

DFLAGS   = 

FILES  =\
 $(LDIR).Messages\
 $(RDIR).!RunImage\
 $(RDIR).Config\
 $(RDIR).TTXLaunch\
 $(RDIR).!Boot\
 $(RDIR).!Run

OBJS = main.o\
       messages.o\
       teletext.o\
       config.o\
       keyboard.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<

#
# GENERIC RULES
#
all: $(FILES) dirs
     @echo $(COMPONENT): all built

install: $(FILES) dirs
         $(MKDIR) ${INSTDIR}
         Access $(INSTDIR) RW/R
         $(CP) $(LDIR).Messages  $(INSTDIR).Messages    $(CPFLAGS)
         Access $(INSTDIR).Messages RW/R
         $(CP) $(RDIR).!Run      $(INSTDIR).!Run        $(CPFLAGS)
         Access $(INSTDIR).!Run RW/R
         $(CP) $(RDIR).!Boot     $(INSTDIR).!Boot       $(CPFLAGS)
         Access $(INSTDIR).!Boot RW/R
         $(CP) $(RDIR).TTXlaunch $(INSTDIR).TTXlaunch   $(CPFLAGS)
         Access $(INSTDIR).TTXlaunch RW/R
         $(CP) $(RDIR).Config    $(INSTDIR).Config      $(CPFLAGS)
         Access $(INSTDIR).Config RW/R
         $(CP) $(RDIR).!RunImage $(INSTDIR).!RunImage   $(CPFLAGS)
         Access $(INSTDIR).!RunImage RW/R
         @echo $(COMPONENT): installed

clean:
        ${WIPE} o ${WFLAGS} 
        ${RM} ${RDIR}.!RunImage
        ${WIPE} ${INSTDIR} ${WFLAGS}
        @echo $(COMPONENT): cleaned

#
# Static dependencies:
#
${RDIR}.!RunImage: ${OBJS} ${CLIB} ${WIMPLIB}
        ${LD} ${LINKFLAGS} -o $@ ${OBJS} ${CLIB} ${WIMPLIB} 
        ${SQUEEZE} $@

dirs:
        ${MKDIR} o

#---------------------------------------------------------------------------
# Dynamic dependencies:
