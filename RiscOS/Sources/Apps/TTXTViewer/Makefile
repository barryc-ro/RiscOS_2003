# Makefile for TTXTView

#
# Program specific options:
#
COMPONENT  = TTXTView
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${INSTDIR}.${APP}

# Generic options:
#
MKDIR   = cdir
CC      = cc
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

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
        ${RM} o.main
        ${RM} o.messages
        ${RM} o.teletext
        ${RM} o.utils
        ${RM} o.config
        ${RM} o.keyboard
        ${RM} ${RDIR}.!RunImage
        @echo $(COMPONENT): cleaned

#
# Static dependencies:
#
${RDIR}.!RunImage: ${OBJS} ${CLIB} ${WIMPLIB} 
        ${LD} ${LINKFLAGS} -o $@ ${OBJS} ${CLIB} ${WIMPLIB} 
#        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.utils:	c.utils
o.utils:	h.utils
o.main: c.main h.messages tbox:h.toolbox C:h.kernel tbox:h.wimp C:h.swis h.main h.stdbool tbox:h.wimp h.teletext h.stdbool C:h.kernel h.config C:h.kernel h.config tbox:h.wimplib
o.messages: c.messages h.main h.stdbool tbox:h.wimp h.messages tbox:h.toolbox C:h.kernel C:h.swis tbox:h.toolbox C:h.swis tbox:h.wimplib h.stdbool
o.teletext: c.teletext h.teletext h.stdbool C:h.kernel h.config C:h.kernel h.messages tbox:h.toolbox tbox:h.wimp C:h.swis h.keyboard h.stdbool C:h.kernel h.config h.config h.main h.stdbool tbox:h.wimp
o.config: c.config h.main h.stdbool tbox:h.wimp h.config C:h.kernel h.messages tbox:h.toolbox C:h.swis
o.keyboard: c.keyboard h.keyboard h.stdbool C:h.kernel h.config C:h.kernel C:h.swis h.stdbool
