# Makefile for TTXTView

#
# Program specific options:
#
COMPONENT  = TTXTView
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${APP}

# Generic options:
#

include Makefiles:StdTools

CFLAGS  = ${THROWBACK} -c -depend !Depend -fah -fussy ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
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
all: $(FILES)
     @echo $(COMPONENT): all built

install: $(FILES)
         $(MKDIR) ${INSTDIR}
         $(CP) $(LDIR).Messages  $(INSTDIR).Messages    $(CPFLAGS)
         $(CP) $(RDIR).!Run      $(INSTDIR).!Run        $(CPFLAGS)
         $(CP) $(RDIR).!Boot     $(INSTDIR).!Boot       $(CPFLAGS)
         $(CP) $(RDIR).TTXlaunch $(INSTDIR).TTXlaunch   $(CPFLAGS)
         $(CP) $(RDIR).Config    $(INSTDIR).Config      $(CPFLAGS)
         $(CP) $(RDIR).!RunImage $(INSTDIR).!RunImage   $(CPFLAGS)
         Access $(INSTDIR).* RW/R
         @echo $(COMPONENT): installed

clean:
        ${RM}   ${RDIR}.!RunImage
        ${WIPE} ${WPREFLAGS} ${APP}.* ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${RM}   ${APP}
        -stripdepnd
        @echo $(COMPONENT): cleaned

dirs:
        ${MKDIR} o

#
# Static dependencies:
#
${RDIR}.!RunImage: ${OBJS} ${CLIB} ${WIMPLIB} dirs
        ${LD} ${LINKFLAGS} -o $@ ${OBJS} ${CLIB} ${WIMPLIB} 
#        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
