# Makefile for slider screensaver settings app
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 03-Apr-98  RML    Created

#
# Program specific options:
#
COMPONENT  = Slider !Settings
APP        = !Settings
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
LD      = link
CP      = copy
RM      = remove
SQUISH  = squish
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit -module
CFLAGS  = ${THROWBACK} -depend !Depend -ffa ${INCLUDES} 
CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v

#
# Libraries
#
CLIB       = CLib:o.stubs
EVENTLIB   = tbox:o.eventlib
TOOLBOXLIB = tbox:o.toolboxlib
WIMPLIB    = tbox:o.wimplib

#
# Include files
#
INCLUDES = -Itbox:,C:


FILES =\
 ${RDIR}.!Boot \
 ${RDIR}.!Run \
 ${RDIR}.!Sprites22 \
 ${RDIR}.!RunImage \
 ${LDIR}.Messages \
 ${LDIR}.Res

OBJS =\
  o.slideset

#
# Rule patterns
#
.SUFFIXES: .o

.c.o:; @echo
       @echo Compiling $<
       @${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<
 
#
# Main rules:
#

all:     ${FILES}
         @echo ${COMPONENT}: Application made.

install: ${FILES}
         @echo
         @echo ${COMPONENT}: Creating application ${APP}
         ${MKDIR} ${INSTDIR}.${APP}
         ${CP} ${RDIR}.!Boot      ${INSTDIR}.${APP}.!Boot      ${CPFLAGS}
         ${CP} ${RDIR}.!Run       ${INSTDIR}.${APP}.!Run       ${CPFLAGS}
         ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.${APP}.!Sprites22 ${CPFLAGS}
         ${CP} ${RDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage  ${CPFLAGS} 
         ${CP} ${LDIR}.Messages   ${INSTDIR}.${APP}.Messages   ${CPFLAGS}
         ${CP} ${LDIR}.Res        ${INSTDIR}.${APP}.Res        ${CPFLAGS}
         Access ${INSTDIR}.${APP}.* wr/r
         @echo
         @echo ${COMPONENT}: Application installed {Disc}

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} ^.!Slider.${APP}.* ${WFLAGS}
        ${RM} ${RDIR}.!RunImage
        ${RM} ^.!Slider.${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

Resources.!RunImage: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${CLIB}
       ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.slideset:	c.slideset
o.slideset:	C:h.swis
o.slideset:	C:h.kernel
o.slideset:	tbox:h.toolbox
o.slideset:	tbox:h.wimp
o.slideset:	tbox:h.event
o.slideset:	tbox:h.window
o.slideset:	tbox:h.gadgets
o.slideset:	tbox:h.wimplib
