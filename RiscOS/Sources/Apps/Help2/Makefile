# Makefile for Help2
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 31-Mar-98  RML    Created

#
# Program specific options:
#
COMPONENT  = Help2
APP        = !Help
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
CFLAGS  = ${THROWBACK} -depend !Depend -ffah ${INCLUDES} 
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
 ${LDIR}.!Help \
 ${RDIR}.!Run \
 ${RDIR}.!Sprites22 \
 ${RDIR}.!RunImage \
 ${LDIR}.Messages \
 ${LDIR}.Res

OBJS =\
  o.main \
  o.help \
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
         ${CP} ${LDIR}.!Help      ${INSTDIR}.${APP}.!Help      ${CPFLAGS}
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
        ${WIPE} ${APP}.*
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
o.main:	tbox:h.wimp
o.main:	tbox:h.wimplib
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	tbox:h.wimplib
o.main:	tbox:h.toolbox
o.main:	tbox:h.event
o.main:	tbox:h.window
o.main:	tbox:h.gadgets
o.main:	tbox:h.iconbar
o.main:	tbox:h.proginfo
o.main:	tbox:h.fontmenu
o.main:	tbox:h.menu
o.main:	h.help
o.main:	h.common
o.main:	h.defines
o.help:	c.help
o.help:	C:h.swis
o.help:	C:h.kernel
o.help:	tbox:h.gadgets
o.help:	tbox:h.toolbox
o.help:	tbox:h.wimp
o.help:	h.common
o.help:	h.help
o.common:	c.common
o.common:	C:h.kernel
o.common:	tbox:h.wimp
o.common:	tbox:h.wimplib
o.common:	tbox:h.toolbox
o.common:	tbox:h.gadgets
o.common:	C:h.swis
o.common:	h.common
o.common:	h.defines
