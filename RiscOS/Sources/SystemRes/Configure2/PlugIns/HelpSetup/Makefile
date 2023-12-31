# Makefile for Help plug-in
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
COMPONENT  = HelpSetup
APP        = !${COMPONENT}
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
 ${RDIR}.CoSprite22 \
 ${RDIR}.!RunImage \
 ${LDIR}.Messages \
 ${LDIR}.Res

OBJS =\
  o.main \
  o.common

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
         ${CP} ${RDIR}.CoSprite22 ${INSTDIR}.${APP}.CoSprite22 ${CPFLAGS}
         ${CP} ${RDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage  ${CPFLAGS} 
         ${CP} ${LDIR}.Messages   ${INSTDIR}.${APP}.Messages   ${CPFLAGS}
         ${CP} ${LDIR}.Res        ${INSTDIR}.${APP}.Res        ${CPFLAGS}
         Access ${INSTDIR}.${APP}.* wr/r
         @echo
         @echo ${COMPONENT}: Application installed {Disc}

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} ${APP}.* ${WFLAGS}
        ${RM} ${RDIR}.!RunImage
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

Resources.!RunImage: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${CLIB}
       ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	tbox:h.toolbox
o.main:	tbox:h.wimp
o.main:	tbox:h.event
o.main:	h.common
o.main:	tbox:h.window
o.main:	tbox:h.gadgets
o.main:	tbox:h.fontmenu
o.main:	tbox:h.wimplib
o.main:	h.defines
o.common:	c.common
o.common:	C:h.kernel
o.common:	tbox:h.wimp
o.common:	tbox:h.wimplib
o.common:	tbox:h.toolbox
o.common:	tbox:h.gadgets
o.common:	C:h.swis
o.common:	h.defines
