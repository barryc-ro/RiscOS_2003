# Makefile for circles screensaver settings app
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
COMPONENT  = Circles setup
APP        = !SvrSetup
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
  o.settings

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
        ${WIPE} ^.!Circles.${APP}.* ${WFLAGS}
        ${RM} ${RDIR}.!RunImage
        ${RM} ^.!Circles.${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

Resources.!RunImage: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${CLIB}
       ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.settings:	c.settings
o.settings:	C:h.swis
o.settings:	C:h.kernel
o.settings:	tbox:h.toolbox
o.settings:	tbox:h.wimp
o.settings:	tbox:h.event
o.settings:	tbox:h.window
o.settings:	tbox:h.gadgets
o.settings:	tbox:h.wimplib
